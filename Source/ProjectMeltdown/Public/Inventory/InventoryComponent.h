// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "AbilitySystemComponent.h"
#include "InventoryComponent.generated.h"

class UItemStaticData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMELTDOWN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventoryComponent();

	UFUNCTION()
	void InitializeInventoryComponent(class APlayerController* PC, class APMPlayerState* PS, class UAbilitySystemComponent* ABSC);

	virtual bool ReplicateSubObjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);

	UFUNCTION(BlueprintCallable)
	void AddItemInventory(TSubclassOf<UItemStaticData> inItemDataClass);


	UFUNCTION(BlueprintCallable)
	void AddItemInventoryInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItemInventory(TSubclassOf<UItemStaticData> inItemDataClass);



	UFUNCTION(BlueprintCallable) //can be change to equip specific class of item
	void EquipItem(TSubclassOf<UItemStaticData> inItemDataClass); 

	UFUNCTION(BlueprintCallable) 
	void EquipItemInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void EquipNext();



	virtual void GameplayEventCallback(const FGameplayEventData* Payload);
	static FGameplayTag EquipItemActorTag;
	static FGameplayTag DropItemTag;
	static FGameplayTag EquipNextTag;
	static FGameplayTag UnEquipTag;



	UFUNCTION(BlueprintCallable) //can be change to Unequip specific class of item
	void UnEquipItem();

	UFUNCTION(BlueprintCallable) //can be change to Unequip specific class of item
	void DropEquipItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryItemInstance* GetCurrentlyEquippedItem() const;

	

protected:
	


	void HandleGameplayEventInternal(FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);

	UFUNCTION(BlueprintCallable)
	void AddInventoryGameplayTags();


	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	FInventoryList InvetoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>>DefaultItems;

	UPROPERTY(Replicated)
	UInventoryItemInstance* CurrentlyEquippedItem {nullptr};

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
