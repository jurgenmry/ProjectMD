// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PMBaseEnemyCharacter.h"


//Custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

APMBaseEnemyCharacter::APMBaseEnemyCharacter(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UPMBaseAbilitySystemComponent>(TEXT("AbilitSystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);


	AttributeSet = CreateDefaultSubobject <UPMBaseAttributeSet>(TEXT("AttributeSet"));
}

void APMBaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())return;
	Enemy_AIController = Cast<ABaseAIController>(NewController);
	Enemy_AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	Enemy_AIController->RunBehaviorTree(BehaviorTree);
}

void APMBaseEnemyCharacter::BeginPlay()
{
	InitAbilityActorInfo();
}

int32 APMBaseEnemyCharacter::GetPlayerlevel()
{
	return lvl;
}

void APMBaseEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();
}
