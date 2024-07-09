// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/PMBaseAttributeSet.h"

//System includes
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"


//Custome includes
#include "ProjectMeltdown/Public/Characters/PMBaseCharacter.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "PMGameplayTags.h"


UPMBaseAttributeSet::UPMBaseAttributeSet()
{
}

void UPMBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//VITAL ATTRIBUTES
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Oxigen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MaxOxigen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MaxResilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MaxSTR, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Radiation, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, OxigenDegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);

	//PRIMARY ATTRIBUTES
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Streght, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Resilience, COND_None, REPNOTIFY_Always);


	//PRIMARY ATTRIBUTES
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MaxRadiation, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, CarryingCapacity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	//OTHER POTENTIAL ATTRIBUTES

	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, RadRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, XPBounty, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPMBaseAttributeSet, GoldBounty, COND_None, REPNOTIFY_Always);

}

void UPMBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxOxigenAttribute())
	{
		AdjustAttributeForMaxChange(Oxigen, MaxOxigen, NewValue, GetOxigenAttribute());
	}

	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}

void UPMBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}


}

void UPMBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	//clamping of max health to not go bellow 0
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{	
		SetMaxHealth(FMath::Clamp(GetMaxHealth(), 0.0f, GetMaxHealth()));
	}

	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetRadiationAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalRadiationDamage = GetRadiation();
		//SetRadiation(0.f);
		if (GetHealth() == GetMaxHealth())
		{
			if (LocalRadiationDamage > 0.0f)
			{
				// Apply the health change and then clamp it
				const float NewHealth = GetHealth() - 1;
				//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, FString::Printf(TEXT("Value of local rad %f"), LocalRadiationDamage));
				SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			}
		}
		if (GetHealth() < GetMaxHealth())
		{
			//Do nothing
		}
	}
		
	/*Radiation should be clamp at 100 or the value of the health ? radiation affects max health */
	else if (Data.EvaluatedData.Attribute == GetRadiationAttribute())
	{
		SetRadiation(FMath::Clamp(GetRadiation(), 0.0f, 100.0f));
	}

	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		// Handle stamina changes.
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}

	else if (Data.EvaluatedData.Attribute == GetOxigenAttribute())
	{
		SetOxigen(FMath::Clamp(GetOxigen(), 0.0f, GetMaxOxigen()));
	}

	else if (Data.EvaluatedData.Attribute == GetStreghtAttribute())
	{
		SetStreght(FMath::Clamp(GetStreght(), 0.0f, GetMaxSTR()));
	}

	else if (Data.EvaluatedData.Attribute == GetResilienceAttribute())
	{
		SetResilience(FMath::Clamp(GetResilience(), 0.0f, GetMaxResilience()));
	}
}



void UPMBaseAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (!bFatal)
		{
			//Check because we can add here another tags as well

			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FPMGameplayTags::Get().Hit_React);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
	}

}


void UPMBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}




/////////////////////////////////////////////////////////////
//  
//                      VITAL ATTRIBUTES
// 
////////////////////////////////////////////////////////////


void UPMBaseAttributeSet::OnRep_Oxigen(const FGameplayAttributeData& OldOxigen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Oxigen, OldOxigen);
}

void UPMBaseAttributeSet::OnRep_MaxOxigen(const FGameplayAttributeData& OldMaxOxigen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MaxOxigen, OldMaxOxigen);
}

void UPMBaseAttributeSet::OnRep_MaxResilience(const FGameplayAttributeData& OldMaxResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MaxResilience, OldMaxResilience);
}

void UPMBaseAttributeSet::OnRep_MaxSTR(const FGameplayAttributeData& OldMaxSTR)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MaxSTR, OldMaxSTR);
}

void UPMBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Stamina, OldStamina);
}

void UPMBaseAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Gold, OldGold);
}

void UPMBaseAttributeSet::OnRep_Radiation(const FGameplayAttributeData& OldRadiation)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Radiation, OldRadiation);
}

void UPMBaseAttributeSet::OnRep_OxigenDegenRate(const FGameplayAttributeData& OldOxigenDegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, OxigenDegenRate, OldOxigenDegenRate);
}

void UPMBaseAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, StaminaRegenRate, OldStaminaRegenRate);
}


/////////////////////////////////////////////////////////////
//  
//                    PRIMARY ATTRIBUTES
// 
////////////////////////////////////////////////////////////

void UPMBaseAttributeSet::OnRep_Streght(const FGameplayAttributeData& OldStreght)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Streght, OldStreght);
}

void UPMBaseAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Resilience, OldResilience);
}


/////////////////////////////////////////////////////////////
//  
//                    SECONDARY ATTRIBUTES
// 
////////////////////////////////////////////////////////////

void UPMBaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Armor, OldArmor);
}

void UPMBaseAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UPMBaseAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, BlockChance, OldBlockChance);
}

void UPMBaseAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UPMBaseAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UPMBaseAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UPMBaseAttributeSet::OnRep_CarryingCapacity(const FGameplayAttributeData& OldCarryingCapacity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, CarryingCapacity, OldCarryingCapacity);
}

void UPMBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UPMBaseAttributeSet::OnRep_MaxRadiation(const FGameplayAttributeData& OldMaxRadiation)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MaxRadiation, OldMaxRadiation);
}

void UPMBaseAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UPMBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, MaxStamina, OldMaxStamina);
}

void UPMBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, Health, OldHealth);
}


/////////////////////////////////////////////////////////////
//  
//                    OTHER ATTRIBUTES
// 
////////////////////////////////////////////////////////////


void UPMBaseAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, HealthRegenRate, OldHealthRegenRate);
}

void UPMBaseAttributeSet::OnRep_RadRegenRate(const FGameplayAttributeData& OldRadRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, RadRegenRate, OldRadRegenRate);
}

void UPMBaseAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, CharacterLevel, OldCharacterLevel);
}

void UPMBaseAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, XP, OldXP);
}

void UPMBaseAttributeSet::OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, XPBounty, OldXPBounty);
}

void UPMBaseAttributeSet::OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPMBaseAttributeSet, GoldBounty, OldGoldBounty);
}

