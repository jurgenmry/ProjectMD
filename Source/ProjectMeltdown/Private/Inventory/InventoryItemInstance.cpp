// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

//System includes
#include "Net/UnrealNetWork.h"

//Custome includes
#include "BlueprintLibraries/ProjectMeltdownStatics.h"
#include "WorldActors/PickUpBaseActor.h"
#include "Characters/PMCharacter.h"
#include "Components/SkeletalMeshComponent.h"


void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemInstance, ItemDataClass);
	DOREPLIFETIME(UInventoryItemInstance, Equipped);
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
}

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemDataClass)
{
	ItemDataClass = InItemDataClass;
}

const UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UProjectMeltdownStatics::GetItemStaticData(ItemDataClass);
}

void UInventoryItemInstance::OnRep_Equipped()
{
	
}

void UInventoryItemInstance::OnEquipped(AActor* InOwner)
{
	if (UWorld* World = GetWorld())
	{
		const UItemStaticData* StaticData = GetItemStaticData();

		FTransform Transform;
		APickUpBaseActor* ItenActor = World->SpawnActorDeferred<APickUpBaseActor>(StaticData->ItemActorClass, Transform, InOwner);
		//ItemActor->InitActor(this);
		//ItemActor->OnEquipped();
		//ItemActor->FinishSpawning(Transform);
		APMCharacter* Character = Cast<APMCharacter>(InOwner);
		if (USkeletalMeshComponent* SkeletalMesh = Character ? Character->GetMesh1P() : nullptr)
		{
			//ItemActor->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, StaticData->AttachedSocket);
		}
	}

	Equipped = true;
}

void UInventoryItemInstance::OnUnEquipped()
{
	if (ItemActor)
	{
		//ItemActor->Destroy();
		//ItemActor = nullptr;
	}

	Equipped = false;
}

void UInventoryItemInstance::OnDropItem()
{
	if (ItemActor)
	{
		//ItemActor->OnDropped();
	}
	Equipped = false;
}
