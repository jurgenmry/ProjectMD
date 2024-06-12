
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayPrediction.h"
#include "InventoryList.generated.h"


class UInventoryItemInstance;
class UInventoryComponent;

USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UInventoryItemInstance* ItemInstance = nullptr;
};


USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams) 
	{ 
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaParams,*this);
	}

	void AddItem(TSubclassOf<class UItemStaticData> inItemDataClass);
	void AddItemInstance(UInventoryItemInstance* InItemInstance);

	void RemoveItem(TSubclassOf<class UItemStaticData> inItemDataClass);
	void RemoveItemInstance(UInventoryItemInstance* InItemInstance);

public:

	TArray<FInventoryListItem>& GetItemsRef() {return Items;}

	TArray<UInventoryItemInstance*> GetAllInstancesWithTag(FGameplayTag InTag);

	TArray<UInventoryItemInstance*> GetAllAvailableInstancesOfType(TSubclassOf<UItemStaticData> InItemStaticDataClass);

protected:

	UPROPERTY()
	TArray<FInventoryListItem> Items;

};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true};
};
