// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomDataTableStruct.generated.h"

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
		FText SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
		UTexture2D* IconOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
		UTexture2D* IconOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
		FText SkillInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
		FText SkillTips;
};


/**
 * 
 */
class LUTIA_API CustomDataTableStruct
{
public:

};
