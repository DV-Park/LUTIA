// Fill out your copyright notice in the Description page of Project Settings.


#include "A_SkySystem.h"
#include "Components/BillboardComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SkyLightComponent.h"


// Sets default values
AA_SkySystem::AA_SkySystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	Sun->SetupAttachment(Root);

	HeightFogComp = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("HeightFogComp"));
	HeightFogComp->SetupAttachment(Root);

	SkyAtmosphereComp = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphereComp"));
	SkyAtmosphereComp->SetupAttachment(Root);

	VcloudComp = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VcloudComp"));
	VcloudComp->SetupAttachment(Root);

	SkyLightComp = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLightComp"));
	SkyLightComp->SetupAttachment(Root);
	SkyLightComp->bRealTimeCapture = true;

}

void AA_SkySystem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetSunRot(TimeOfDay);
}

// Called when the game starts or when spawned
void AA_SkySystem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AA_SkySystem::SetSunRot(float InTime)
{
	Sun->SetRelativeRotation(FRotator(FMath::GetMappedRangeValueUnclamped(FVector2D(600,1800),FVector2D(180,360),InTime),0,0));
}

// Called every frame
void AA_SkySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

