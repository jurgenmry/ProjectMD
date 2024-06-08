// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameplayTags.h"

//System include

#include "GameplayTagsManager.h"


FPMGameplayTags FPMGameplayTags::GameplayTags;

void FPMGameplayTags::InitializeNativeGameplayTags()
{

	/*Vital attribute Tags */

	GameplayTags.Attributes_Vital_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MoveSpeed"), 
		FString("Speed of character while moving"));

	GameplayTags.Attributes_Vital_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxStamina"),
		FString("Max stamina, allows movement of character"));
	 
	GameplayTags.Attributes_Vital_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxHealth"),
		FString("Max Health of character"));

	GameplayTags.Attributes_Vital_MaxOxigen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxOxigen"),
		FString("Max oxigen for moving/surviving"));

	GameplayTags.Attributes_Vital_MaxResilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxResilience"),
		FString("Max amount of resilience a character can reach"));

	GameplayTags.Attributes_Vital_MaxSTR = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxSTR"),
		FString("Max amount of STR a character can reach"));

	GameplayTags.Attributes_Vital_Gold = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Gold"),
		FString("Used to buy in the store"));

	GameplayTags.Attributes_Vital_StaminaRegenRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.StaminaRegenRate"),
		FString("How fast will stamina regenerate"));



	/* Primary Attribute Tags */

	GameplayTags.Attributes_Primary_Streght = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Streght"),
		FString("Str of the character to perform certain actions"));

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Resilience of the character to perform certain actions"));


	/* Secondary Attribute Tags */

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("how strong you are against attacks"));

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("how strong you are to break enemy armor"));

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("how strong you are to block enemy attack"));

	GameplayTags.Attributes_Secondary_Critical_Hit_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Critical.Hit.Chance"),
		FString("Chances of performing a critical while attacking"));

	GameplayTags.Attributes_Secondary_Critical_Hit_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Critical.Hit.Damage"),
		FString("Chances of doing more damage when critical attack"));

	GameplayTags.Attributes_Secondary_Critical_Hit_Resist = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Critical.Hit.Resist"),
		FString("Chances of resist more damage when critical attack received"));

	GameplayTags.Attributes_Secondary_MaxRadiationResist = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxRadiationResist"),
		FString("Chances of resist more radiation"));

	GameplayTags.Attributes_Secondary_CarriyingCapacity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CarriyingCapacity"),
		FString("How much weight we can carry"));
}
