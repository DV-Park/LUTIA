// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LUTIA_PlayerState.generated.h"


/**
 * 
 */
class ULUTIA_SaveGame;

UCLASS()
class LUTIA_API ALUTIA_PlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
		int32 Credits;

	UPROPERTY(BlueprintReadOnly)
		float PersonalRecordTime;
	
public:
	UFUNCTION(BlueprintNativeEvent)
		void SavePlayerState(ULUTIA_SaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
		void LoadPlayerState(ULUTIA_SaveGame* SaveObject);



};
