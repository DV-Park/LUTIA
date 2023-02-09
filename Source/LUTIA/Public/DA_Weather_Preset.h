// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Weather_Preset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGmVolumetricSettings
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults",meta=(MultiLine="true",UIMin="0",UIMax="10"))
	double CloudCoverage{ 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults",meta=(MultiLine="true",UIMin="0",UIMax="10"))
	double Rain{ 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults",meta=(MultiLine="true",UIMin="0",UIMax="10"))
	double Snow{0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults",meta=(MultiLine="true",UIMin="0",UIMax="10"))
	double Dust{ 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults",meta=(MultiLine="true",UIMin="0",UIMax="10"))
	double Fog{ 0 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Defaults",meta=(MultiLine="true",UIMin="0",UIMax="10"))
	double WindIntensity{ 0 };

};

USTRUCT(BlueprintType)
struct FGmWeatherSettings 
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VolumetricCloud", meta = (ShowOnlyInnerProperties))
	FGmVolumetricSettings GmVCSettings;

};

UCLASS()
class LUTIA_API UDA_Weather_Preset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Volumetric Cloud", meta = (ShowOnlyInnerProperties))
	FGmVolumetricSettings GmVolumetricSettings;

	UFUNCTION()
	FGmWeatherSettings GetWeatherSettings();
	
};
