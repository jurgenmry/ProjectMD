
#include "Inventory/InventoryComponent.h"

//System includes
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/GameplayStatics.h"

//Custome includes
#include "Inventory/InventoryList.h"
#include "Inventory/InventoryItemInstance.h"
#include "WorldActors/InteractableActorBase.h"
#include "GameModes/PMPlayerState.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "Characters/PMCharacter.h"
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"

#include "Inventory/FastArrayTagCounter.h"

FGameplayTag UInventoryComponent::EquipItemActorTag;
FGameplayTag UInventoryComponent::DropItemTag;
FGameplayTag UInventoryComponent::EquipNextTag;
FGameplayTag UInventoryComponent::UnequipTag;
FGameplayTag UInventoryComponent::CanTraceItemActorTag;
FGameplayTag UInventoryComponent::RemoveCanTraceItemActorTag;
FGameplayTag UInventoryComponent::EquipItem1Tag;
FGameplayTag UInventoryComponent::EquipItem2Tag;
FGameplayTag UInventoryComponent::EquipItem3Tag;
FGameplayTag UInventoryComponent::EquipItem4Tag;


static TAutoConsoleVariable<int32> CVarShowInventory(
	TEXT("ShowDebugInventory"),
	0,
	TEXT("Draws debug info about inventory")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	static bool bHandledAddingTags = false;
	if (!bHandledAddingTags)
	{
		bHandledAddingTags = true;
		UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddInventoryTags);
	}
}

int32 UInventoryComponent::GetInventoryTagCount(FGameplayTag Tag) const
{
	return InventoryTags.GetTagCount(Tag);
}

void UInventoryComponent::AddInventoryTagCount(FGameplayTag InTag, int32 CountDelta)
{
	InventoryTags.AddTagCount(InTag, CountDelta);
}

void UInventoryComponent::EquipItemAtIndex(int32 Index)
{
	const FInventoryListItem* Item = InventoryList.GetItem(Index);
	
	//UnEquip the currently Equipped item
	if (CurrentItem)
	{
		UnequipItem();
	}

	//Equip the new item from index
	if (Item)
	{
		EquipItemInstance(Item->ItemInstance);
	}
}

void UInventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	UInventoryComponent::EquipItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItemActor"), TEXT("Equip item from item actor event"));
	UInventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.DropItem"), TEXT("Drop equipped item"));
	UInventoryComponent::EquipNextTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipNext"), TEXT("Try equip next item"));
	UInventoryComponent::UnequipTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.Unequip"), TEXT("Unequip current item"));
	UInventoryComponent::CanTraceItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.Trace"), TEXT("Allow Trace for Equipping Items"));
	UInventoryComponent::RemoveCanTraceItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.RemoveCanTraceItemActorTag"), TEXT("Hides widget"));
	
	UInventoryComponent::EquipItem1Tag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItem1"), TEXT("Add Item on Slot 1"));
	UInventoryComponent::EquipItem2Tag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItem2"), TEXT("Equip item on Slot2"));
	UInventoryComponent::EquipItem3Tag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItem3"));
	UInventoryComponent::EquipItem4Tag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItem4"));

	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

void UInventoryComponent::InitCustomeComponent(class UPMBaseAbilitySystemComponent* ASC)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto ItemClass : DefaultItems)
		{
			InventoryList.AddItem(ItemClass);
		}
	}

	if (ASC)//UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnequipTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::CanTraceItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);

		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::RemoveCanTraceItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItem1Tag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItem2Tag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItem3Tag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItem4Tag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
	}
}

/*
void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (auto ItemClass : DefaultItems)
		{
			InventoryList.AddItem(ItemClass);
		}
	}

	APMCharacter* CharacterOwner = Cast<APMCharacter>(this->GetOwner());
	APMPlayerState* PS = Cast<APMPlayerState>(CharacterOwner->GetPlayerState());
	UPMBaseAbilitySystemComponent* ASC = Cast<UPMBaseAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	if (ASC)//UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnequipTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
	}
}*/

void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemStaticDataClass);
	}
}

