// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryList.h"

//Custome includes
#include "Inventory/ItemStaticData.h"
#include "Inventory/InventoryItemInstance.h"


void FInventoryList::AddItem(TSubclassOf<UItemStaticData> inItemDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();
	Item.ItemInstance->Init(inItemDataClass);
	MarkItemDirty(Item);
}


void FInventoryList::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(TSubclassOf<class UItemStaticData> inItemDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticData()->IsA(inItemDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

void FInventoryList::RemoveItemInstance(UInventoryItemInstance* InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

TArray<UInventoryItemInstance*> FInventoryList::GetAllInstancesWithTag(FGameplayTag InTag)
{
	TArray<UInventoryItemInstance*> OutInstances;

	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;

		if (Item.ItemInstance->GetItemStaticData()->InventoryTags.Contains(InTag))
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}

	return OutInstances;
}

TArray<UInventoryItemInstance*> FInventoryList::GetAllAvailableInstancesOfType(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	TArray<UInventoryItemInstance*> OutInstances;

	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;

		const UItemStaticData* StaticData = Item.ItemInstance->GetItemStaticData();

		const bool bSameType = StaticData->IsA(InItemStaticDataClass);
		const bool bHasCapacity = StaticData->MaxStackCount > Item.ItemInstance->GetQuantity();

		if (bSameType && bHasCapacity)
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}

	return OutInstances;
}