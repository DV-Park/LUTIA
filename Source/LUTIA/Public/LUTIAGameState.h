// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LUTIAGameState.generated.h"

/**
 * 
 */
UCLASS()
class LUTIA_API ALUTIAGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Time")
		float WorldTime;
};
