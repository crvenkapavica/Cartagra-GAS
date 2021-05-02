// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Cartagra.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CARTAGRA_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGASGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
	
};
