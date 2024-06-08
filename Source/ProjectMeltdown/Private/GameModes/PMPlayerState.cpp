// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PMPlayerState.h"

//Custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"

//System includes
#include "Net/UnrealNetwork.h"

APMPlayerState::APMPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPMBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPMBaseAttributeSet>(TEXT("AttributeSet"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* APMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPMBaseAttributeSet* APMPlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}

void APMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APMPlayerState, lvl);
}


////////////////////////////
//						  //
// GETTERS FOR ATTRIBUTES //
//                        //  
////////////////////////////

float APMPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float APMPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float APMPlayerState::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegenRate();
}

float APMPlayerState::GetRadiation() const
{
	return AttributeSet->GetRadiation();
}

float APMPlayerState::GetMaxRadiation() const
{
	return AttributeSet->GetMaxRadiation();
}

float APMPlayerState::GetRadRegenRate() const
{
	return AttributeSet->GetRadRegenRate();
}

float APMPlayerState::GetOxigen() const
{
	return AttributeSet->GetOxigen();
}

float APMPlayerState::GetMaxOxigen() const
{
	return AttributeSet->GetMaxOxigen();
}

float APMPlayerState::GetDegenOxigen() const
{
	return AttributeSet->GetOxigenDegenRate();
}

float APMPlayerState::GetStamina() const
{
	return AttributeSet->GetStamina();
}

float APMPlayerState::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float APMPlayerState::GetStaminaRegenRate() const
{
	return AttributeSet->GetStaminaRegenRate();
}

float APMPlayerState::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

int32 APMPlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetCharacterLevel();
}

int32 APMPlayerState::GetXP() const
{
	return AttributeSet->GetXP();
}

int32 APMPlayerState::GetXPBounty() const
{
	return AttributeSet->GetXPBounty();
}

int32 APMPlayerState::GetGold() const
{
	return AttributeSet->GetGold();
}

int32 APMPlayerState::GetGoldBounty() const
{
	return AttributeSet->GetGoldBounty();
}

void APMPlayerState::OnRep_Level(int32 OldLevel)
{
}
