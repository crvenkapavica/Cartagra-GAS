// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CartagraCharacter.h"
#include "PlayerCharacter.generated.h"


/**
 * This class is Derived from ACartagraCharacter which is a base class for all characters
 * Inventory should be implemented here, as well as all effects specific to players
 */
UCLASS()
class CARTAGRA_API APlayerCharacter : public ACartagraCharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocaiton);

	void OnInitiateAction(ACartagraCharacter* Target);

	void OnCompleteAction();

protected:

	class UPlayerAttributeSet* Attributes;

	class USpringArmComponent* CameraBoom;

	class UCameraComponent* CameraComponent;

	class ACartagraPlayerController* Controller;

	virtual void PossessedBy(AController* NewController) override;

	void BindAbilityInput();

	bool bAbilityInputBound = false;

public:
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; }
};

