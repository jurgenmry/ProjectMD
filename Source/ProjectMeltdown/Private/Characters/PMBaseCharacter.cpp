// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PMBaseCharacter.h"
 
//System includes
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

//custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"


APMBaseCharacter::APMBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* APMBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UPMBaseAttributeSet* APMBaseCharacter::GetAttributeSetBase() const
{
	return AttributeSet.Get();
}


int32 APMBaseCharacter::GetPlayerlevel()
{
	return 0;
}


void APMBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APMBaseCharacter::InitAbilityActorInfo()
{
}


void APMBaseCharacter::InitializeAttributes()
{
	ApplyGEToSelf(DefaultAttributes, 1.f);
	ApplyGEToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyGEToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyGEToSelf(DefaultOvertimeAttributes, 1.f);
}

void APMBaseCharacter::ApplyGEToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}


float APMBaseCharacter::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float APMBaseCharacter::GetRadiation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetRadiation();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxRadiation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxRadiation();
	}

	return 0.0f;
}

float APMBaseCharacter::GetStamina() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetStamina();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxStamina() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMoveSpeed() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue();
	}

	return 0.0f;
}

float APMBaseCharacter::GetOxigen() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetOxigen();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxOxigen() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxOxigen();
	}

	return 0.0f;
}

void APMBaseCharacter::SetHealth(float Health)
{
	if (AttributeSet)
	{
		AttributeSet->SetHealth(Health);
	}
}

void APMBaseCharacter::SetRadiation(float Radiation)
{
	if (AttributeSet)
	{
		AttributeSet->SetRadiation(Radiation);
	}
}

void APMBaseCharacter::SetStamina(float Stamina)
{
	if (AttributeSet)
	{
		AttributeSet->SetStamina(Stamina);
	}
}

void APMBaseCharacter::SetOxigen(float Oxigen)
{
	if (AttributeSet)
	{
		AttributeSet->SetOxigen(Oxigen);
	}
}


