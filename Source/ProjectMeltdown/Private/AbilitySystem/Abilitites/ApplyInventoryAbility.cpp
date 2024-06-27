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

	APMCharacter* Character = Cast<APMCharacter>(ActorInfo->AvatarActor);
	if (Character->GetInventoryComponent())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Invenotry not null")));
		InventoryComponent = Cast<UInventoryComponent>(Character->InventoryComponent);
	}
	else
	{

	}
	
	/*
	InventoryComponent = ActorInfo->OwnerActor.Get()->FindComponentByClass<UInventoryComponent>();
	if (InventoryComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Invenotry not null")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("null")));
	}
	*/
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
	AWeaponItemActor* WaponItemActor = Cast<AWeaponItemActor>(GetEquippedItemActor());

	return WaponItemActor;

	//return Cast<AWeaponItemActor>(GetEquippedItemActor());
}