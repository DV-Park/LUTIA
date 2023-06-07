// Fill out your copyright notice in the Description page of Project Settings.


#include "LUTIA_SaveGame.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "LUTIA_PlayerState.h"
#include "SavableObjectInterface.h"
#include "LUTIAGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"



FPlayerSaveData* ULUTIA_SaveGame::GetPlayerData(APlayerState* PlayerState)
{
	if (PlayerState == nullptr)
	{
		return nullptr;
	}

	// Will not give unique ID while PIE so we skip that step while testing in editor.
	// UObjects don't have access to UWorld, so we grab it via PlayerState instead
	if (PlayerState->GetWorld()->IsPlayInEditor())
	{
		UE_LOG(LogTemp, Log, TEXT("During PIE we cannot use PlayerID to retrieve Saved Player data. Using first entry in array if available."));

		if (SavedPlayers.IsValidIndex(0))
		{
			return &SavedPlayers[0];
		}

		// No saved player data available
		return nullptr;
	}

	// Easiest way to deal with the different IDs is as FString (original Steam id is uint64)
	// Keep in mind that GetUniqueId() returns the online id, where GetUniqueID() is a function from UObject (very confusing...)
	FString PlayerID = PlayerState->GetUniqueId().ToString();
	// Iterate the array and match by PlayerID (eg. unique ID provided by Steam)
	return SavedPlayers.FindByPredicate([&](const FPlayerSaveData& Data) { return Data.PlayerID == PlayerID; });
}

void ULUTIA_SaveGame::HandleStartingNewPlayer(AController* NewPlayer)
{
	ALUTIA_PlayerState* PS = NewPlayer->GetPlayerState<ALUTIA_PlayerState>();
	if (PS != nullptr)
	{
		PS->LoadPlayerState(this);
		this->OverrideSpawnTransform(NewPlayer);
	}
}

bool ULUTIA_SaveGame::OverrideSpawnTransform(AController* NewPlayer)
{
	if (!IsValid(NewPlayer))
	{
		return false;
	}

	ALUTIA_PlayerState* PS = NewPlayer->GetPlayerState<ALUTIA_PlayerState>();
	if (PS == nullptr)
	{
		return false;
	}

	if (APawn* MyPawn = PS->GetPawn())
	{
		FPlayerSaveData* FoundData = this->GetPlayerData(PS);
		if (FoundData && FoundData->bResumeAtTransform)
		{
			MyPawn->SetActorLocation(FoundData->Location);
			MyPawn->SetActorRotation(FoundData->Rotation);

			// PlayerState owner is a (Player)Controller
			AController* PC = Cast<AController>(PS->GetOwner());
			// Set control rotation to change camera direction, setting Pawn rotation is not enough
			PC->SetControlRotation(FoundData->Rotation);

			return true;
		}
	}

	return false;
}

void ULUTIA_SaveGame::WriteSaveGame(UWorld* CurrentWorld)
{

	this->SavedPlayers.Empty();
	this->SavedActors.Empty();

	AGameStateBase* GS = CurrentWorld->GetGameState();
	if (GS == nullptr)
	{
		// Warn about failure to save?
		return;
	}

	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for (int32 i = 0; i < GS->PlayerArray.Num(); i++)
	{
		ALUTIA_PlayerState* PS = Cast<ALUTIA_PlayerState>(GS->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(this);
			break; // single player only at this point
		}
	}

	// Iterate the entire world of actors
	for (FActorIterator It(CurrentWorld); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our 'gameplay actors', skip actors that are being destroyed
		// Note: You might instead use a dedicated SavableObject interface for Actors you want to save instead of re-using GameplayInterface
		if (!IsValid(Actor) || !Actor->Implements<USavableObjectInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		ActorData.Transform = Actor->GetActorTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		this->SavedActors.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(this, SaveSlotName, SaveIndex);

	OnSaveGameWritten.Broadcast(this);
}

void ULUTIA_SaveGame::LoadSaveGame(UWorld* CurrentWorld)
{
	// Update slot name first if specified, otherwise keeps default name

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveIndex))
	{
		// Iterate the entire world of actors
		for (FActorIterator It(CurrentWorld); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our 'gameplay actors'
			if (!(Actor->Implements<USavableObjectInterface>()))
			{
				continue;
			}

			for (FActorSaveData ActorData : this->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					ISavableObjectInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}

		OnSaveGameLoaded.Broadcast(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}

void ULUTIA_SaveGame::LoadActorSave(AActor* CurrentActor)
{
	// Update slot name first if specified, otherwise keeps default name

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveIndex))
	{
		// Iterate the entire world of actors
		for (FActorSaveData ActorData : this->SavedActors)
		{
			if (ActorData.ActorName == CurrentActor->GetFName())
			{
				CurrentActor->SetActorTransform(ActorData.Transform);

				FMemoryReader MemReader(ActorData.ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				CurrentActor->Serialize(Ar);

				ISavableObjectInterface::Execute_OnActorLoaded(CurrentActor);

				break;
			}
		}
	}
}
