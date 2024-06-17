// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "AbilitySystemComponent.h"
#include "Inventory/FastArrayTagCounter.h"
#include "InventoryComponent.generated.h"

class UItemStaticData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMELTDOWN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventoryComponent();

	void InitCustomeComponent(class UPMBaseAbilitySystemComponent* ASC);

	//virtual void InitializeComponent() override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void RemoveItemInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItemWithInventoryTag(FGameplayTag Tag, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void EquipItemInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintCallable)
	void EquipNext();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryItemInstance* GetEquippedItem() const;

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	static FGameplayTag EquipItemActorTag;
	static FGameplayTag DropItemTag;
	static FGameplayTag EquipNextTag;
	static FGameplayTag UnequipTag;
	static FGameplayTag CanTraceItemActorTag; //CD
	static FGameplayTag RemoveCanTraceItemActorTag;//CD

	static  FGameplayTag EquipItem1Tag; //CD
	static  FGameplayTag EquipItem2Tag; //CD
	static  FGameplayTag EquipItem3Tag; //CD
	static  FGameplayTag EquipItem4Tag; //CD

	UFUNCTION(BlueprintCallable)
	void EquipItemAtIndex(int32 Index); //CD

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", Replicated)
	bool bCanTraceItemActorTag; //CD

	UFUNCTION(BlueprintCallable)
	int32 GetInventoryTagCount(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable)
	void AddInventoryTagCount(FGameplayTag InTag, int32 CountDelta);

	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	UPROPERTY(Replicated)
	UInventoryItemInstance* TracedItemInstance;

protected:

	UFUNCTION()
	void AddInventoryTags();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

	UPROPERTY(Replicated)
	UInventoryItemInstance* CurrentItem ;

	UPROPERTY(Replicated)
	FFastArrayTagCounter InventoryTags;

	TArray<UInventoryItemInstance*> GetAllInstancesWithTag(FGameplayTag Tag);

	void HandleGameplayEventInternal(FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);


	//Tracing for checking item / ui
	void TraceForItems(UInventoryItemInstance* InItemInstance);
	bool PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
