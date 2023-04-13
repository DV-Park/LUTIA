// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomDataTableStruct.h"
#include "UObject/ConstructorHelpers.h"
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
	UPROPERTY(BlueprintReadWrite)
		int32 AcquiredSkillCount;

	UPROPERTY(BlueprintReadOnly)
		float PersonalRecordTime;

	UPROPERTY(BlueprintReadWrite)
		TArray<bool> AcquiredSkills;

	UPROPERTY(BlueprintReadWrite)
		class UDataTable* SkillDataTable;
	


public:
	UFUNCTION(BlueprintNativeEvent)
		void SavePlayerState(ULUTIA_SaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
		void LoadPlayerState(ULUTIA_SaveGame* SaveObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> CurrentUseSkills;

	ALUTIA_PlayerState();
};
