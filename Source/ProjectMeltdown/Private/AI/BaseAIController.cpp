// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

//System includes

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

//custome Includes

ABaseAIController::ABaseAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComponent"));

	CPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("CPerceptionComponent"));

}

