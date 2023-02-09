// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_SkySystem.generated.h"

class UBillboardComponent;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;
class UVolumetricCloudComponent;
class USkyLightComponent;

UCLASS()
class LUTIA_API AA_SkySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_SkySystem();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetSunRot(float InTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
