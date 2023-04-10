// Copyright Epic Games, Inc. All Rights Reserved.

#include "LUTIAGameMode.h"
#include "LUTIACharacter.h"
#include "UObject/ConstructorHelpers.h"


ALUTIAGameMode::ALUTIAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALUTIAGameMode::Save()
{
	ULUTIA_SaveGame* SaveGameInstance 
		= Cast<ULUTIA_SaveGame>(UGameplayStatics::CreateSaveGameObject(ULUTIA_SaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		SaveGameInstance->SaveSlotName = "Save1";
		SaveGameInstance->SaveIndex = 0;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr"));
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->SaveIndex);
}

void ALUTIAGameMode::Load()
{
	ULUTIA_SaveGame* LoadGameInstance = Cast<ULUTIA_SaveGame>(UGameplayStatics::CreateSaveGameObject(ULUTIA_SaveGame::StaticClass()));

	if (LoadGameInstance)
	{
		LoadGameInstance->SaveSlotName = "Save1";
		LoadGameInstance->SaveIndex = 0;

		LoadGameInstance = Cast<ULUTIA_SaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->SaveIndex));

		UE_LOG(LogTemp, Warning, TEXT("Load"));
	}
}
