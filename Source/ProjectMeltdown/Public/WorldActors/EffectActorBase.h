// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "EffectActorBase.generated.h"



class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EffectApplicationPolicy
{
	ApplyOnOverlap,
	AppyInEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotApply
};


UCLASS()
class PROJECTMELTDOWN_API AEffectActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AEffectActorBase();
	virtual void BeginPlay() override;

protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apply Effect")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apply Effect")
	float ActorLevel = 1.0f;


	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent* >ActiveEffectHandles;


	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTargets(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	/*Instant*/
	UPROPERTY(BlueprintReadWrite, Category = "Apply Effect")
	TSubclassOf <UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apply Effect")
	EffectApplicationPolicy InstantEffecAplicationPolicy = EffectApplicationPolicy::DoNotApply;


	/*Duration*/
	UPROPERTY(BlueprintReadWrite, Category = "Apply Effect")
	TSubclassOf <UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apply Effect")
	EffectApplicationPolicy DurationEffecAplicationPolicy = EffectApplicationPolicy::DoNotApply;;


	/*Periodic*/
	UPROPERTY(BlueprintReadWrite, Category = "Apply Effect")
	TSubclassOf <UGameplayEffect> PeriodicGameplayEffectClass;


	/*Infinite*/
	UPROPERTY(BlueprintReadWrite, Category = "Apply Effect")
	TSubclassOf <UGameplayEffect> InfiniteGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apply Effect")
	EffectApplicationPolicy InfinitEffecAplicationPolicy = EffectApplicationPolicy::DoNotApply;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apply Effect")
	EffectRemovalPolicy InfiniteEffectRemovalPolicy = EffectRemovalPolicy::DoNotApply;
};
