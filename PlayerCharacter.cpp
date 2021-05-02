#include "PlayerCharacter.h"
#include "Cartagra.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CartagraPlayerController.h"
#include "PlayerAttributeSet.h"
#include "Components/GASAbilitySystemComponent.h"

APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 300.f;
	CameraBoom->CameraLagMaxDistance = 800.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocaiton)
{
	InitiateAction(ActionInstigator, HitLocaiton);
}

void APlayerCharacter::OnInitiateAction(ACartagraCharacter* Target)
{
	InitiateAction(Target);
}

void APlayerCharacter::OnCompleteAction()
{
	CompleteAction();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	Controller = Cast<ACartagraPlayerController>(NewController);

	AbilitySystemComponent = Controller->GetAbilitySystemComponent();

	//Initialize GSC; Owner is Controller; Avatar is Player
	AbilitySystemComponent->InitAbilityActorInfo(Controller, this);

	//Get Player Character AttributeSets from its GAS Owner - PlayerController
	Stats = Controller->GetCartagraAttributeSet();
	Attributes = Controller->GetPlayerAttributeSet();
	
	InitializeAttributes();
	AddAbilities();
	AddEffects();

	BindAbilityInput();
}

void APlayerCharacter::BindAbilityInput()
{
	if (!bAbilityInputBound && AbilitySystemComponent && IsValid(Controller->InputComponent))
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(Controller->InputComponent, Binds);

		bAbilityInputBound = true;
	}
}