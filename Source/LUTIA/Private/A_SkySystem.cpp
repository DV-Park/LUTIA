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
AA_SkySystem::AA_SkySystem()
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

	if(Weather)
		UpdateSettings(Weather);
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
	SetWindSettings(InWeatherSetts.GmWindSettings);
}

void AA_SkySystem::SetVCSettings(FGmVolumetricSettings InVolumetricSettings)
{
	VcloudComp->SetVisibility(true, false);

	if (VcloudComp && VCMIDynamic)
	{
		GMWeatherSettings.GmVCSettings = InVolumetricSettings;

		VCMIDynamic->SetScalarParameterValue("CloudDensity", GMWeatherSettings.GmVCSettings.CloudCoverage);
		VCMIDynamic->SetScalarParameterValue("CloudDetail", GMWeatherSettings.GmVCSettings.CloudDetail);
	}
}

void AA_SkySystem::SetWindSettings(FGmWindSettings InGmWindSettings)
{
	if (VcloudComp && VCMIDynamic)
	{
		GMWeatherSettings.GmWindSettings = InGmWindSettings;
	}
}

void AA_SkySystem::SetVCWindOffset(float DeltaTime)
{
	if (VcloudComp)
	{
		float CurWindOffset = DeltaTime * (GMWeatherSettings.GmWindSettings.Cloud01WindOffset / 100);
		Cloud01WindOffset += FVector(CurWindOffset, CurWindOffset, 0);
	}

	if (VCMIDynamic)
	{
		VCMIDynamic->SetVectorParameterValue("Cloud01WindOffset", Cloud01WindOffset);
	}
}

void AA_SkySystem::UpdateSettings(UDA_Weather_Preset* w)
{
	Weather = w;
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

	SetVCWindOffset(DeltaTime);

}

void AA_SkySystem::SetCompSettings()
{
	CreateVCMIDynamic();

	if (VCMIDynamic) 
	{
		VcloudComp->SetMaterial(VCMIDynamic);
	}
}

