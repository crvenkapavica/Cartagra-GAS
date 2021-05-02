// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Components/GASAbilitySystemComponent.h"
#include "CartagraCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginAttack, ACartagraCharacter*, Character);

USTRUCT()
struct FDefensiveStats
{
	GENERATED_BODY()


	FGameplayAttributeData Resilience;
	
	FGameplayAttributeData BlockChance;
	
	FGameplayAttributeData MeleeDodgeChance;
	
	FGameplayAttributeData RangedDodgeChance;
};

USTRUCT()
struct FOffensiveStats
{
	GENERATED_BODY()

	FGameplayAttributeData HitChance;
	
	FGameplayAttributeData CritChance;
	
	FGameplayAttributeData CritDamage;
};

USTRUCT()
struct FElementalResistance
{
	GENERATED_BODY()

	FGameplayAttributeData FireResistance;
	
	FGameplayAttributeData FrostResistance;
	
	FGameplayAttributeData LightningResistance;

	FElementalResistance() : FireResistance(0.f), FrostResistance(0.f), LightningResistance(0.f) { }
};

USTRUCT()
struct FToxicResistance
{
	GENERATED_BODY()

	FGameplayAttributeData AcidResistance;
	
	FGameplayAttributeData PoisonResistance;
	
	FGameplayAttributeData AlkalineResistance;

	FToxicResistance() : AcidResistance(0.f), PoisonResistance(0.f), AlkalineResistance(0.f) { }
};

USTRUCT()
struct FResistanceTypes
{
	GENERATED_BODY()

	FElementalResistance ElementalResistance;
	
	FToxicResistance ToxicResistance;

	FResistanceTypes() { }
};

USTRUCT()
struct FElementalDamage
{
	GENERATED_BODY()

	FGameplayAttributeData FireDamage;
	
	FGameplayAttributeData FrostDamage;
	
	FGameplayAttributeData LightningDamage;

	FElementalDamage() : FireDamage(0.f), FrostDamage(0.f), LightningDamage(0.f) { }
};

USTRUCT()
struct FToxicDamage
{
	GENERATED_BODY()

	FGameplayAttributeData AcidDamage;
	
	FGameplayAttributeData PoisonDamage;
	
	FGameplayAttributeData AlkalineDamage;

	FToxicDamage() : AcidDamage(0.f), PoisonDamage(0.f), AlkalineDamage(0.f) { }
};

USTRUCT()
struct FDamageTypes
{
	GENERATED_BODY()

	FElementalDamage ElementalDamage;
	FToxicDamage ToxicDamage;

	FDamageTypes() { }
};


USTRUCT()
struct FCharacterStats
{
	GENERATED_BODY()

	FGameplayAttributeData BaseDamage;

	FGameplayAttributeData AttackSpeed;

	FGameplayAttributeData AttackRange;

	FGameplayAttributeData BaseHealth;

	FGameplayAttributeData BaseArmor;

	FGameplayAttributeData MovementSpeed;

	//Percentage
	FGameplayAttributeData MagicFind;
		
	FDefensiveStats Defense;

	FOffensiveStats Offense;

	FResistanceTypes Resistances;

	FDamageTypes DamageTypes;

	FCharacterStats() : BaseDamage(3.f), AttackSpeed(1.f), AttackRange(50.f), BaseHealth(100), BaseArmor(1.f), MovementSpeed(200.f), MagicFind(0.f) { }

	//also implement a Resource (mana, rage...etc) typedef, enum, struct???
};


UCLASS()
class CARTAGRA_API ACartagraCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACartagraCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FVector AbilityLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PrimaryAttackProjectile_BP;

	FTimerHandle AttackSpeedTimerHandle;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBeginAttack OnBeginAttack;

	virtual class UGASAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:

	//Gameplay Ability SYSTEM
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UCartagraAttributeSet* Stats;


	virtual void InitializeAttributes();
	virtual void AddAbilities();
	virtual void AddEffects();

	float GetHealth() const;
	float GetMaxHealth() const;
	float GetAttackSpeed() const;
	float GetAttackRange() const;
	float GetArmor() const;
	float GetMoveSpeed() const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UGASGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Controller checks and decides whether to perform and attack, interaction etc..
	//If this is called we're already sure we want to perform some kind of attack / ability
	///Target is a location
	void InitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation);

	///Target is a Character
	void InitiateAction(ACartagraCharacter* Target);

	void CompleteAction();

private:

	void EndPrimaryAttack();

	void BeginPrimaryAttack(class ACartagraCharacter* Target);
	void BeginPrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator);

	/// This is a homing attack.
	void PrimaryAttack(class ACartagraCharacter* Target);

	/// This is a point attack.
	void PrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator);

	///Spawn homing projectiles and start interpolation.
	void SpawnProjectile(class ACartagraCharacter* Target);

	//Spawn projectile at point and start interpolation.
	void SpawnProjectile(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator);

	//Apply Calculated Damage After accounting for procs and resistances etc.. to the Target we were Calculating for
	void ApplyDamage(ACartagraCharacter* DamagedTarget, uint64 DamageToApply);

	//Calculate Damage; For now the Damage system is not implemented it is just a per Character BaseDamage
	void CalculateDamage(class ACartagraCharacter* TargetHit, UAttributeSet* InstigatorAttributeSet);

	//Play animations, resurrections.....
	void OnBeginDeath(class ACartagraCharacter* DyingTarget);

	//Start rolling rool, assing kill counts, and destroy
	void OnEndDeath(class ACartagraCharacter* KilledTarget);

	UFUNCTION()
	void OnTargetHit(class ACartagraCharacter* Target, class ACartagraCharacter* ActionInstigator);

	UFUNCTION()
	void OnHit();

	void AttackSpeedTimer();

	bool bMoving = false;
	bool bAttacking = false;
	bool bAlternateMode = false;
	//this bool checks whether we can attack yet, relative to out attack speed
	bool bDefaultAttackReady = true;
};

