// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class PROJECTMELTDOWN_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController();

protected:
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviourTreeComponent;
};
