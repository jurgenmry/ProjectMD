// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PMBaseCharacter.h"
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

	virtual int32 GetPlayerlevel() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> Enemy_AIController;

protected:

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Defaults")
	int32 lvl = { 1 };
	

};
