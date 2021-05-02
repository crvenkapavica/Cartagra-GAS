// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class CARTAGRA_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UPlayerAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//PRIMARY ATTRIBUTES
	//***************************************************************************************
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Spirituality)
	FGameplayAttributeData Spirituality;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Spirituality)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Agility)
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Agility)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Intelligence)
	//***************************************************************************************	

	//SECONDARY ATTRIBUTES
	//***************************************************************************************
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Recovery)
	FGameplayAttributeData Recovery;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Recovery)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Intuition)
	FGameplayAttributeData Intuition;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Intuition)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Leadership)
	FGameplayAttributeData Leadership;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Leadership)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Power)
	FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Power)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Prediction)
	FGameplayAttributeData Prediction;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Prediction)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Dexterity)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Craftmanship)
	FGameplayAttributeData Craftmanship;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Craftmanship)
	//***************************************************************************************

protected:

	UFUNCTION()
	virtual void OnRep_Spirituality(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Agility(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Recovery(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Intuition(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Leadership(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Power(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Prediction(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Craftmanship(const FGameplayAttributeData& OldValue);

};
