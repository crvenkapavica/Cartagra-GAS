// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Necrolyte.generated.h"

UCLASS()
class CARTAGRA_API ANecrolyte : public AActor
{
	GENERATED_BODY()
	
public:	

	ANecrolyte();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	//FCharacterAttributes CharacterAttributes;
};
