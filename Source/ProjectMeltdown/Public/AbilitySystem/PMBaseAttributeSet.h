// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PMBaseAttributeSet.generated.h"


/*	Attributes:

	Vital:

		MAX OXIGEN      : Buy bigger oxigen tank in store
		OXIGEN          : Gets Depleted faster when running(Breathing faster)
		MAX RESILIENCE  : Max Amount of resilience a character can take
		MAX STRENGHT    : Max Amount of extreght to carry stuff
		MAX STAMINA     : For how much time i can run affected by carrying capacity but not secondary attribute
		STAMINA         : How much i can run before i give up
		HEALTH          : Amount of health character has.Less oxigen you die (could be vital attribute)
	    MAX HEALTH      : Max Amount of health character can reach. (will only decrease)
		GOLD            : Amount of money received after killing an enemy
		MOVE SPEED      : How fast I can move. (With 0 objects walk faster) {Carrying C}  
		


	Primary:

		STRENGHT   : Increase carrying capacity
		RESILIENCE : Increase Armor, Armor penetration and radiation resistance

	Secondary : {Depends On}

		{Resilience}  ARMOR               : Reduce damage taken and improves block chance
		{Resilience}  ARMOR PENETRATION   : Ignores % of enemy  armor, increase crit hit chance
		{Armor}       BLOCK CHANCE        : Cut incoming damage in half
		{Armor P}     CRITICAL HIT CHANCE : Chance to double damage + critical hit bonus
		{Armor P}     CRITICAL HIT DAMAGE : Bonus Damage on critical hit scored
		{Armor}       CRITICAL HIT RESIST : Reduce critical hit chance of attacking enemies
		{Resilience}  MAX RADIATION       : Increase amount of radiation resistance
		
		{Strenght} CARRYING CAPACITY   : Increase the amount of Items we can hold
		
		{health} RADIATION: 

		radiation should be set to a value of 0 and clamp to the value of health (or 100)
		When entering volum how much radiation damage you take
		and will affect the max health.
		when i increase radiation max health should decrease. 

	Over Time:

		OxigenDegenRate     : Amount of oxigen lost when going out
		STAMINA REN         : Stamina regeneration after loosing it

*/

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class PROJECTMELTDOWN_API UPMBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPMBaseAttributeSet();

	// AttributeSet Overrides

	virtual void PreAttributeChange(const FGameplayAttribute & Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Current Health, when 0 we expect owner to die unless prevented by an ability. Capped by MaxHealth.
	// Positive changes can directly use this.
	// Negative changes to Health should go through Damage meta attribute.



	//======================================================== //
	//                                                         //   
	//                    VITAL ATTRIBUTES                     //
	//                                                         //  
	//======================================================== //


		//OXIGEN: Gets Depleted faster when running(Breathing faster)
	
		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_Oxigen)
		FGameplayAttributeData Oxigen;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Oxigen)
		UFUNCTION()
		virtual void OnRep_Oxigen(const FGameplayAttributeData& OldOxigen);

		//MAX OXIGEN : Buy bigger oxigen tank in store
		
		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_MaxOxigen)
		FGameplayAttributeData MaxOxigen;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MaxOxigen)
		UFUNCTION()
		virtual void OnRep_MaxOxigen(const FGameplayAttributeData& OldMaxOxigen);

		//MAX RESILIENCE : Max Amount of resilience a character can take
		
		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_MaxResilience)
		FGameplayAttributeData MaxResilience;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MaxResilience)
		UFUNCTION()
		virtual void OnRep_MaxResilience(const FGameplayAttributeData& OldMaxResilience);

		//MAX STRENGHT : Max Amount of extreght to carry stuff

		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_MaxSTR)
		FGameplayAttributeData MaxSTR;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MaxSTR)
		UFUNCTION()
		virtual void OnRep_MaxSTR(const FGameplayAttributeData& OldMaxSTR);

		//STAMINA : How much i can run before i give up

		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_Stamina)
		FGameplayAttributeData Stamina;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Stamina)
		UFUNCTION()
		virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

		//GOLD : Amount of money received after killing an enemy (or deliver the core)

		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_Gold)
		FGameplayAttributeData Gold;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Gold)
		UFUNCTION()
		virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);

		// Current Radiation, used to execute special abilities. Capped by MaxMana.
		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_Radiation)
		FGameplayAttributeData Radiation;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Radiation)
		UFUNCTION()
		virtual void OnRep_Radiation(const FGameplayAttributeData& OldRadiation);

		// Mana regen rate will passively increase Mana every second
		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_OxigenDegenRate)
		FGameplayAttributeData OxigenDegenRate;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, OxigenDegenRate)
		UFUNCTION()
		virtual void OnRep_OxigenDegenRate(const FGameplayAttributeData& OldOxigenDegenRate);

		// Stamina regen rate will passively increase Stamina every second
		UPROPERTY(BlueprintReadOnly, Category = "Vital attributes", ReplicatedUsing = OnRep_StaminaRegenRate)
		FGameplayAttributeData StaminaRegenRate;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, StaminaRegenRate)
		UFUNCTION()
		virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);


	//======================================================== //
	//                                                         //   
	//                  PRIMARY ATTRIBUTES                     //
	//                                                         //  
	//======================================================== //

		//STRENGHT  : Increase carrying capacity
		
		UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Streght)
		FGameplayAttributeData Streght;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Streght)
		UFUNCTION()
		virtual void OnRep_Streght(const FGameplayAttributeData& OldStreght);
		
		
		//RESILIENCE : Increase Armor, Armor penetration and radiation resistance

		UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes", ReplicatedUsing = OnRep_Resilience)
		FGameplayAttributeData Resilience;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Resilience)
		UFUNCTION()
		virtual void OnRep_Resilience(const FGameplayAttributeData& OldResilience);



	//======================================================== //
	//                                                         //   
	//                  SECONDARY ATTRIBUTES                   //
	//                                                         //  
	//======================================================== //


		//{Resilience}ARMOR : Reduce damage taken and improves block chance
		
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_Armor)
		FGameplayAttributeData Armor;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Armor)
		UFUNCTION()
		virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

		// {Resilience} ARMOR PENETRATION : Ignores % of enemy  armor, increase crit hit chance

		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_ArmorPenetration)
		FGameplayAttributeData ArmorPenetration;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, ArmorPenetration)
		UFUNCTION()
		virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);

		// {Armor} BLOCK CHANCE : Cut incoming damage in half
		
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_BlockChance)
		FGameplayAttributeData BlockChance;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, BlockChance)
		UFUNCTION()
		virtual void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance);

		//{Armor P} CRITICAL HIT CHANCE : Chance to double damage + critical hit bonus
		
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CriticalHitChance)
		FGameplayAttributeData CriticalHitChance;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, CriticalHitChance)
		UFUNCTION()
		virtual void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);

		//{Armor P} CRITICAL HIT DAMAGE : Bonus Damage on critical hit scored

		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CriticalHitDamage)
		FGameplayAttributeData CriticalHitDamage;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, CriticalHitDamage)
		UFUNCTION()
		virtual void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);

		//{Armor} CRITICAL HIT RESIST : Reduce critical hit chance of attacking enemies
		
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CriticalHitResistance)
		FGameplayAttributeData CriticalHitResistance;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, CriticalHitResistance)
		UFUNCTION()
		virtual void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance);

		//{Strenght}  CARRYING CAPACITY : Increase the amount of Items we can hold
	
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_CarryingCapacity)
		FGameplayAttributeData CarryingCapacity;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, CarryingCapacity)
		UFUNCTION()
		virtual void OnRep_CarryingCapacity(const FGameplayAttributeData& OldCarryingCapacity);


		//{Radiation}   MAX HEALTH  : Max Amount of health character can reach. (will only decrease)

		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MaxHealth)
		UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

		//{Resilience}  MAX RADIATION : Increase amount of radiation resistance
			
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_MaxRadiation)
		FGameplayAttributeData MaxRadiation;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MaxRadiation)
		UFUNCTION()
		virtual void OnRep_MaxRadiation(const FGameplayAttributeData& OldMaxRadiation);

		//{Carrying C}  MOVE SPEED : How fast I can move. (With 0 objects walk faster)
		
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_MoveSpeed)
		FGameplayAttributeData MoveSpeed;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MoveSpeed)
		UFUNCTION()
		virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);


		//{Carrying C} MAX STAMINA  : For how much time i can run
		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_MaxStamina)
		FGameplayAttributeData MaxStamina;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, MaxStamina)
		UFUNCTION()
		virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

		 
		//{Oxigen} HEALTH  : Amount of health character has.Less oxigen you die

		UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Health)
		UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);



		//======================================================== //
		//                                                         //   
		//               OTHER POTENTIAL ATTRIBUTES                //
		//                                                         //  
		//======================================================== //

		// Health regen rate will passively increase Health every second
		UPROPERTY(BlueprintReadOnly, Category = "Other attributes", ReplicatedUsing = OnRep_HealthRegenRate)
		FGameplayAttributeData HealthRegenRate;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, HealthRegenRate)
		UFUNCTION()
		virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

		// Mana regen rate will passively increase Mana every second
		UPROPERTY(BlueprintReadOnly, Category = "Other attributes", ReplicatedUsing = OnRep_RadRegenRate)
		FGameplayAttributeData RadRegenRate;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, RadRegenRate)
		UFUNCTION()
		virtual void OnRep_RadRegenRate(const FGameplayAttributeData& OldRadRegenRate);


		// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
		// Temporary value that only exists on the Server. Not replicated.
		UPROPERTY(BlueprintReadOnly, Category = "Other attributes")
		FGameplayAttributeData Damage;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, Damage)


		UPROPERTY(BlueprintReadOnly, Category = "Other attributes", ReplicatedUsing = OnRep_CharacterLevel)
		FGameplayAttributeData CharacterLevel;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, CharacterLevel)
		UFUNCTION()
		virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);


		// Experience points gained from killing enemies. Used to level up (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "Other attributes", ReplicatedUsing = OnRep_XP)
		FGameplayAttributeData XP;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, XP)
		UFUNCTION()
		virtual void OnRep_XP(const FGameplayAttributeData& OldXP);

		// Experience points awarded to the character's killers. Used to level up (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "Other attributes", ReplicatedUsing = OnRep_XPBounty)
		FGameplayAttributeData XPBounty;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, XPBounty)
		UFUNCTION()
		virtual void OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty);

		// Gold awarded to the character's killer. Used to purchase items (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "Other attributes", ReplicatedUsing = OnRep_GoldBounty)
		FGameplayAttributeData GoldBounty;
		ATTRIBUTE_ACCESSORS(UPMBaseAttributeSet, GoldBounty)
		UFUNCTION()
		virtual void OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty);

protected:

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	
};
