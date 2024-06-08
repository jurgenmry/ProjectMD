// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PMBaseAbilitySystemComponent.h"

void UPMBaseAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UPMBaseAbilitySystemComponent::EffectApplied);
}


// So we will use the tag container to broadcast information the information of effects applied.
//This will be used on the HUD
void UPMBaseAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//Wull be used to draw to the hud 
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Blue, FString("Effect Applied"));

	//Getting tag container
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer); //retrieve the gameplay effect asset tag
	EffectAssetTags.Broadcast(TagContainer);  //Broadcast to be binded

}