void UInventoryComponent::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		TArray<UInventoryItemInstance*> Items = InventoryList.GetAllAvailableInstancesOfType(InItemInstance->ItemDataClass);

		Algo::Sort(Items, [](UInventoryItemInstance* InA, UInventoryItemInstance* InB)
			{
				return InA->GetQuantity() < InB->GetQuantity();
			}
		);

		const int32 MaxItemStackCount = InItemInstance->GetItemStaticData()->MaxStackCount;

		int32 ItemsLeft = InItemInstance->GetQuantity();

		for (auto Item : Items)
		{
			const int32 EmptySlots = MaxItemStackCount - Item->GetQuantity();

			int32 SlotsToAdd = ItemsLeft;

			if (ItemsLeft > EmptySlots)
			{
				SlotsToAdd = EmptySlots;
			}

			ItemsLeft -= SlotsToAdd;

			Item->AddItems(SlotsToAdd);
			InItemInstance->AddItems(-SlotsToAdd);

			
			for (FGameplayTag InvTag : Item->GetItemStaticData()->InventoryTags)
			{
				InventoryTags.AddTagCount(InvTag, SlotsToAdd);
			}
			

			if (ItemsLeft <= 0)
			{
				ItemsLeft = 0;

				return;
			}
		}

		while (ItemsLeft > MaxItemStackCount)
		{
			AddItem(InItemInstance->GetItemStaticData()->GetClass());

			
			for (FGameplayTag InvTag : InItemInstance->GetItemStaticData()->InventoryTags)
			{
				InventoryTags.AddTagCount(InvTag, MaxItemStackCount);
			}
			

			ItemsLeft -= MaxItemStackCount;
			InItemInstance->AddItems(-MaxItemStackCount);
		}

		InventoryList.AddItemInstance(InItemInstance);

		
		for (FGameplayTag InvTag : InItemInstance->GetItemStaticData()->InventoryTags)
		{
			InventoryTags.AddTagCount(InvTag, InItemInstance->GetQuantity());
		}
		

	}
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(InItemStaticDataClass);
	}
}

void UInventoryComponent::RemoveItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItemInstance(InItemInstance);

		
		for (FGameplayTag InvTag : InItemInstance->GetItemStaticData()->InventoryTags)
		{
			InventoryTags.AddTagCount(InvTag, -InItemInstance->GetQuantity());
		}
		
	}
}

void UInventoryComponent::RemoveItemWithInventoryTag(FGameplayTag Tag, int32 Count)
{
	if (GetOwner()->HasAuthority())
	{
		int32 CountLeft = Count;

		TArray<UInventoryItemInstance*> Items = GetAllInstancesWithTag(Tag);

		Algo::Sort(Items, [](UInventoryItemInstance* InA, UInventoryItemInstance* InB)
			{
				return InA->GetQuantity() < InB->GetQuantity();
			}
		);

		for (auto Item : Items)
		{
			int32 AvailableCount = Item->GetQuantity();
			int32 ItemsToRemove = CountLeft;

			if (ItemsToRemove >= AvailableCount)
			{
				ItemsToRemove = AvailableCount;

				RemoveItemInstance(Item);
			}
			else
			{
				Item->AddItems(-ItemsToRemove);

				
				for (FGameplayTag InvTag : Item->GetItemStaticData()->InventoryTags)
				{
					InventoryTags.AddTagCount(InvTag, -ItemsToRemove);
				}
				
			}

			CountLeft -= ItemsToRemove;
		}
	}
}

void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance->ItemDataClass == InItemStaticDataClass)
			{
				Item.ItemInstance->OnEquipped(GetOwner());

				CurrentItem = Item.ItemInstance;


				break;
			}
		}
	}
}

void UInventoryComponent::EquipItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance == InItemInstance)
			{
				Item.ItemInstance->OnEquipped(GetOwner());
				CurrentItem = Item.ItemInstance;
				break;
			}
		}
	}
}

void UInventoryComponent::EquipNext()
{
	TArray<FInventoryListItem>& Items = InventoryList.GetItemsRef();

	const bool bNoItems = Items.Num() == 0;
	const bool bOneAndEquipped = Items.Num() == 1 && CurrentItem;

	if (bNoItems || bOneAndEquipped) return;

	UInventoryItemInstance* TargetItem = CurrentItem;

	for (auto Item : Items)
	{
		if (Item.ItemInstance->GetItemStaticData()->bCanBeEquipped)
		{
			if (Item.ItemInstance != CurrentItem)
			{
				TargetItem = Item.ItemInstance;
				break;
			}
		}
	}

	if (CurrentItem)
	{
		if (TargetItem == CurrentItem)
		{
			return;
		}

		UnequipItem();
	}

	EquipItemInstance(TargetItem);
}

void UInventoryComponent::UnequipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnUnEquipped(GetOwner());
			CurrentItem = nullptr;
		}
	}
}

void UInventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnDropItem(GetOwner());//OnDropped(GetOwner());
			RemoveItem(CurrentItem->ItemDataClass);
			CurrentItem = nullptr;
		}
	}
}

UInventoryItemInstance* UInventoryComponent::GetEquippedItem() const
{
	return CurrentItem;
}

void UInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}

TArray<UInventoryItemInstance*> UInventoryComponent::GetAllInstancesWithTag(FGameplayTag Tag)
{
	TArray<UInventoryItemInstance*> OutInstances;

	OutInstances = InventoryList.GetAllInstancesWithTag(Tag);

	return OutInstances;
}

