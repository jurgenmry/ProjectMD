// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitites/FireGun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Inventory/ItemStaticData.h"
#include "Characters/PMCharacter.h"
#include "Camera/CameraComponent.h"
#include "WorldActors/InteractableActorBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WorldActors/WeaponItemActor.h"

void UFireGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UFireGun::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

FGameplayEffectSpecHandle UFireGun::GetWeaponEffectSpec(const FHitResult& InHitResult)
{
	if (UAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponentFromActorInfo())
	{
		if (const UWeaponStaticData* WeaponStaticData = GetEquippedItemWeaponStaticData())
		{
			FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();

			FGameplayEffectSpecHandle OutSpec = AbilityComponent->MakeOutgoingSpec(WeaponStaticData->DamageEffect, 1, EffectContext);

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(OutSpec, FGameplayTag::RequestGameplayTag(TEXT("Attribute.Health")), -WeaponStaticData->BaseDamage);

			return OutSpec;
		}
	}

	return FGameplayEffectSpecHandle();
}

const bool UFireGun::GetWeaponToFocusTraceResult(float TraceDistance, ETraceTypeQuery TraceType, FHitResult& OutHitResult)
{
	AWeaponItemActor* WeaponItemActor = GetEquippedWeaponItemActor();

	APMCharacter* GameCharacter = GetActionGameCharacterFromActorInfo();

	const FTransform& CameraTransform = GameCharacter->GetFirstPersonCameraComponent()->GetComponentTransform();

	const FVector FocusTraceEnd = CameraTransform.GetLocation() + CameraTransform.GetRotation().Vector() * TraceDistance;

	TArray<AActor*> ActorsToIgnore = { GetAvatarActorFromActorInfo() };

	FHitResult FocusHit;

	UKismetSystemLibrary::LineTraceSingle(this, CameraTransform.GetLocation(), FocusTraceEnd, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None, FocusHit, true);

	FVector MuzzleLocation = WeaponItemActor->GetMuzzleLocation();

	const FVector WeaponTraceEnd = MuzzleLocation + (FocusHit.Location - MuzzleLocation).GetSafeNormal() * TraceDistance;

	UKismetSystemLibrary::LineTraceSingle(this, MuzzleLocation, WeaponTraceEnd, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None, OutHitResult, true);

	return OutHitResult.bBlockingHit;
}
