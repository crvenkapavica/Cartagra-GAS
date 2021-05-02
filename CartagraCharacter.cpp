#include "CartagraCharacter.h"
#include "Cartagra.h"
#include "PlayerCharacter.h"
#include "Defiler.h"
#include "CartagraPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/InterpolationComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/GASAbilitySystemComponent.h"
#include "CartagraAttributeSet.h"
#include "GASGameplayAbility.h"
#include "GameplayEffectTypes.h"

ACartagraCharacter::ACartagraCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(33.f, 88.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->MaxWalkSpeed = 482.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 482.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

void ACartagraCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void ACartagraCharacter::AddAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ACartagraCharacter::AddEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		/*TDODO MAKE LEVELS*/ const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

UGASAbilitySystemComponent* ACartagraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACartagraCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void ACartagraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACartagraCharacter::InitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation)
{
	FVector Destination = (HitLocation - ActionInstigator->GetActorLocation()) * GetAttackRange();
	FRotator Rotation = (Destination - ActionInstigator->GetActorLocation()).Rotation();
	BeginPrimaryAttack(Destination, Rotation, ActionInstigator);
}

void ACartagraCharacter::InitiateAction(ACartagraCharacter* Target)
{
	BeginPrimaryAttack(Target);
}

void ACartagraCharacter::CompleteAction()
{
	bMoving = false;
}

void ACartagraCharacter::EndPrimaryAttack()
{

}

void ACartagraCharacter::BeginPrimaryAttack(ACartagraCharacter* Target)
{
	if (bDefaultAttackReady == false) return;
	bDefaultAttackReady = false;

	GetWorldTimerManager().SetTimer(AttackSpeedTimerHandle, this, &ACartagraCharacter::AttackSpeedTimer, GetAttackSpeed(), false, GetAttackSpeed());
	bAttacking = true;

	PrimaryAttack(Target);

	bAttacking = false;
	bMoving = false;
}

void ACartagraCharacter::BeginPrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator)
{
	if (bDefaultAttackReady == false) return;
	bDefaultAttackReady = false;

	GetWorldTimerManager().SetTimer(AttackSpeedTimerHandle, this, &ACartagraCharacter::AttackSpeedTimer, GetAttackSpeed(), false, GetAttackSpeed());
	bAttacking = true;

	PrimaryAttack(Location, Rotation, ActionInstigator);
	

	bAttacking = false;
	bMoving = false;
}

void ACartagraCharacter::PrimaryAttack(ACartagraCharacter* Target)
{
	//OnBeginAttack.Broadcast(this);
	SpawnProjectile(Target);
}

void ACartagraCharacter::PrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator)
{
	SpawnProjectile(Location, Rotation, ActionInstigator);
}

void ACartagraCharacter::SpawnProjectile(ACartagraCharacter* Target)
{
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile_BP, GetActorLocation(), (Target->GetActorLocation() - GetActorLocation()).Rotation());
	
	UInterpolationComponent* InterpComp = Cast<UInterpolationComponent>(Projectile->GetComponentByClass(UInterpolationComponent::StaticClass()));
	InterpComp->InterpSpeed = 1500.f;
	InterpComp->bHoming = true;
	InterpComp->HomingTarget = Target;
	InterpComp->bIsDestroyable = true;
	InterpComp->Instigator = this;
	InterpComp->OnHit.BindUFunction(this, FName("OnHit"));
	InterpComp->OnTargetHit.BindUObject(this, &ACartagraCharacter::OnTargetHit);
}

void ACartagraCharacter::SpawnProjectile(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator)
{
	Rotation.Pitch = 0.f;
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile_BP, ActionInstigator->GetActorLocation(), Rotation);

	UInterpolationComponent* InterpComp = Cast<UInterpolationComponent>(Projectile->GetComponentByClass(UInterpolationComponent::StaticClass()));
	InterpComp->InterpSpeed = 1200.f;
	InterpComp->bHoming = false;
	InterpComp->StaticTarget = Location;
	InterpComp->bIsDestroyable = true;
	InterpComp->Instigator = ActionInstigator;
	InterpComp->OnHit.BindUFunction(this, FName("OnHit"));
	InterpComp->OnTargetHit.BindUObject(this, &ACartagraCharacter::OnTargetHit);
}

void ACartagraCharacter::CalculateDamage(ACartagraCharacter* TargetHit, UAttributeSet* InstigatorAttributeSet)
{
	const float Damage = Cast<UCartagraAttributeSet>(InstigatorAttributeSet)->GetDamage();

	ApplyDamage(TargetHit, Damage);
}

void ACartagraCharacter::ApplyDamage(ACartagraCharacter* DamagedTarget, uint64 DamageToApply)
{
	const float NewHealth = DamagedTarget->GetHealth() - DamageToApply;
	DamagedTarget->Stats->SetHealth(NewHealth);

	if (NewHealth <= 0)
	{
		OnBeginDeath(DamagedTarget);
	}
}

void ACartagraCharacter::OnBeginDeath(ACartagraCharacter* DyingTarget)
{
	//Check for possible resurrection and play cues
	//if the TargetReallyDied Kill the Target
	
	OnEndDeath(DyingTarget);
}

void ACartagraCharacter::OnEndDeath(ACartagraCharacter* KilledTarget)
{
	//Broadcast delegates here and roll loot

	KilledTarget->Destroy();
}

void ACartagraCharacter::OnTargetHit(ACartagraCharacter* Target,  ACartagraCharacter* ActionInstigator)
{
	//call CalculateDamage
	//perhaps add other Dynamic Delegates for procs..

	CalculateDamage(Target, ActionInstigator->Stats);
}


void ACartagraCharacter::OnHit()
{
	//Placeholder for ground cues
}

void ACartagraCharacter::AttackSpeedTimer()
{
	bDefaultAttackReady = true;
}

float ACartagraCharacter::GetHealth() const
{
	if (IsValid(Stats))
	{
		return Stats->GetHealth();
	}

	return 1.f;
}

float ACartagraCharacter::GetMaxHealth() const
{
	if (IsValid(Stats))
	{
		return Stats->GetMaxHealth();
	}

	return 1.f;
}

float ACartagraCharacter::GetAttackSpeed() const
{
	if (IsValid(Stats))
	{
		return Stats->GetAttackSpeed();
	}

	return 1.f;
}

float ACartagraCharacter::GetAttackRange() const
{
	if (IsValid(Stats))
	{
		return Stats->GetAttackRange();
	}

	return 1.f;
}

float ACartagraCharacter::GetArmor() const
{
	if (IsValid(Stats))
	{
		return Stats->GetArmor();
	}

	return 1.f;
}

float ACartagraCharacter::GetMoveSpeed() const
{
	if (IsValid(Stats))
	{
		return Stats->GetMoveSpeed();
	}

	return 1.f;
}