
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

	FInventoryList(); // CD

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams) 
	{ 
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaParams,*this);
	}

	void AddItem(TSubclassOf<class UItemStaticData> inItemDataClass);
	void AddItemInstance(UInventoryItemInstance* InItemInstance);

	void RemoveItem(TSubclassOf<class UItemStaticData> inItemDataClass);
	void RemoveItemInstance(UInventoryItemInstance* InItemInstance);

	bool AddItem(const FInventoryListItem& Item); // CD
	void RemoveItem(int32 Index);                 // CD
	FInventoryListItem* GetItem(int32 Index);     // CD

public:

	TArray<FInventoryListItem>& GetItemsRef() {return Items;}

	TArray<UInventoryItemInstance*> GetAllInstancesWithTag(FGameplayTag InTag);

	TArray<UInventoryItemInstance*> GetAllAvailableInstancesOfType(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FInventoryListItem> Items;

	int32 MaxInventorySize = 4; // CD
	
	void EnforceSizeLimit();
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true};
};
