#include "../Components/InterpolationComponent.h"
#include "../CartagraCharacter.h"

UInterpolationComponent::UInterpolationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HomingTarget = nullptr;
	InterpSpeed = 200.f;
}


void UInterpolationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInterpolationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckOverlap();
	//Homing
	if (bHoming)
	{
		Interpolate(GetOwner()->GetActorLocation(), HomingTarget->GetActorLocation(), DeltaTime);
	}
	//Point
	else
	{
		Interpolate(GetOwner()->GetActorLocation(), StaticTarget, DeltaTime);
	}
}

void UInterpolationComponent::Interpolate(FVector StartLocation, FVector TargetLocation, float DeltaTime)
{
	if (GetOwner())
	{
		FVector CurrentLocation = FMath::VInterpConstantTo(GetOwner()->GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);		
		GetOwner()->SetActorLocation(CurrentLocation);
	}
}

void UInterpolationComponent::CheckOverlap() const
{
	if (bHoming && HomingTarget)
	{
		if (FMath::IsNearlyZero(FVector::Dist(GetOwner()->GetActorLocation(), HomingTarget->GetActorLocation())))
		{
			//We hit a Character with a homing attack
			OnTargetHit.ExecuteIfBound(HomingTarget, Instigator);

			GetOwner()->Destroy();
		}
	}
	else if (!bHoming)
	{
		
		TArray<AActor*> ActorsOverlapped;
		GetOwner()->GetOverlappingActors(ActorsOverlapped, ACartagraCharacter::StaticClass());

		if (ActorsOverlapped.Num())
		{
			for (AActor* Actor : ActorsOverlapped)
			{
				if (Actor->GetClass() != Instigator->GetClass())
				{
					//We hit a Character
					OnTargetHit.ExecuteIfBound(Cast<ACartagraCharacter>(Actor), Instigator);

					GetOwner()->Destroy();
				}
			}
		}
		else if (FMath::IsNearlyZero(FVector::Dist(GetOwner()->GetActorLocation(), StaticTarget)))
		{
			//We hit the ground
			OnHit.ExecuteIfBound();

			GetOwner()->Destroy();
		}
	}
}
