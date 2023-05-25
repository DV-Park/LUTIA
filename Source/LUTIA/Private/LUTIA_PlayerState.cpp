// Fill out your copyright notice in the Description page of Project Settings.


#include "LUTIA_PlayerState.h"
#include "LUTIA_SaveGame.h"
#include "Net/UnrealNetwork.h"
#include "CharacterAttributeSetBase.h"



int32 ALUTIA_PlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetLevel();
}

ALUTIA_PlayerState::ALUTIA_PlayerState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> SkillDataObj(TEXT("/Game/DataBase/SkillData"));
	if (SkillDataObj.Succeeded())
	{
		SkillDataTable = SkillDataObj.Object;
		AcquiredSkills.Init(0, SkillDataTable->GetRowNames().Num());
		CurrentUseSkills.Init(0, SkillDataTable->GetRowNames().Num());
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultSkillImg(TEXT("/Game/UMG/UMG_Img/Skill/Default.Default"));
	if (DefaultSkillImg.Succeeded())
	{
		FSkillPresetImg img;
		img.Skill.Init(DefaultSkillImg.Object, 4);
		SkillPreset.SkillImg.Init(img, 4);
	}

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void ALUTIA_PlayerState::SavePlayerState_Implementation(ULUTIA_SaveGame* SaveObject)
{
	if (SaveObject)
	{
		// Gather all relevant data for player
		FPlayerSaveData SaveData;
		FSkillPresetImgData ImgData;

		SaveData.PersonalRecordTime = PersonalRecordTime;

		SaveData.AcquiredSkillCount = AcquiredSkillCount;

		SaveData.AcquiredSkills.Empty();
		SaveData.AcquiredSkills.Append(AcquiredSkills);

		SaveData.CurrentSkillPreset = CurrentSkillPreset;

		SaveData.SkillPreset.SkillImg.Empty();

		SaveData.MainQuestIndex = MainQuestIndex;

		SaveData.IsMainQuestClear = IsMainQuestClear;

		for (int i = 0; i < 4; i++) {
			ImgData.Skill.Empty();
			for (int j = 0; j < 4; j++)
				ImgData.Skill.Emplace(SkillPreset.SkillImg[i].Skill[j]);
			SaveData.SkillPreset.SkillImg.Emplace(ImgData);
		}

		SaveData.CurrentUseSkills.Empty();
		SaveData.CurrentUseSkills.Append(CurrentUseSkills);
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
		FSkillPresetImg ImgData;
		if (FoundData)
		{
			//Credits = SaveObject->Credits;
			// Makes sure we trigger credits changed event
			AcquiredSkillCount = FoundData->AcquiredSkillCount;

			MainQuestIndex = FoundData->MainQuestIndex;
			IsMainQuestClear = FoundData->IsMainQuestClear;

			AcquiredSkills.Empty();
			AcquiredSkills.Append(FoundData->AcquiredSkills);

			CurrentUseSkills.Empty();
			CurrentUseSkills.Append(FoundData->CurrentUseSkills);

			CurrentSkillPreset = FoundData->CurrentSkillPreset;

			SkillPreset.SkillImg.Empty();

			for (int i = 0; i < 4; i++) {
				ImgData.Skill.Empty();
				for (int j = 0; j < 4; j++)
					ImgData.Skill.Emplace(FoundData->SkillPreset.SkillImg[i].Skill[j]);
				SkillPreset.SkillImg.Emplace(ImgData);
			}

			PersonalRecordTime = FoundData->PersonalRecordTime;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find SaveGame data for player id '%i'."), GetPlayerId());
		}
	}
}

void ALUTIA_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ALUTIA_PlayerState::HealthChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ALUTIA_PlayerState::StunTagChanged);
	}

}

void ALUTIA_PlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed!"));
}

void ALUTIA_PlayerState::LevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Level Changed!"));
}

void ALUTIA_PlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}

void ALUTIA_PlayerState::MainQuestClear(int32 Index)
{
	if (Index == MainQuestIndex)
	{
		IsMainQuestClear = true;
		MainQuestIndex++;
	}
}

UAbilitySystemComponent* ALUTIA_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSetBase* ALUTIA_PlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ALUTIA_PlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ALUTIA_PlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	//TODO -- Implement HUD Later
}

float ALUTIA_PlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}
