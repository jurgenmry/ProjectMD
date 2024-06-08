// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMELTDOWN_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void PickUpItem(class APMCharacter* InPickupOwner) = 0;

	virtual void SwapItem() = 0;

	virtual void EquipItem(class APMCharacter* InPickupOwner) = 0;

	virtual void DropItem() = 0;
};
