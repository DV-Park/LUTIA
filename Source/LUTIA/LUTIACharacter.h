// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "LUTIA/LUTIA.h"
#include "Public/LUTIA_PlayerState.h"
#include "LUTIACharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ALUTIACharacter*, Character);

UCLASS(config=Game)
class ALUTIACharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ALUTIACharacter(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "LUTIA|Character")
	FCharacterDiedDelegate OnCharacterDied;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "stats")
	bool IsShiftPressed;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "stats")
	bool IsCommunicating = false;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera | Zoom")
	float MinZoomLength = 150.f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera | Zoom")
	float MaxZoomLength = 410.f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera | Zoom")
	float DefaultArmLength = 400.f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Camera | Zoom")
	float ZoomStep = 10.f;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float TurnRateGamepad;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Character")
	virtual int32 GetAbilityLevel(LUTIAAbilityID AbilityID) const;

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Character|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Character|Attributes")
	float GetHealth() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Camera")
	float GetStartingCameraBoomArmLength();

	UFUNCTION(BlueprintCallable, Category = "LUTIA|Camera")
	FVector GetStartingCameraBoomLocation();
	

protected:

	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LUTIA|Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LUTIA|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LUTIA|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LUTIA|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LUTIA|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();
	
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void SetHealth(float Health);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void CameraZoom(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void ShiftPressed();

	void ShiftReleased();

	void CommuPressed();

	void CommuReleased();

	void Jump();

	bool ASCInputBound = false;

	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	void InitializeStartingValues(ALUTIA_PlayerState* PS);

	void BindASCInput();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LUTIA|Camera")
	float StartingCameraBoomArmLength;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LUTIA|Camera")
	FVector StartingCameraBoomLocation;
};

