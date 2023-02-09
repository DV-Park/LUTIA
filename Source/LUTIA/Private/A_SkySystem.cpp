// Fill out your copyright notice in the Description page of Project Settings.


#include "A_SkySystem.h"
#include "Components/BillboardComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SkyLightComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AA_SkySystem::AA_SkySystem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UMaterialInterface> VCMaterials;

		FConstructorStatics()
			:VCMaterials(TEXT("MaterialInterface'/Game/Materials/M_VolumetricCloud_Inst'"))
		{

		}
	};

	static FConstructorStatics ConstructorStatics;

	CurVCMaterial = ConstructorStatics.VCMaterials.Object;

	bReplicates = true;

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

void AA_SkySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AA_SkySystem, GMWeatherSettings);
}

void AA_SkySystem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetSunRot(TimeOfDay);

	UpdateSettings();
}

void AA_SkySystem::CreateVCMIDynamic()
{
	if (CurVCMaterial)
	{
		if (!VCMIDynamic || (VCMIDynamic->Parent != CurVCMaterial))
		{
			VCMIDynamic = UMaterialInstanceDynamic::Create(CurVCMaterial, this);
		}
	}
}

void AA_SkySystem::SetWeaterSettings(FGmWeatherSettings InWeatherSetts)
{
	SetVCSettings(InWeatherSetts.GmVCSettings);
}

void AA_SkySystem::SetVCSettings(FGmVolumetricSettings InVolumetricSettings)
{
	VcloudComp->SetVisibility(true, false);

	if (VcloudComp && VCMIDynamic)
	{
		GMWeatherSettings.GmVCSettings = InVolumetricSettings;

		VCMIDynamic->SetScalarParameterValue("CloudDensity", GMWeatherSettings.GmVCSettings.CloudCoverage);
	}
}

void AA_SkySystem::UpdateSettings()
{
	SetCompSettings();

	SetWeaterSettings(Weather->GetWeatherSettings());
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

void AA_SkySystem::SetCompSettings()
{
	CreateVCMIDynamic();

	if (VCMIDynamic) 
	{
		VcloudComp->SetMaterial(VCMIDynamic);
	}
}

