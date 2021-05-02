// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemInterface.h" 
#include "Components/GASAbilitySystemComponent.h"
#include "CartagraPlayerController.generated.h"

/**
 *
 */
UCLASS()
class CARTAGRA_API ACartagraPlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	ACartagraPlayerController();
	
	virtual void BeginPlay() override;

protected:

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	UGASAbilitySystemComponent* AbilitySystemComponent;

	class UCartagraAttributeSet* Stats;

	class UPlayerAttributeSet* Attributes;

public:
	
	virtual class UGASAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UCartagraAttributeSet* GetCartagraAttributeSet() const { return Stats; }
	
	UPlayerAttributeSet* GetPlayerAttributeSet() const { return Attributes; }
	
private:

	class APlayerCharacter* Player;

	//Move to DestLocation
	void MoveToMouseCursor(float DeltaTime);

	//While holding the mouse key keep updating the DestLocation
	void SetNewMoveDestination(const FVector Destination);

	void CameraZoomIn();
	void CameraZoomOut();

	///Initiate a specific action (attacking, selecting, interacting)
	void OnInitiateAction();
	///Depending on the action that was initated, complete the task
	void OnCompleteAction();

	//SHIFT key is pressed, so set bAlternateMode = true
	void AlternateModePressed();
	void AlternateModeReleased();
	
	bool bMoveToMouseCursor = 0;
	bool bAlternateMode = 0;

};
