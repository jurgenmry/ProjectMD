// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitites/GA_InventoryCombatAbility.h"

#include "Inventory/InventoryComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/ItemStaticData.h"
#include "WorldActors/InteractableActorBase.h"
#include "WorldActors/WeaponItemActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UGA_InventoryCombatAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags ) && HasEnoughAmmo();
}

bool UGA_InventoryCombatAbility::HasEnoughAmmo() const
{
	if (UAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponentFromActorInfo())
	{
		if (const UWeaponStaticData* WeaponStaticData = GetEquippedItemWeaponStaticData())
		{
			if (UInventoryComponent* Inventory = GetInventoryComponent())
			{
				return !WeaponStaticData->AmmoTag.IsValid() || Inventory->GetInventoryTagCount(WeaponStaticData->AmmoTag) > 0;
			}
		}
	}

	return false;
}

void UGA_InventoryCombatAbility::DecAmmo()
{
	if (UAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponentFromActorInfo())
	{
		if (const UWeaponStaticData* WeaponStaticData = GetEquippedItemWeaponStaticData())
		{
			if (!WeaponStaticData->AmmoTag.IsValid()) return;

			if (UInventoryComponent* Inventory = GetInventoryComponent())
			{
				Inventory->RemoveItemWithInventoryTag(WeaponStaticData->AmmoTag, 1);
			}
		}
	}
}

FGameplayEffectSpecHandle UGA_InventoryCombatAbility::GetWeaponEffectSpec(const FHitResult& InHitResult)
{
	if (UAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponentFromActorInfo())
	{
		if (const UWeaponStaticData* WeaponStaticData = GetEquippedItemWeaponStaticData())
		{
			FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();

			FGameplayEffectSpecHandle OutSpec = AbilityComponent->MakeOutgoingSpec(WeaponStaticData->DamageEffect, 1, EffectContext);

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(OutSpec, FGameplayTag::RequestGameplayTag(TEXT("Attributes.Meta.Damage")), WeaponStaticData->BaseDamage);

			return OutSpec;
		}
	}

	return FGameplayEffectSpecHandle();
}

const bool UGA_InventoryCombatAbility::GetWeaponToFocusTraceResult(float TraceDistance, ETraceTypeQuery TraceType, FHitResult& OutHitResult)
{
	AWeaponItemActor* WeaponItemActor = GetEquippedWeaponItemActor();

	APMCharacter* ActionGameCharacter = GetActionGameCharacterFromActorInfo();

	const FTransform& CameraTransform = ActionGameCharacter->GetFirstPersonCameraComponent()->GetComponentTransform();

	const FVector FocusTraceEnd = CameraTransform.GetLocation() + CameraTransform.GetRotation().Vector() * TraceDistance;

	TArray<AActor*> ActorsToIgnore = { GetAvatarActorFromActorInfo(), WeaponItemActor };

	FHitResult FocusHit;

	UKismetSystemLibrary::LineTraceSingle(this, CameraTransform.GetLocation(), FocusTraceEnd, TraceType, false, ActorsToIgnore, EDrawDebugTrace::Persistent, FocusHit, true);

	FVector MuzzleLocation = WeaponItemActor->GetMuzzleLocation();

	const FVector WeaponTraceEnd = MuzzleLocation + (FocusHit.Location - MuzzleLocation).GetSafeNormal() * TraceDistance;

	UKismetSystemLibrary::LineTraceSingle(this, MuzzleLocation, WeaponTraceEnd, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None, OutHitResult, true);

	return OutHitResult.bBlockingHit;
}