// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemInstance.generated.h"


class UItemStaticData;
class APickUpBaseActor;

UCLASS(BlueprintType, Blueprintable)
class PROJECTMELTDOWN_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Init(TSubclassOf<UItemStaticData> InItemDataClass);

	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UItemStaticData* GetItemStaticData() const;

	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_Equipped)
	bool Equipped = false;
	
	UFUNCTION()
	void OnRep_Equipped();


	virtual void OnEquipped(AActor* InOwner = nullptr);
	virtual void OnUnEquipped();
	virtual void OnDropItem();


protected:

	UPROPERTY(Replicated)
	APickUpBaseActor* ItemActor = nullptr;

};
