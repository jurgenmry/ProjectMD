// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

//System includes
#include "Net/UnrealNetWork.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemLog.h"
#include "Blueprint/UserWidget.h"

//Custome includes
#include "BlueprintLibraries/ProjectMeltdownStatics.h"
#include "WorldActors/InteractableActorBase.h"
#include "Characters/PMCharacter.h"

#include "Inventory/ItemStaticData.h"



void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemInstance, ItemDataClass);
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
	DOREPLIFETIME(UInventoryItemInstance, Quantity);
	DOREPLIFETIME(UInventoryItemInstance, bEquipped);
}



void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass, int32 InQuantity)
{
	ItemDataClass = InItemStaticDataClass;
	Quantity = InQuantity;
}

const UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UProjectMeltdownStatics::GetItemStaticData(ItemDataClass);
}


void UInventoryItemInstance::OnEquipped(AActor* InOwner)
{
	if (UWorld* World = InOwner->GetWorld())
	{
		const UItemStaticData* StaticData = GetItemStaticData();

		FTransform Transform;
		ItemActor = World->SpawnActorDeferred<AInteractableActorBase>(StaticData->ItemActorClass, Transform, InOwner);
		ItemActor->Init(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(Transform);

		APMCharacter* Character = Cast<APMCharacter>(InOwner);

		USkeletalMeshComponent* TargetMesh = nullptr;
		FName SocketName;

		if (Character->IsLocallyControlled())
		{
			TargetMesh = Character->GetMesh1P();
			SocketName = StaticData->AttachmentSocket;
		}
		else
		{
			TargetMesh = Character->GetMesh3P();
			SocketName = StaticData->TPAttachmentSocket;
		}

		if (TargetMesh && TargetMesh->DoesSocketExist(SocketName))
		{
			ItemActor->AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}

	TryGrantAbilities(InOwner);

	TryApplyEffects(InOwner);

	bEquipped = true;
}

void UInventoryItemInstance::OnRep_ItemActor()
{
	if (ItemActor && ItemActor->GetOwner())
	{
		APMCharacter* Character = Cast<APMCharacter>(ItemActor->GetOwner());
		if (Character)
		{
			// Attach to the correct mesh based on local control
			const UItemStaticData* StaticData = GetItemStaticData();
			if (!StaticData)
			{
				return;
			}

			USkeletalMeshComponent* TargetMesh = nullptr;
			FName SocketName;

			if (Character->IsLocallyControlled())
			{
				TargetMesh = Character->GetMesh1P();
				SocketName = StaticData->AttachmentSocket;
			}
			else
			{
				TargetMesh = Character->GetMesh3P();
				SocketName = StaticData->TPAttachmentSocket;
			}

			if (TargetMesh && TargetMesh->DoesSocketExist(SocketName))
			{
				ItemActor->AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			}
		}
	}
}


void UInventoryItemInstance::OnUnEquipped(AActor* InOwner)
{
	if (ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}

	TryRemoveAbilities(InOwner);

	TryRemoveEffects(InOwner);

	bEquipped = false;
}

void UInventoryItemInstance::OnDropItem(AActor* InOwner)
{
	if (ItemActor)
	{
		ItemActor->OnDropped();
	}

	TryRemoveAbilities(InOwner);

	TryRemoveEffects(InOwner);

	bEquipped = false;
}


AInteractableActorBase* UInventoryItemInstance::GetItemActor() const
{
	return ItemActor;
}

void UInventoryItemInstance::TryGrantAbilities(AActor* InOwner)
{
	if (InOwner && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			const UItemStaticData* StaticData = GetItemStaticData();

			for (auto ItemAbility : StaticData->GrantedAbilities)
			{
				GrantedAbilityHandles.Add(AbilityComponent->GiveAbility(FGameplayAbilitySpec(ItemAbility)));
			}
		}
	}
}

void UInventoryItemInstance::TryRemoveAbilities(AActor* InOwner)
{
	if (InOwner && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			const UItemStaticData* StaticData = GetItemStaticData();

			for (auto AbilityHandle : GrantedAbilityHandles)
			{
				AbilityComponent->ClearAbility(AbilityHandle);
			}

			GrantedAbilityHandles.Empty();
		}
	}
}

void UInventoryItemInstance::TryApplyEffects(AActor* InOwner)
{
	if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		const UItemStaticData* ItemStaticData = GetItemStaticData();

		FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();

		for (auto GameplayEffect : ItemStaticData->OngoingEffects)
		{
			if (!GameplayEffect.Get()) continue;

			FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveGEHandle.WasSuccessfullyApplied())
				{
					ABILITY_LOG(Log, TEXT("Item %s failed to apply runtime effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
				}
				else
				{
					OngoingEffectHandles.Add(ActiveGEHandle);
				}
			}
		}
	}
}

void UInventoryItemInstance::TryRemoveEffects(AActor* InOwner)
{
	if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		for (FActiveGameplayEffectHandle ActiveEffectHandle : OngoingEffectHandles)
		{
			if (ActiveEffectHandle.IsValid())
			{
				AbilityComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}
	}

	OngoingEffectHandles.Empty();
}

void UInventoryItemInstance::AddItems(int32 Count)
{
	Quantity += Count;

	if (Quantity < 0)
	{
		Quantity = 0;
	}
}