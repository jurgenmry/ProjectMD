// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PMBaseEnemyCharacter.h"


//Custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"

APMBaseEnemyCharacter::APMBaseEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPMBaseAbilitySystemComponent>(TEXT("AbilitSystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);


	AttributeSet = CreateDefaultSubobject <UPMBaseAttributeSet>(TEXT("AttributeSet"));
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
