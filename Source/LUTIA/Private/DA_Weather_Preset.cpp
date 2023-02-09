// Fill out your copyright notice in the Description page of Project Settings.


#include "DA_Weather_Preset.h"

FGmWeatherSettings UDA_Weather_Preset::GetWeatherSettings()
{
	FGmWeatherSettings GmWS;

	GmWS.GmVCSettings = GmVolumetricSettings;

	return GmWS;
}
