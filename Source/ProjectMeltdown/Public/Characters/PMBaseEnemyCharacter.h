// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PMBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "PMBaseEnemyCharacter.generated.h"


UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	EnemyRanger  UMETA(DisplayName = "Ranger"),
	EnemyMelee   UMETA(DisplayName = "Melee")
};


class UBehaviorTree;
class ABaseAIController;

UCLASS()
class PROJECTMELTDOWN_API APMBaseEnemyCharacter : public ACharacter, public IAbilitySystemInterface //public APMBaseCharacter
{
	GENERATED_BODY()

public:

	APMBaseEnemyCharacter(); //const class FObjectInitializer& ObjectInitializer);

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UPMBaseAttributeSet* GetAttributeSetBase() const;

	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	ECharacterClass BaseCharacterClass = ECharacterClass::EnemyMelee;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> Enemy_AIController;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:

	virtual void InitAbilityActorInfo();
	
	UPROPERTY()
	TObjectPtr<class UPMBaseAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<class UPMBaseAttributeSet> AttributeSet;


	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultOvertimeAttributes;


	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	virtual void ApplyGEToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level); //Apply effect to self

	void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
