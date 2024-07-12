// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UBehaviorTreeComponent;
class UAIPerceptionComponent;

UCLASS()
class PROJECTMELTDOWN_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTreeComponent> BehaviourTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionComponent> CPerceptionComponent;

};
