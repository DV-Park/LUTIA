// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommuActor.generated.h"

UCLASS()
class LUTIA_API ACommuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommuActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Communicate")
		void GetCommu();
	virtual void GetCommu_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Communicate")
		void Select1();
	virtual void Select1_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Communicate")
		void Select2();
	virtual void Select2_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Communicate")
		void Select3();
	virtual void Select3_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Communicate")
		void Select4();
	virtual void Select4_Implementation();
};
