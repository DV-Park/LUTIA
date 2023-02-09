// Fill out your copyright notice in the Description page of Project Settings.


#include "A_SkySystem.h"
#include "Components/BillboardComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"


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

}

// Called when the game starts or when spawned
void AA_SkySystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_SkySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

