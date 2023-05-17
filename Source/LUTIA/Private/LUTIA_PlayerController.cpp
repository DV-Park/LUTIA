// Fill out your copyright notice in the Description page of Project Settings.


#include "LUTIA_PlayerController.h"
#include "LUTIA_PlayerState.h"
#include "AbilitySystemComponent.h"

void ALUTIA_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ALUTIA_PlayerState* PS = GetPlayerState<ALUTIA_PlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}
