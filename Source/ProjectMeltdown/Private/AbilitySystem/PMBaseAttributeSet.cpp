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


UPMBaseAttributeSet::UPMBaseAttributeSet()
{
}

void UPMBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// If a Max value changes, adjust current to keep Current % of Current to Max

	/*
	if (Attribute == GetMaxHealthAttribute()) 
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	*/
	

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

void UPMBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	/*
	//Source = couser of the effect
	//Target = Target of the effect (Owner of this Atribute set)

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	APMBaseCharacter* TargetCharacter = nullptr;
	

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<APMBaseCharacter>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	APMBaseCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<APMBaseCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<APMBaseCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}
	*/

	/*
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = true;

			if (TargetCharacter)
			{
				//WasAlive = TargetCharacter->IsAlive();
			}

			//if (!TargetCharacter->IsAlive())
			//{
			//	//UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), TEXT(__FUNCTION__), *TargetCharacter->GetName());
			//}

			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			//Start block here
			//if (TargetCharacter && WasAlive)
			//{
				// This is the log statement for damage received. Turned off for live games.
				//UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

				// Play HitReact animation and sound with a multicast RPC.
				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();

				//if (Hit)
				//{
				//	EGDHitReactDirection HitDirection = TargetCharacter->GetHitReactDirection(Data.EffectSpec.GetContext().GetHitResult()->Location);
				//	switch (HitDirection)
				//	{
				//	case EGDHitReactDirection::Left:
				//		TargetCharacter->PlayHitReact(HitDirectionLeftTag, SourceCharacter);
				//		break;
				//	case EGDHitReactDirection::Front:
				//		TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				//		break;
				//	case EGDHitReactDirection::Right:
				//		TargetCharacter->PlayHitReact(HitDirectionRightTag, SourceCharacter);
				//		break;
				//	case EGDHitReactDirection::Back:
				//		TargetCharacter->PlayHitReact(HitDirectionBackTag, SourceCharacter);
				//		break;
				//	}
				//}
				//else
				//{
				//	// No hit result. Default to front.
				//	TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				//}
				//
				// Show damage number for the Source player unless it was self damage
				//if (SourceActor != TargetActor)
				//{
				//	AGDPlayerController* PC = Cast<AGDPlayerController>(SourceController);
				//	if (PC)
				//	{
				//		PC->ShowDamageNumber(LocalDamageDone, TargetCharacter);
				//	}
				//}

				//if (!TargetCharacter->IsAlive())
				//{
				//	// TargetCharacter was alive before this damage and now is not alive, give XP and Gold bounties to Source.
				//	// Don't give bounty to self.
				//	if (SourceController != TargetController)
				//	{
				//		// Create a dynamic instant Gameplay Effect to give the bounties
				//		UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
				//		GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;
				//
				//		int32 Idx = GEBounty->Modifiers.Num();
				//		GEBounty->Modifiers.SetNum(Idx + 2);
				//
				//		FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
				//		InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
				//		InfoXP.ModifierOp = EGameplayModOp::Additive;
				//		InfoXP.Attribute = UGDAttributeSetBase::GetXPAttribute();
				//
				//		FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
				//		InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
				//		InfoGold.ModifierOp = EGameplayModOp::Additive;
				//		InfoGold.Attribute = UGDAttributeSetBase::GetGoldAttribute();
				//
				//		Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
				//	}
				//}
			//}	
		//}
	//	}*/

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
	
	// Damage

	
	//clamping of max health to not go bellow 0
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Clamp(GetMaxHealth(), 0.0f, 100.0f));
	}

	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
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

void UPMBaseAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		//if (bFatal)
		//{
		//}
	}

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

