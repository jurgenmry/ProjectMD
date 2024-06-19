// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PMAnimationInstance.h"

//Custome includes;
#include "Characters/PMCharacter.h"
#include "Inventory/ItemStaticData.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/BlendSpace.h"
#include "DataAssets/CharacterAnimDataAsset.h"



#include "DataAssets/CharacterDataAsset.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"

const UItemStaticData* UPMAnimationInstance::GetEquippedItemData() const
{
	APMCharacter* PMCharacter = Cast<APMCharacter>(GetOwningActor());
	UInventoryComponent* InventoryComponent = PMCharacter ? PMCharacter->GetInventoryComponent() : nullptr;
	UInventoryItemInstance* ItemInstance = InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;

	return ItemInstance ? ItemInstance->GetItemStaticData() : nullptr;

}

UBlendSpace* UPMAnimationInstance::GetLocomotionBlendspace() const
{
	
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData.MovementBlendspace)
			{
				return ItemData->CharacterAnimationData.MovementBlendspace;
			}
		}

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData();

		if (Data.MovementBlendspace)
		{
			return Data.MovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace : nullptr;
}

UAnimSequenceBase* UPMAnimationInstance::GetIdleAnimation() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData.IdleAnimationAsset)
			{
				return ItemData->CharacterAnimationData.IdleAnimationAsset;
			}
		}

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData();

		if (Data.IdleAnimationAsset)
		{
			return Data.IdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;

}

UBlendSpace* UPMAnimationInstance::GetCrouchLocomotionBlendspace() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData.CrouchMovementBlendspace)
			{
				return ItemData->CharacterAnimationData.CrouchMovementBlendspace;
			}
		}

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData();

		if (Data.CrouchMovementBlendspace)
		{
			return Data.CrouchMovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchMovementBlendspace : nullptr;
}

UAnimSequenceBase* UPMAnimationInstance::GetCrouchIdleAnimation() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData.CrouchIdleAnimationAsset)
			{
				return ItemData->CharacterAnimationData.CrouchIdleAnimationAsset;
			}
		}

		// this should be change to avoid using CharacterData
		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData();

		if (Data.CrouchIdleAnimationAsset)
		{
			return Data.CrouchIdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;	
}