void UInventoryComponent::HandleGameplayEventInternal(FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		FGameplayTag EventTag = Payload.EventTag;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Handling gameplay event: %s"), *EventTag.ToString()));


		if (EventTag == UInventoryComponent::EquipItemActorTag)
		{
			/*
				if (Payload.OptionalObject)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("EquipItemActorTag: OptionalObject is valid: %s"), *Payload.OptionalObject->GetName()));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("EquipItemActorTag: OptionalObject is null")));
				}
				*/

			if (const UInventoryItemInstance* ItemInstance = Cast<UInventoryItemInstance>(Payload.OptionalObject))
			{
				AddItemInstance(const_cast<UInventoryItemInstance*>(ItemInstance));
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("EquipItemActorTag: Item equipped: %s"), *ItemInstance->GetName()));
		
				if (Payload.Instigator && Payload.Instigator.Get())
				{
						const_cast<AActor*>(Payload.Instigator.Get())->Destroy();
				}
				
			}
		}
	
		else if (EventTag == UInventoryComponent::EquipNextTag)
		{
			EquipNext();
		}
		else if (EventTag == UInventoryComponent::DropItemTag)
		{
			DropItem();
		}
		else if (EventTag == UInventoryComponent::UnequipTag)
		{
			UnequipItem();
		}
		else if (EventTag == UInventoryComponent::CanTraceItemActorTag) //CD
		{
			bCanTraceItemActorTag = true;
		}

		else if (EventTag == UInventoryComponent::RemoveCanTraceItemActorTag) //CD
		{
			bCanTraceItemActorTag = false;
			TracedItemInstance = nullptr;

		}
		else if (EventTag == UInventoryComponent::EquipItem1Tag) //CD
		{
			EquipItemAtIndex(0);
		}
		else if (EventTag == UInventoryComponent::EquipItem2Tag) //CD
		{
			EquipItemAtIndex(1);
		}
		else if (EventTag == UInventoryComponent::EquipItem3Tag) //CD
		{
			EquipItemAtIndex(2);
		}
		else if (EventTag == UInventoryComponent::EquipItem4Tag) //CD
		{
			EquipItemAtIndex(3);
		}
	}
}

void UInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEventInternal(Payload);
}

bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bShowDebug = CVarShowInventory.GetValueOnGameThread() != 0;
	if (bShowDebug)
	{
		for (FInventoryListItem& Item : InventoryList.GetItemsRef())
		{
			UInventoryItemInstance* ItemInstance = Item.ItemInstance;
			const UItemStaticData* ItemStaticData = ItemInstance->GetItemStaticData();

			if (IsValid(ItemInstance) && IsValid(ItemStaticData))
			{
				GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Blue, FString::Printf(TEXT("Ttem: %s"), *ItemStaticData->Name.ToString()));
			}
		}

		
		const TArray<FFastArrayTagCounterRecord>& InventoryTagArray = InventoryTags.GetTagArray();

		for (auto TagRecord : InventoryTagArray)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Purple, FString::Printf(TEXT("Tag: %s %d"), *TagRecord.Tag.ToString(), TagRecord.Count));
		}
		
	}
}

void UInventoryComponent::TraceForItems(UInventoryItemInstance* InItemInstance)
{
	//FVector HitLocation; //Maybe this need to be adjusted
	//FHitResult ItemTraceresult;
	//PerformTrace(ItemTraceresult, HitLocation);

	//if (ItemTraceresult.bBlockingHit)
	//{
	//}	//Test code
		/*
		if (ItemTraceresult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			//If they are not the same we found new Interactable
			if (ItemTraceresult.GetActor() != CurrentIntertactable)
			{
				FoundInteractable(ItemTraceresult.GetActor());
				return;
			}

			//if its the same we dont need to do anything
			if (ItemTraceresult.GetActor() == CurrentIntertactable)
			{
				return;
			}
		}
	}

	else
	{
		NoInteractableFound();
	}
	*/

}

bool UInventoryComponent::PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector2D VierportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(VierportSize);
	}
	FVector2D CrossHairWorldLocation{ VierportSize.X / 2.0f, VierportSize.Y / 2.0f }; //Divides the screen in  half to set crosshairs
	FVector CrossHairWorldDirection;
	FVector CrossHairWorldPosition;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrossHairWorldLocation,
		CrossHairWorldPosition,
		CrossHairWorldDirection);
	if (bScreenToWorld)
	{
		const FVector Start{ CrossHairWorldPosition }; //Trace from center of the Screen outwards
		const FVector End{ Start + CrossHairWorldDirection * 50'000.f };
		OutHitLocation = End;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Red,
			false, -1, 0,
			1.0f);
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, CurrentItem);
	DOREPLIFETIME(UInventoryComponent, InventoryTags);
	DOREPLIFETIME(UInventoryComponent, bCanTraceItemActorTag);
	DOREPLIFETIME(UInventoryComponent, TracedItemInstance);
}