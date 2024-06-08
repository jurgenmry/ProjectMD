// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/EffectActorBase.h"

//System includes

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AEffectActorBase::AEffectActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject <USceneComponent>(TEXT("SceneRoot")));

}

// Called when the game starts or when spawned
void AEffectActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEffectActorBase::OnOverlap(AActor* TargetActor)
{
	if (InstantEffecAplicationPolicy == EffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTargets(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffecAplicationPolicy == EffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTargets(TargetActor, InstantGameplayEffectClass);
	}

	if (InfinitEffecAplicationPolicy == EffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTargets(TargetActor, InfiniteGameplayEffect);
	}
}

void AEffectActorBase::EndOverlap(AActor* TargetActor)
{
	if (InstantEffecAplicationPolicy == EffectApplicationPolicy::AppyInEndOverlap)
	{
		ApplyEffectToTargets(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffecAplicationPolicy == EffectApplicationPolicy::AppyInEndOverlap)
	{
		ApplyEffectToTargets(TargetActor, DurationGameplayEffectClass);
	}

	if (InfinitEffecAplicationPolicy == EffectApplicationPolicy::AppyInEndOverlap)
	{
		ApplyEffectToTargets(TargetActor, InfiniteGameplayEffect);
	}

	if (InfiniteEffectRemovalPolicy == EffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void AEffectActorBase::ApplyEffectToTargets(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAbilitySystem == nullptr) { return; } //avoid crash


	/*
	1- Take ability system and Choose where to apply the effect :
		ApplyGameplay effect to self : TargetAbilitySystem->ApplyGameplayEffectSpecToSelf();
	2- For that we need a "Gampley Effect Spec" (FGameplayEffectSpecHandle).
	3- The "Gampley Effect Spec" needs (FGameplayEffectSpecHandle):
		a. Gameplay effect
		b. Level : ameplay effect can have levels as they grow bigger or more effects
		c. Context Handle (FGameplayEffectContextHandle): That is basically a wrapper for the data we are pushing. The data is the true context.
	4. FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystem->MakeEffectContext() makes the handle
	*/

	check(GameplayEffectClass); // like ensure to avoid editor crash

	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystem->MakeEffectContext();

	//we need to understand what caused this effect
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystem->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); // We need a spec and we can make one from ability system above


	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystem);
	}
}
