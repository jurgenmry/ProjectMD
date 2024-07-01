// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Inventory/ItemStaticData.h"
#include "DataAssets/CharacterAnimDataAsset.h"
#include "PMAnimationInstance.generated.h"




UCLASS()
class PROJECTMELTDOWN_API UPMAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	const UItemStaticData* GetEquippedItemData() const;

	//Third Person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UBlendSpace* GetLocomotionBlendspace() const;

	//First person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UBlendSpace* Get1FPLocomotionBlendspace() const;

	//Third Person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UAnimSequenceBase* GetIdleAnimation() const;

	//First person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UAnimSequenceBase* Get1FPIdleAnimation() const;

	//Third Person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UBlendSpace* GetCrouchLocomotionBlendspace() const;

	//First Person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UBlendSpace* Get1FPCrouchLocomotionBlendspace() const;

	//Third Person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UAnimSequenceBase* GetCrouchIdleAnimation() const;

	//Third Person
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UAnimSequenceBase* Get1FPCrouchIdleAnimation() const;

	//Third Person
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UCharacterAnimDataAsset* DefaultCharacterAnimDataAsset3P;

	//First Person
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UCharacterAnimDataAsset* DefaultCharacterAnimDataAsset1FP;
	
};
