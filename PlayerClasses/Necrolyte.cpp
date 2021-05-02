// Fill out your copyright notice in the Description page of Project Settings.


#include "../PlayerClasses/Necrolyte.h"

// Sets default values
ANecrolyte::ANecrolyte()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//FCharacterAttributes* NecrolyteAttributes = &CharacterAttributes;
	//FCharacterAttributes->Agility = FAgility(1, 2, 3);

}

void ANecrolyte::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANecrolyte::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

