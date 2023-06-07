// Copyright Epic Games, Inc. All Rights Reserved.

#include "LUTIAGameMode.h"
#include "LUTIACharacter.h"
#include "EngineUtils.h"
#include "Public/LUTIA_PlayerState.h"
#include "Public/LUTIA_SaveGame.h"
#include "Public/SavableObjectInterface.h"
#include "Public/LUTIAGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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

void ALUTIAGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (UGameplayStatics::DoesSaveGameExist("UserData", 0))
	{
		SaveGame = Cast<ULUTIA_SaveGame>(UGameplayStatics::LoadGameFromSlot("UserData", 0));
		SaveGame->LoadSaveGame(GetWorld());
	}
	else
		SaveGame = Cast<ULUTIA_SaveGame>(UGameplayStatics::CreateSaveGameObject(ULUTIA_SaveGame::StaticClass()));

}

void ALUTIAGameMode::WriteSaveGame()
{
	SaveGame->WriteSaveGame(GetWorld());
}

void ALUTIAGameMode::LoadCharacterInfo()
{
	AController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr) {
		SaveGame->HandleStartingNewPlayer(PC);
		SaveGame->LoadSaveGame(GetWorld());
	}

}

void ALUTIAGameMode::LoadActorData(AActor* CurrentActor)
{
	SaveGame->LoadActorSave(CurrentActor);
}

