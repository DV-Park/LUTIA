// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_SkySystem.generated.h"

class UBillboardComponent;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;

UCLASS()
class LUTIA_API AA_SkySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_SkySystem();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default");
	TObjectPtr<UBillboardComponent> Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components");
	TObjectPtr<UDirectionalLightComponent> Sun;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components");
	TObjectPtr<UExponentialHeightFogComponent> HeightFogComp;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components");
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
