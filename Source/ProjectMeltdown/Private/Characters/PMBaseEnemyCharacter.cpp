// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PMBaseEnemyCharacter.h"


//System includes
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"

//Custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"
#include "AI/BaseAIController.h"
#include "BlueprintLibraries/ProjectMeltdownStatics.h"


APMBaseEnemyCharacter::APMBaseEnemyCharacter(const class FObjectInitializer& ObjectInitializer)	:Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UPMBaseAbilitySystemComponent>(TEXT("AbilitSystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject <UPMBaseAttributeSet>(TEXT("AttributeSet"));

	//SetRootComponent(GetCapsuleComponent());
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;


	bAlwaysRelevant = true;
}


void APMBaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())return;
	Enemy_AIController = Cast<ABaseAIController>(NewController);
	Enemy_AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	Enemy_AIController->RunBehaviorTree(BehaviorTree);

	Enemy_AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	Enemy_AIController->GetBlackboardComponent()->SetValueAsBool(FName("Ranger"), BaseCharacterClass != ECharacterClass::EnemyMelee);
}

void APMBaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();


}


void APMBaseEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : 250.0f;
	if (Enemy_AIController && Enemy_AIController->GetBlackboardComponent())
	{
		Enemy_AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void APMBaseEnemyCharacter::InitAbilityActorInfo()
{
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	//Cast<UPMBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->AbilityActorInfoSet();
	
	if (HasAuthority())
	{
		InitializeAttributes();
		//AddCharacterAbilities();
	}
}

void APMBaseEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void APMBaseEnemyCharacter::InitializeAttributes()
{
	UProjectMeltdownStatics::InitializeEnemyDefaultAttributes(this, BaseCharacterClass, AbilitySystemComponent);
}
