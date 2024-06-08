// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PMBaseCharacter.h"
#include "PMBaseEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMELTDOWN_API APMBaseEnemyCharacter : public APMBaseCharacter
{
	GENERATED_BODY()

	APMBaseEnemyCharacter();

public:

	virtual void BeginPlay() override;

	virtual int32 GetPlayerlevel() override;

protected:

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Defaults")
	int32 lvl = { 1 };
	
};
