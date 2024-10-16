// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilitySpec.h"
#include "WorldActors/InteractableActorBase.h"
#include "InventoryItemInstance.generated.h"


class UItemStaticData;
class AInteractableActorBase;


UCLASS(BlueprintType, Blueprintable)
class PROJECTMELTDOWN_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass, int32 InQuantity = 1);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UItemStaticData* GetItemStaticData() const;

	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemDataClass;

	UPROPERTY(Replicated)
	bool bEquipped = false;
	

	virtual void OnEquipped(AActor* InOwner = nullptr);
	virtual void OnUnEquipped(AActor* InOwner = nullptr);
	virtual void OnDropItem(AActor* InOwner = nullptr);



	UFUNCTION(BlueprintPure)
	AInteractableActorBase* GetItemActor() const;

	int32 GetQuantity() const { return Quantity; }

	void AddItems(int32 Count);


protected:

	UPROPERTY(ReplicatedUsing = OnRep_ItemActor)
	AInteractableActorBase* ItemActor = nullptr;

	UFUNCTION()
	void OnRep_ItemActor();

	UPROPERTY(Replicated)
	int32 Quantity = 1;

	void TryGrantAbilities(AActor* InOwner);

	void TryRemoveAbilities(AActor* InOwner);

	void TryApplyEffects(AActor* InOwner);

	void TryRemoveEffects(AActor* InOwner);

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	TArray<FActiveGameplayEffectHandle> OngoingEffectHandles;

};
