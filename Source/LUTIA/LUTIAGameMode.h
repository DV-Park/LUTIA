// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/LUTIA_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "LUTIAGameMode.generated.h"


UCLASS(minimalapi)
class ALUTIAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALUTIAGameMode();

	UFUNCTION(BlueprintCallable, Category = "SaveData")
	void Save();

	UFUNCTION(BlueprintCallable, Category = "SaveData")
	void Load();
};



