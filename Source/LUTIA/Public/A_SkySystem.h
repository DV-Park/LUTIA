// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_Weather_Preset.h"
#include "A_SkySystem.generated.h"


class UBillboardComponent;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;
class UVolumetricCloudComponent;
class USkyLightComponent;
class UDA_Weather_Preset;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class LUTIA_API AA_SkySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_SkySystem(const FObjectInitializer& ObjectInitializer);
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; //Replicated 사용 위해 필수!

	virtual void OnConstruction(const FTransform& Transform) override;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UBillboardComponent> Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UDirectionalLightComponent> Sun;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UExponentialHeightFogComponent> HeightFogComp;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComp;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UVolumetricCloudComponent> VcloudComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<USkyLightComponent> SkyLightComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (UIMin = "0", UIMax = "2400"))
	float TimeOfDay{ 610.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FGmWeatherSettings GMWeatherSettings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UDA_Weather_Preset> Weather;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Volumetric Clouds")
	UMaterialInterface* CurVCMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dynamic Material Instance", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> VCMIDynamic;

	UFUNCTION()
	void CreateVCMIDynamic();

	UFUNCTION()
	void SetWeaterSettings(FGmWeatherSettings InWeatherSetts);

	UFUNCTION()
	void SetVCSettings(FGmVolumetricSettings InVolumetricSettings);

	UFUNCTION()
	void UpdateSettings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetSunRot(float InTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void SetCompSettings();

};
