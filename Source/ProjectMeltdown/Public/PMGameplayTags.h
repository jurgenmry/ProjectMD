// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 

 Singelton that contains c++ gameplay tags

 */

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


struct FPMGameplayTags
{
public:

	static const FPMGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();


	/* Vital  Attribute Tags */
	FGameplayTag Attributes_Vital_MoveSpeed;
	FGameplayTag Attributes_Vital_MaxStamina;
	FGameplayTag Attributes_Vital_MaxHealth;
	FGameplayTag Attributes_Vital_MaxOxigen;
	FGameplayTag Attributes_Vital_MaxResilience;
	FGameplayTag Attributes_Vital_MaxSTR;
	FGameplayTag Attributes_Vital_Gold;
	FGameplayTag Attributes_Vital_StaminaRegenRate;


	/* Primary Attribute Tags */
	FGameplayTag Attributes_Primary_Streght;
	FGameplayTag Attributes_Primary_Resilience;

	/* Secondary Attribute Tags */
	FGameplayTag Attibutes_Secondary_Health;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_Critical_Hit_Chance;
	FGameplayTag Attributes_Secondary_Critical_Hit_Damage;
	FGameplayTag Attributes_Secondary_Critical_Hit_Resist;
	FGameplayTag Attributes_Secondary_MaxRadiationResist;
	FGameplayTag Attributes_Secondary_CarriyingCapacity;

	/* Meta Attribute Tags*/
	FGameplayTag Attributes_Meta_Damage;
	
	/* State Tags */
	FGameplayTag Movement_State_Crouch;
	FGameplayTag Movement_State_Sprint;
	FGameplayTag Movement_State_Dead;
	FGameplayTag Movement_State_RemoveOnDeathTag;

	/*Hit character*/
	FGameplayTag Hit_Direction_FrontTag;
	FGameplayTag Hit_Direction_BackTag;
	FGameplayTag Hit_Direction_RightTag;
	FGameplayTag Hit_Direction_LeftTag;

	
	FGameplayTag Attack_StartedEventTag;
	FGameplayTag Attack_EndedEventTag;


	/*Ability Tags*/
	FGameplayTag Ability_Combat_SingleShot;
	FGameplayTag Ability_Combat_Shoot; //Event.Combat.Shoot

	/*Enemy Tags*/
	FGameplayTag Enemy_Attack_Melee;
	FGameplayTag Enemy_Attack_Ranger;



	static FPMGameplayTags GameplayTags;
	
};