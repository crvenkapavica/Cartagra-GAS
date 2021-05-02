#include "Defiler.h"
#include "CartagraAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CartagraItem.h"
#include "PlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CartagraAttributeSet.h"

ADefiler::ADefiler()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySysComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Stats = CreateDefaultSubobject<UCartagraAttributeSet>(TEXT("Stats"));

	Stats->MoveSpeed = 225.f;
	Stats->AttackRange = 520.f;
	Stats->Damage = 20.f;
	Stats->Health = 100.f;
}

// Called when the game starts or when spawned
void ADefiler::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	StartLocation = GetActorLocation();

	MoveComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	MoveComp->MaxWalkSpeed = GetMoveSpeed();

	GetWorldTimerManager().SetTimer(MoveIdleTimerHandle, this, &ADefiler::Move_Idle, 2.2f, true, 0.3f);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitializeAttributes();
	AddAbilities();
	AddEffects();
}

// Called every frame
void ADefiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (bMovingToStartLocation)
	{
		CheckIsAtStartLocation();
	}
	else if (CheckIsPlayerInAggroRange())
	{
		if (CheckIsPlayerInAttackRange())
		{
			FaceLocation(Player->GetActorLocation());
			OnInitiateAction(this, Player->GetActorLocation());
		}
		else if (CheckIsPlayerInChaseRange())
		{
			ChasePlayer();
		}
	}*/


	if (!bPlayerHasAggro)
	{
		CheckIsPlayerInAggroRange();
	}
	else if (CheckIsPlayerInChaseRange())
	{
		//attack


		ChasePlayer();
	} 
	//else Chase
}

void ADefiler::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


}

void ADefiler::OnCompleteAction()
{
	CompleteAction();
}

void ADefiler::CheckIsPlayerInAggroRange()
{
	if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) <= AGGRO_RANGE)
	{
		bPlayerHasAggro = true;
		GetWorldTimerManager().ClearTimer(MoveIdleTimerHandle);
	}
}

bool ADefiler::CheckIsPlayerInAttackRange()
{
	if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) <= GetAttackRange())
	{
		return true;
	}
	return false;
}

bool ADefiler::CheckIsPlayerInChaseRange()
{
	if (FVector::Dist(GetActorLocation(), StartLocation) > AGGRO_FOLLOW_RANGE)
	{
		bPlayerHasAggro = false;
		bMovingToStartLocation = true;
		MoveToLocation(StartLocation);
		GetWorldTimerManager().SetTimer(MoveIdleTimerHandle, this, &ADefiler::Move_Idle, 2.2f, true, 0.3f);
		return false;
	}
	return true;
}

void ADefiler::CheckIsAtStartLocation()
{
	if (FVector::Dist(GetActorLocation(), StartLocation) < 50.f)
	{
		bMovingToStartLocation = false;
	}
}

void ADefiler::ChasePlayer()
{
	//face player
	FaceLocation(Player->GetActorLocation());

	//if out of ranged attack move closer
	if (!CheckIsPlayerInAttackRange())
	{
		MoveToLocation(Player->GetActorLocation());
	}

	OnInitiateAction(this, Player->GetActorLocation());
}

void ADefiler::FaceLocation(FVector Location)
{
	FRotator LookAtRotation = ((Location - GetActorLocation()).Rotation());
	LookAtRotation.Pitch = 0.f;

	SetActorRotation(LookAtRotation);
}

void ADefiler::Move_Idle()
{
	float X = GetActorLocation().X + FMath::RandRange(-162, 162);
	float Y = GetActorLocation().Y + FMath::RandRange(-162, 162);

	const FVector Location{ X, Y, 1.f };

	if (FVector::Dist(Location, StartLocation) > 162) return;

	FaceLocation(Location);
	MoveToLocation(Location);
}

void ADefiler::MoveToLocation(FVector DestLocation)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLocation);
}

void ADefiler::OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation)
{
	InitiateAction(ActionInstigator, HitLocation);
}

void ADefiler::OnInitiateAction(ACartagraCharacter* Target)
{
	InitiateAction(Target);
}

UCartagraItem* ADefiler::RollLoot()
{
	return nullptr;
}