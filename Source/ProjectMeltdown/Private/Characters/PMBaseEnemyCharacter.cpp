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


APMBaseEnemyCharacter::APMBaseEnemyCharacter()//const class FObjectInitializer& ObjectInitializer)	:Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UPMBaseAbilitySystemComponent>(TEXT("AbilitSystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject <UPMBaseAttributeSet>(TEXT("AttributeSet"));

	//SetRootComponent(GetCapsuleComponent());
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;


	bAlwaysRelevant = true;
}

UAbilitySystemComponent* APMBaseEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UPMBaseAttributeSet* APMBaseEnemyCharacter::GetAttributeSetBase() const
{
	return AttributeSet.Get();
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
	if (AbilitySystemComponent)
	{
		InitAbilityActorInfo();
	}
	
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
	Cast<UPMBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->AbilityActorInfoSet();
	
	if (HasAuthority())
	{
		InitializeAttributes();
		AddCharacterAbilities();
	}
}

void APMBaseEnemyCharacter::InitializeAttributes()
{
	ApplyGEToSelf(DefaultAttributes, 1.f);
	ApplyGEToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyGEToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyGEToSelf(DefaultOvertimeAttributes, 1.f);
}

void APMBaseEnemyCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility));

		//FString AbilityName = StartupAbility ? StartupAbility->GetName() : FString("Unknown");
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Added Ability %s"), *AbilityName), 1);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void APMBaseEnemyCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

void APMBaseEnemyCharacter::ApplyGEToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void APMBaseEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}
