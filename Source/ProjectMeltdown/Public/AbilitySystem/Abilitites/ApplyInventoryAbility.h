// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemStaticData.h"
#include "AbilitySystem/Abilitites/InventoryAbility.h"
#include "ApplyInventoryAbility.generated.h"


class UInventoryItemInstance;
class UWeaponStaticData;
class AInteractableActorBase;
class AWeaponItemActor;
class UInventoryComponent;


UCLASS()
class PROJECTMELTDOWN_API UApplyInventoryAbility : public UInventoryAbility
{
	GENERATED_BODY()

public:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintPure)
	UInventoryItemInstance* GetEquippedItemInstance() const;

	UFUNCTION(BlueprintPure)
	const UItemStaticData* GetEquippedItemStaticData() const;

	UFUNCTION(BlueprintPure)
	const UWeaponStaticData* GetEquippedItemWeaponStaticData() const;

	UFUNCTION(BlueprintPure)
	AInteractableActorBase* GetEquippedItemActor() const;

	UFUNCTION(BlueprintPure)
	AWeaponItemActor* GetEquippedWeaponItemActor() const;

protected:

	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;
	
};
