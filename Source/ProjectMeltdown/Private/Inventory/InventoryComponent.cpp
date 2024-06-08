
#include "Inventory/InventoryComponent.h"

//System includes
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Abilities/GameplayAbilityTypes.h"


//Custome includes
#include "Inventory/InventoryList.h"
#include "Inventory/InventoryItemInstance.h"
#include "WorldActors/PickUpBaseActor.h"
#include "GameModes/PMPlayerState.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "Characters/PMCharacter.h"

FGameplayTag UInventoryComponent::EquipItemActorTag;
FGameplayTag UInventoryComponent::DropItemTag;
FGameplayTag UInventoryComponent::EquipNextTag;
FGameplayTag UInventoryComponent::UnEquipTag;

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	
	UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddInventoryGameplayTags);

}

void UInventoryComponent::AddInventoryGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	UInventoryComponent::EquipItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItemActor"), TEXT("Equip item form item actor event"));
	UInventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.DropItemActor"), TEXT("Drop equipped item"));
	UInventoryComponent::EquipNextTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipNextItemActor"), TEXT("Try equip new item"));
	UInventoryComponent::UnEquipTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.UnEquipItemActor"), TEXT("Un equip current item"));


	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//InitializeComponent();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::InitializeInventoryComponent(APlayerController* PC,  APMPlayerState* PS, UAbilitySystemComponent* ABSC)
{
	//Super::InitializeComponent();

	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Red, FString::Printf(TEXT("Component initialized")), 1);

	if (GetOwner()->HasAuthority())
	{
		for (auto ItemClass : DefaultItems)
		{
			InvetoryList.AddItem(ItemClass);
		}
	}

	/*
	APMCharacter* Character = Cast<APMCharacter>(this->GetOwner());
	//if (!ensure(Character != nullptr)) return;

	
	//APMMainCharacterPlayerController* PC = Cast<APMMainCharacterPlayerController>(Character->GetController());
	//if(!ensure(PC != nullptr)) return;
	

	APMPlayerState* MainPlayerState = Character->GetPlayerState<APMPlayerState>();;
	//if (!ensure(MainPlayerState != nullptr)) return;

	//MainPlayerState->GetAbilitySystemComponent();*/

	if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())//UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnEquipTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);

	}
}

bool UInventoryComponent::ReplicateSubObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InvetoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UInventoryComponent::AddItemInventory(TSubclassOf<UItemStaticData> inItemDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InvetoryList.AddItem(inItemDataClass);
	}
}

void UInventoryComponent::AddItemInventoryInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner() ->HasAuthority())
	{
		InvetoryList.AddItemInstance(InItemInstance);
	}
}

void UInventoryComponent::RemoveItemInventory(TSubclassOf<UItemStaticData> inItemDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InvetoryList.RemoveItem(inItemDataClass);
	}
}

void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> inItemDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto Item : InvetoryList.GetItemsRef())
		{
			if (Item.ItemInstance->ItemDataClass == inItemDataClass)
			{
				Item.ItemInstance->OnEquipped(GetOwner());
				CurrentlyEquippedItem = Item.ItemInstance;
				break;
			}
		}
	}
}

void UInventoryComponent::EquipItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner() ->HasAuthority())
	{
		for (auto Item : InvetoryList.GetItemsRef())
		{
			if (Item.ItemInstance == InItemInstance)
			{
				Item.ItemInstance->OnEquipped(GetOwner());
				CurrentlyEquippedItem = Item.ItemInstance;
				break;
			}
		}
	}
}

void UInventoryComponent::EquipNext()
{
	TArray<FInventoryListItem>& Items = InvetoryList.GetItemsRef();

	const bool bNoItems = Items.Num() == 0;
	const bool bOneAndEquipped = Items.Num() == 1 && CurrentlyEquippedItem;

	if (bNoItems || bOneAndEquipped) return;
	
	UInventoryItemInstance* TargetItem = CurrentlyEquippedItem;

	for (auto Item : Items)
	{
		if (Item.ItemInstance->GetItemStaticData()->bCanEquip)
		{
			if (Item.ItemInstance != CurrentlyEquippedItem)
			{
				TargetItem = Item.ItemInstance;
				break;
			}
		}
	}

	if (CurrentlyEquippedItem)
	{
		if (TargetItem == CurrentlyEquippedItem)
		{
			return;
		}

		UnEquipItem();
	}

	EquipItemInstance(TargetItem);
}

void UInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*Payload);
	}

	else if(NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}

}

void UInventoryComponent::HandleGameplayEventInternal(FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		FGameplayTag EventTag = Payload.EventTag;

		if (EventTag == UInventoryComponent::EquipItemActorTag)
		{
			if (const UInventoryItemInstance* ITemInstance = Cast<UInventoryItemInstance>(Payload.OptionalObject))
			{
				AddItemInventoryInstance(const_cast<UInventoryItemInstance*>(ITemInstance));
				if (Payload.Instigator)
				{
					static_cast<AActor*>(Payload.Instigator)->Destroy();
				}
			}
		}
		else if (EventTag == UInventoryComponent::EquipNextTag)
		{
			EquipNext();
		}
		else if (EventTag == UInventoryComponent::DropItemTag)
		{
			DropEquipItem();
		}

		else if (EventTag == UInventoryComponent::UnEquipTag)
		{
			UnEquipItem();
		}
	}
}

void UInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEventInternal(Payload);
}

void UInventoryComponent::UnEquipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentlyEquippedItem))
		{
			CurrentlyEquippedItem->OnUnEquipped();
			CurrentlyEquippedItem = nullptr;
		}
	}
}

void UInventoryComponent::DropEquipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentlyEquippedItem))
		{
			CurrentlyEquippedItem->OnDropItem();
			RemoveItemInventory(CurrentlyEquippedItem->ItemDataClass);
			CurrentlyEquippedItem = nullptr;
		}
	}
}

UInventoryItemInstance* UInventoryComponent::GetCurrentlyEquippedItem() const
{
	return CurrentlyEquippedItem;
}




void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InvetoryList);
	DOREPLIFETIME(UInventoryComponent, CurrentlyEquippedItem);
}


