// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/LUTIAGameState.h"
#include "Public/LUTIA_PlayerState.h"
#include "Public/LUTIA_SaveGame.h"

#include "LUTIAGameMode.generated.h"


UCLASS(minimalapi)
class ALUTIAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALUTIAGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveData")
		ULUTIA_SaveGame* SaveGame;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};



