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

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UBlendSpace* GetLocomotionBlendspace() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UAnimSequenceBase* GetIdleAnimation() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UBlendSpace* GetCrouchLocomotionBlendspace() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	class UAnimSequenceBase* GetCrouchIdleAnimation() const;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UCharacterAnimDataAsset* DefaultCharacterAnimDataAsset;
	
};
