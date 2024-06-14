// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryList.h"

//Custome includes
#include "Inventory/ItemStaticData.h"
#include "Inventory/InventoryItemInstance.h"


FInventoryList::FInventoryList() // CD
{
	Items.Reserve(MaxInventorySize);
}

void FInventoryList::AddItem(TSubclassOf<UItemStaticData> inItemDataClass)
{
	if (Items.Num() >= MaxInventorySize) // CD
	{                                   
		Items.RemoveAt(0);               // CD
	}

	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();
	Item.ItemInstance->Init(inItemDataClass);
	MarkItemDirty(Item);
}


void FInventoryList::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (Items.Num() >= MaxInventorySize)  // CD
	{
		Items.RemoveAt(0);                // CD
	}

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

bool FInventoryList::AddItem(const FInventoryListItem& Item) // CD
{
	if (Items.Num() >= MaxInventorySize)
	{
		Items.RemoveAt(0);
	}
	Items.Add(Item);
	return true;
}

void FInventoryList::RemoveItem(int32 Index) // CD
{
	if (Index >= 0 && Index < Items.Num())
	{
		Items.RemoveAt(Index);
	}
}

FInventoryListItem* FInventoryList::GetItem(int32 Index) // CD
{
	if (Index >= 0 && Index < Items.Num())
	{
		return &Items[Index];
	}
	return nullptr;
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

void FInventoryList::EnforceSizeLimit()
{
	while (Items.Num() > MaxInventorySize)
	{
		Items.RemoveAt(0);
	}
}
