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

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData3P();

		if (Data.MovementBlendspace)
		{
			return Data.MovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset3P ? DefaultCharacterAnimDataAsset3P->CharacterAnimationData.MovementBlendspace : nullptr;
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

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData3P();

		if (Data.IdleAnimationAsset)
		{
			return Data.IdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset3P ? DefaultCharacterAnimDataAsset3P->CharacterAnimationData.IdleAnimationAsset : nullptr;

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

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData3P();

		if (Data.CrouchMovementBlendspace)
		{
			return Data.CrouchMovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset3P ? DefaultCharacterAnimDataAsset3P->CharacterAnimationData.CrouchMovementBlendspace : nullptr;
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
		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData3P();

		if (Data.CrouchIdleAnimationAsset)
		{
			return Data.CrouchIdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset3P ? DefaultCharacterAnimDataAsset3P->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
}

///////////////////////////////////////////////////////////////////
//
//                          FIRST PERSON
//
///////////////////////////////////////////////////////////////////


UBlendSpace* UPMAnimationInstance::Get1FPLocomotionBlendspace() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData1FP.MovementBlendspace)
			{
				return ItemData->CharacterAnimationData1FP.MovementBlendspace;
			}
		}

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData1FP();

		if (Data.MovementBlendspace)
		{
			return Data.MovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset1FP ? DefaultCharacterAnimDataAsset1FP->CharacterAnimationData.MovementBlendspace : nullptr;
}

UAnimSequenceBase* UPMAnimationInstance::Get1FPIdleAnimation() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData1FP.IdleAnimationAsset)
			{
				return ItemData->CharacterAnimationData1FP.IdleAnimationAsset;
			}
		}

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData1FP();

		if (Data.IdleAnimationAsset)
		{
			return Data.IdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset1FP ? DefaultCharacterAnimDataAsset1FP->CharacterAnimationData.IdleAnimationAsset : nullptr;

}

UBlendSpace* UPMAnimationInstance::Get1FPCrouchLocomotionBlendspace() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData1FP.CrouchMovementBlendspace)
			{
				return ItemData->CharacterAnimationData1FP.CrouchMovementBlendspace;
			}
		}

		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData1FP();

		if (Data.CrouchMovementBlendspace)
		{
			return Data.CrouchMovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset1FP ? DefaultCharacterAnimDataAsset1FP->CharacterAnimationData.CrouchMovementBlendspace : nullptr;
}

UAnimSequenceBase* UPMAnimationInstance::Get1FPCrouchIdleAnimation() const
{
	if (APMCharacter* ActionGameCharacter = Cast<APMCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemData = GetEquippedItemData())
		{
			if (ItemData->CharacterAnimationData1FP.CrouchIdleAnimationAsset)
			{
				return ItemData->CharacterAnimationData1FP.CrouchIdleAnimationAsset;
			}
		}

		// this should be change to avoid using CharacterData
		FCharacterAnimationData Data = ActionGameCharacter->GetCharacterData1FP();

		if (Data.CrouchIdleAnimationAsset)
		{
			return Data.CrouchIdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset1FP ? DefaultCharacterAnimDataAsset1FP->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
}
