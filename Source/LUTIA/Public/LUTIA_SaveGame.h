// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LUTIA_SaveGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class ULUTIA_SaveGame*, SaveObject);

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	/* Identifier for which Actor this belongs to */
	UPROPERTY()
		FName ActorName;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY()
		FTransform Transform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
		TArray<uint8> ByteData;
};

USTRUCT(BlueprintType)
struct FSkillPresetImgData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (MultiLine = "true", UIMin = "0", UIMax = "10"))
		TArray<int32> SkillIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (MultiLine = "true", UIMin = "0", UIMax = "10"))
		TArray<UTexture2D*> Skill;
};

USTRUCT(BlueprintType)
struct FSkillPresetData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (MultiLine = "true", UIMin = "0", UIMax = "10"))
		TArray<FSkillPresetImgData> SkillImg;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	/* Player Id defined by the online sub system (such as Steam) converted to FString for simplicity  */
	UPROPERTY()
		FString PlayerID;

	UPROPERTY()
		int32 AcquiredSkillCount;

	UPROPERTY()
		int32 CurrentSkillPreset;

	UPROPERTY()
		TArray<bool> AcquiredSkills;

	UPROPERTY()
		TArray<bool> CurrentUseSkills;

	UPROPERTY()
		FSkillPresetData SkillPreset;

	UPROPERTY()
		int32 MainQuestIndex;

	UPROPERTY()
		bool IsMainQuestClear;

	/* Longest survival time */
	UPROPERTY()
		float PersonalRecordTime;

	/* Location if player was alive during save */
	UPROPERTY()
		FVector Location;

	/* Orientation if player was alive during save */
	UPROPERTY()
		FRotator Rotation;

	/* We don't always want to restore location, and may just resume player at specific respawn point in world. */
	UPROPERTY()
		bool bResumeAtTransform;
};
/**
 * 
 */
UCLASS()
class LUTIA_API ULUTIA_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;

	/* Actors stored from a level (currently does not support a specific level and just assumes the demo map) */
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);

	void HandleStartingNewPlayer(AController* NewPlayer);

	UFUNCTION(BlueprintCallable)
		bool OverrideSpawnTransform(AController* NewPlayer);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void WriteSaveGame(UWorld* CurrentWorld);
	
	void LoadSaveGame(UWorld* CurrentWorld);

	void LoadActorSave(AActor* CurrentActor);

	UPROPERTY(BlueprintAssignable)
		FOnSaveGameSignature OnSaveGameLoaded;

	UPROPERTY(BlueprintAssignable)
		FOnSaveGameSignature OnSaveGameWritten;


protected:
	UPROPERTY()
		TObjectPtr<ULUTIA_SaveGame> CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FString SaveSlotName = "UserData";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveIndex = 0;
};
