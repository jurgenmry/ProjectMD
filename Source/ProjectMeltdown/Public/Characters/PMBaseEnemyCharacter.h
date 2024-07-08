// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PMBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "DataAssets/CharacterDataAsset.h"
#include "PMBaseEnemyCharacter.generated.h"


class UBehaviorTree;
class ABaseAIController;

UCLASS()
class PROJECTMELTDOWN_API APMBaseEnemyCharacter : public APMBaseCharacter
{
	GENERATED_BODY()

public:

	APMBaseEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MAIN|Character Class Defaults")
	ECharacterClass BaseCharacterClass = ECharacterClass::EnemyMelee;

	UPROPERTY(EditAnywhere, Category = "MAIN|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "MAIN|Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadWrite, Category = "MAIN|Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY()
	TObjectPtr<ABaseAIController> Enemy_AIController;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:

	virtual void InitAbilityActorInfo();
	
	void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void InitializeAttributes() override;
};
