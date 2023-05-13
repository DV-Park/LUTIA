// Fill out your copyright notice in the Description page of Project Settings.


#include "LUTIAAssetManager.h"
#include "AbilitySystemGlobals.h"

void ULUTIAAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}