// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Abilitites/ApplyInventoryAbility.h"


#include "Inventory/InventoryComponent.h"

#include "Inventory/InventoryItemInstance.h"
#include "Inventory/ItemStaticData.h"
#include "WorldActors/InteractableActorBase.h"
#include "WorldActors/WeaponItemActor.h"

void UApplyInventoryAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InventoryComponent = ActorInfo->OwnerActor.Get()->FindComponentByClass<UInventoryComponent>();
}

UInventoryComponent* UApplyInventoryAbility::GetInventoryComponent() const
{
	return InventoryComponent;
}

UInventoryItemInstance* UApplyInventoryAbility::GetEquippedItemInstance() const
{
	return InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
}

const UItemStaticData* UApplyInventoryAbility::GetEquippedItemStaticData() const
{
	UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();

	return EquippedItem ? EquippedItem->GetItemStaticData() : nullptr;
}

const UWeaponStaticData* UApplyInventoryAbility::GetEquippedItemWeaponStaticData() const
{
	return Cast<UWeaponStaticData>(GetEquippedItemStaticData());
}

AInteractableActorBase* UApplyInventoryAbility::GetEquippedItemActor() const
{
	UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();

	return EquippedItem ? EquippedItem->GetItemActor() : nullptr;
}

AWeaponItemActor* UApplyInventoryAbility::GetEquippedWeaponItemActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}