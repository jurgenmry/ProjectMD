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
