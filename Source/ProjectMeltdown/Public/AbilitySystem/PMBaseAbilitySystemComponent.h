// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PMBaseAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&/* Asset tags*/);


UCLASS()
class PROJECTMELTDOWN_API UPMBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	/*Use to bind delegates*/
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

protected:

	/*custome code*/
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};



/*

	++++++++++++++++++++++ Game mechanics and concepts +++++++++++++++++++++++++++++



	Attributes: 

		Vital:
			
			
			
			OXIGEN        : Gets Depleted faster when running (Breathing faster)
			MAX OXIGEN    : Buy bigger oxigen tank in store
			STAMINA       : How much i can run before i give up
			GOLD          : Amount of money received after killing an enemy
			*MAX RADIATION RESISTANCE : Total amount of max radiation the boxy can take before dying 
		

		Primary:


			STRENGHT: Increase carrying capacity
			RESILIENCE: Increase Armor, Armor penetration and radiation resistance


		Secondary: {Depends On}

			{Resilience}  ARMOR               : Reduce damage taken and improves block chance
			{Resilience}  ARMOR PENETRATION   : Ignores % of enemy  armor, increase crit hit chance
			{Armor}       BLOCK CHANCE        : Cut incoming damage in half
			{Armor P}     CRITICAL HIT CHANCE : Chance to double damage + critical hit bonus
			{Armor P}     CRITICAL HIT DAMAGE : Bonus Damage on critical hit scored
			{Armor}       CRITICAL HIT RESIST : Reduce critical hit chance of attacking enemies
			{Resilience}  MAX RADIATION       : Increase amount of radiation resistance
			{Strenght}    CARRYING CAPACITY   : Increase the amount of Items we can hold
			{Carryng C}   MOVE SPEED		  : How fast I can move. (With 0 objects walk faster)         
			{Carrying C}  MAX STAMINA         : For how much time i can run
			{Oxigen}      HEALTH              : Amount of health character has. Less oxigen you die
			{Radiation}   MAX HEALTH          : Max Amount of health character can reach. (will only decrease)
			

	Abilities:

		Walk
		Run
		crouch ? perhaps. 
		Interact 

	Other actors: 

		volums: apply different type of effects to character.
			
			Radiation volums
			Oxigen volums

		Weapons: (affect encumberment)

			Shotgun: Infinite amount of rounds. Has cooldown effect.
			Round types: Different ammo you can buy at the store . Apply different effects to enemies.
			Baton: Apply stunt to enemies. 

		Store: 
			
			Where you can buy weapon rounds,  health increase, oxigen tank increase, reduce weapon cooldown, different type of stupid props

		Radiation Core: 
			
			Main objective of the game  -> Drop radiation core at the designated location. 

		Radiation core socket: (affects encumberment the most) 
			
			Place to drop the radiation core. If succesfull, we grant new cycle to clock . If not .. we die , new round is respwan. 


General clock that pass days: 

	You have certain amount of time to complete the objective .. if not everyone if death 

*/