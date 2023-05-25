// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomDataTableStruct.h"
#include "UObject/ConstructorHelpers.h"
#include "AbilitySystemInterface.h"
#include "CharacterAttributeSetBase.h"
#include "CharacterAbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "LUTIA_PlayerState.generated.h"


USTRUCT(BlueprintType)
struct FSkillPresetImg
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (MultiLine = "true", UIMin = "0", UIMax = "10"))
		TArray<UTexture2D*> Skill;
};

USTRUCT(BlueprintType)
struct FSkillPreset
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (MultiLine = "true", UIMin = "0", UIMax = "10"))
		TArray<FSkillPresetImg> SkillImg;
};

/**
 * 
 */
class ULUTIA_SaveGame;

UCLASS()
class LUTIA_API ALUTIA_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite)
	int32 AcquiredSkillCount;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentSkillPreset;

	UPROPERTY(BlueprintReadOnly)
	float PersonalRecordTime;

	UPROPERTY(BlueprintReadWrite)
	TArray<bool> AcquiredSkills;

	UPROPERTY(BlueprintReadWrite)
	class UDataTable* SkillDataTable;

	UPROPERTY(BlueprintReadWrite)
	int32 MainQuestIndex;

	UPROPERTY(BlueprintReadWrite)
	bool IsMainQuestClear = true;

	UPROPERTY()
	class UCharacterAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UCharacterAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	virtual void LevelChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> CurrentUseSkills;

	UPROPERTY(BlueprintReadWrite)
		FSkillPreset SkillPreset;

	UFUNCTION(BlueprintNativeEvent)
		void SavePlayerState(ULUTIA_SaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
		void LoadPlayerState(ULUTIA_SaveGame* SaveObject);

	UFUNCTION(BlueprintCallable)
		void MainQuestClear(int32 Index);

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UCharacterAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "LUTIA|LUTIAPlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category= "LUTIA|LUTIAPlayerState")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category="LUTIA|LUTIAPlayerState")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "LUTIA|LUTIAPlayerState")
	int32 GetCharacterLevel() const;

	ALUTIA_PlayerState();
};
