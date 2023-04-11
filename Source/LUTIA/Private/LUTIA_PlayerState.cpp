// Fill out your copyright notice in the Description page of Project Settings.


#include "LUTIA_PlayerState.h"
#include "LUTIA_SaveGame.h"
#include "Net/UnrealNetwork.h"




void ALUTIA_PlayerState::SavePlayerState_Implementation(ULUTIA_SaveGame* SaveObject)
{
	if (SaveObject)
	{
		// Gather all relevant data for player
		FPlayerSaveData SaveData;
		SaveData.PersonalRecordTime = PersonalRecordTime;
		// Stored as FString for simplicity (original Steam ID is uint64)
		SaveData.PlayerID = GetUniqueId().ToString();

		// May not be alive while we save
		if (APawn* MyPawn = GetPawn())
		{
			SaveData.Location = MyPawn->GetActorLocation();
			SaveData.Rotation = MyPawn->GetActorRotation();
			SaveData.bResumeAtTransform = true;
		}

		SaveObject->SavedPlayers.Add(SaveData);
	}
}

void ALUTIA_PlayerState::LoadPlayerState_Implementation(ULUTIA_SaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData* FoundData = SaveObject->GetPlayerData(this);
		if (FoundData)
		{
			//Credits = SaveObject->Credits;
			// Makes sure we trigger credits changed event

			PersonalRecordTime = FoundData->PersonalRecordTime;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find SaveGame data for player id '%i'."), GetPlayerId());
		}
	}
}
