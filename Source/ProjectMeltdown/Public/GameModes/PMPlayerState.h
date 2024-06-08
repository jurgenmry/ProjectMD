// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "PMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMELTDOWN_API APMPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	APMPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UPMBaseAttributeSet* GetAttributeSetBase() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/////////////////////////////////////////////////////////////////////////////
	// 
	// Getters for attributes. Returns Current Value unless otherwise specified.
	//
	////////////////////////////////////////////////////////////////////////////

	/*Health Getters*/
	UFUNCTION(BlueprintCallable, Category = "Gas|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Gas|Attributes")
	float GetHealthRegenRate() const;


	/*Radiation Getters*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetRadiation() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxRadiation() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetRadRegenRate() const;


	/*Oxigen Getters*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetOxigen() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxOxigen() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetDegenOxigen() const;


	/* Stamina Getters*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetStaminaRegenRate() const;

	
	/*Movement Getter*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMoveSpeed() const;


	/*Character Level Getter*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	int32 GetCharacterLevel() const;


	/*XP Getters*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	int32 GetXP() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	int32 GetXPBounty() const;


	/*Gold Getters*/
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	int32 GetGoldBounty() const;

	FORCEINLINE int32 GetCurrentLvl() const { return lvl; }

protected:

	UPROPERTY()
	class UPMBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UPMBaseAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 lvl = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
};
