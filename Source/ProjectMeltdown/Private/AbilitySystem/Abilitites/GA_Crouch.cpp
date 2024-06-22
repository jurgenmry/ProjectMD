// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilitites/GA_Crouch.h"

#include "AbilitySystemComponent.h"
#include "Characters/PMCharacter.h"


UGA_Crouch::UGA_Crouch()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const APMCharacter* Character = CastChecked<APMCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	if (Character->CanCrouch())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Can crouch called successfully"));
	}
	return Character->CanCrouch();
}

void UGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APMCharacter* Character = CastChecked<APMCharacter>(ActorInfo->AvatarActor.Get());
	Character->Crouch();

}

void UGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	APMCharacter* Character = CastChecked<APMCharacter>(ActorInfo->AvatarActor.Get());
	Character->UnCrouch();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
