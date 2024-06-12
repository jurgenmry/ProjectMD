// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/PickUpBaseActor.h"


#include "Inventory/InventoryItemInstance.h"
#include "Inventory/ItemStaticData.h"
#include "Components/WidgetComponent.h"
#include "Inventory/InventoryComponent.h"


#include "Engine/ActorChannel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"





//System imncludes: 
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"

//Custome includes
#include "Characters/PMCharacter.h"


/*
APickUpBaseActor::APickUpBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; 

	SetReplicateMovement(true);
	//SetPhysicsReplicationMode(EPhysicsReplicationMode::Default);



	//GetBoxComps()->SetSimulatePhysics(true);
}
*/

/*void APickUpBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APickUpBaseActor, ItemState);
	DOREPLIFETIME(APickUpBaseActor, bItemFalling);
	DOREPLIFETIME(APickUpBaseActor, SlotItemIndex);

}*/
/*
void APickUpBaseActor::BeginPlay()
{
	Super::BeginPlay();
	SetItemProperties(ItemState);
}*/

//void APickUpBaseActor::Tick(float DeltaTime)
//{
	//Super::Tick(DeltaTime);
//
//	/*
//	if (GetItemState() == EItemsState::EIS_Falling && bItemFalling)
//	{
//		if(HasAuthority())
//		{
//
//		}
//	}
//	*/
//}


//void APickUpBaseActor::Interact(APMCharacter* InInteractOwner)
//{
//	Super::Interact(InInteractOwner);
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Entering actor interactable")), 1);
//
//	if (InInteractOwner)
//	{
//		if (InInteractOwner->IsLocallyControlled())
//		{
//			//UI Stuff
//		}
//		if (HasAuthority())
//		{
//			//InInteractOwner->AddItemToInventory(this);
//			//InInteractOwner->EquipItem(this);
//			//Destroy();
//		}
//	}
//}

//void APickUpBaseActor::SetItemProperties(EItemsState State)
//{
//	switch (State)
//	{
//	case EItemsState::EIS_Pickup:
//		
//		/*Set Mesh Properties*/
//		GetItemMesh()->SetSimulatePhysics(false);
//		GetItemMesh()->SetEnableGravity(false);
//		GetItemMesh()->SetVisibility(true);
//		GetItemMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		/*Area Sphere properties*/
//		GetSphereComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
//		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//
//		/*Box Properties properties*/
//		GetBoxComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetBoxComps()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
//		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//
//		break;
//
//	case EItemsState::EIS_EquipInterping:
//		break;
//
//	case EItemsState::EIS_PickedUP:
//
//		/*Set Mesh Properties*/
//		GetItemMesh()->SetSimulatePhysics(false);
//		GetItemMesh()->SetEnableGravity(false);
//		GetItemMesh()->SetVisibility(false);
//		GetItemMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		/*Area Sphere properties*/
//		GetSphereComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		/*Box Properties properties*/
//		GetBoxComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//
//		break;
//
//	case EItemsState::EIS_Equipped:
//
//		/*Set Mesh Properties*/
//		GetItemMesh()->SetSimulatePhysics(false);
//		GetItemMesh()->SetEnableGravity(false);
//		GetItemMesh()->SetVisibility(true);
//		GetItemMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		/*Area Sphere properties*/
//		GetSphereComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		/*Box Properties properties*/
//		GetBoxComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		break;
//
//	case EItemsState::EIS_Falling:
//
//		/*Set Mesh Properties*/
//
//		GetItemMesh()->SetSimulatePhysics(true);
//		GetItemMesh()->SetEnableGravity(true);
//
//		GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//		GetItemMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetItemMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
//
//		
//		
//		/*Area Sphere properties*/
//		GetSphereComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		/*Box Properties properties*/
//		//GetBoxComps()->SetSimulatePhysics(true);
//		GetBoxComps()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		break;
//
//	case EItemsState::EIS_MAX:
//		break;
//
//	default:
//		break;
//	}
//}

//void APickUpBaseActor::SetItemState(EItemsState State)
//{
//	ItemState = State;
//	OnRep_ItemState();
//}

//void APickUpBaseActor::OnRep_ItemState()
//{
//	SetItemProperties(ItemState);
//}

//void APickUpBaseActor::PickUpItem()
//{
//	//check the array
//	//If bigger than array amount then swap
//	//Other wise pick up the item 
//}

//void APickUpBaseActor::SwapItem()
//{
//	//If Inventory full 
//	//Drop current and Equip new
//}


//////////////////////////////////////
/////
/////           PICK UP ITEM
////
//////////////////////////////////////


//void APickUpBaseActor::EquipItem(APMCharacter* InPickupOwner)
//{
//	if (InPickupOwner)
//	{
//		if(InPickupOwner->HasAuthority())
//		{
//			StartPickUp(InPickupOwner);
//			SetItemState(EItemsState::EIS_Equipped);
//		}
//
//		else
//		{
//			Server_EquipItemActor_Implementation(InPickupOwner); //Server_EquipItemActor(InPickupOwner); //
//		}
//	}
//}

//void APickUpBaseActor::StartPickUp(APMCharacter* InPickUpOwner)
//{
//	const USkeletalMeshSocket* HandSocket = InPickUpOwner->GetMesh1P()->GetSocketByName(FName("ItemSocket"));
//	if (HandSocket)
//	{
//		//GetRootComponent()->AttachToComponent(InPickUpOwner->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("ItemSocket"));
//		
//		HandSocket->AttachActor(this, InPickUpOwner->GetMesh1P());
//	}
//	
//	InPickUpOwner->SetEquippedItem(this);
//}

//bool APickUpBaseActor::Server_EquipItemActor_Validate(APMCharacter* InPickUpOwner)
//{
//	return true;
//}

//void APickUpBaseActor::Server_EquipItemActor_Implementation(APMCharacter* InPickUpOwner)
//{
//	StartPickUp(InPickUpOwner);
//}

//////////////////////////////////////
/////
/////           DROP ITEM
////
//////////////////////////////////////

//void APickUpBaseActor::DropItem(APMCharacter* InPickUpOwner)
//{
//	if (InPickUpOwner)
//	{
//		if (InPickUpOwner->HasAuthority())
//		{
//			StartDropItem();
//			//SetItemState(EItemsState::EIS_Falling);
//		}
//
//		else
//		{
//			Server_DropItem_Implementation();
//		}
//	}
//}

//void APickUpBaseActor::StartDropItem()
//{
//	//On input button pressed 
//	//Drop the item based on current subIndex
//	// OPTION 1
//	/*GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//	SetItemState(EItemsState::EIS_Falling);
//
//	FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
//	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
//	const FVector Forwardmesh = GetItemMesh()->GetForwardVector();
//	const FVector RightMesh = GetItemMesh()->GetRightVector();
//
//	//Direction on which throw weapon
//	FVector ImpulseDirection = RightMesh.RotateAngleAxis(-20.0f, Forwardmesh);
//
//	float RandomRotation = FMath::FRandRange(0.0f, 90.0f);
//	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
//
//	ImpulseDirection *= ImpulseThrow;
//
//	GetItemMesh()->AddImpulse(ImpulseDirection);
//
//	if (HasAuthority())
//	{
//		GetWorldTimerManager().SetTimer(ThrowItemHandle, this, &APickUpBaseActor::StopFalling, ThrowItemTime);
//	}
//	*/
//
//
//	//OPTION2
//	//GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//	SetItemState(EItemsState::EIS_Falling);
//
//	if (APMBaseCharacter* ActorOwner = Cast<APMBaseCharacter>(this->GetAttachParentActor()))
//	{
//		const FVector Location = GetActorLocation();
//		const FVector Forward = ActorOwner->GetActorForwardVector();
//
//		const float DropItemDistance = ImpulseThrow;
//		const float DropItemTraceDistance = 10.0f;
//
//		const FVector TraceStart = Location + Forward * DropItemDistance;
//		const FVector TraceEnd = TraceStart - FVector::UpVector * DropItemTraceDistance;
//
//		TArray<AActor*> ActorsToIgnore = { this->GetAttachParentActor() };
//		FHitResult HitResult;
//		FVector TargetLocation = TraceEnd;
//
//		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
//		{
//			if (HitResult.bBlockingHit)
//			{
//				TargetLocation = HitResult.Location;
//				GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//				this->SetActorLocation(HitResult.Location);
//			}
//			else
//			{
//				GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//				this->SetActorLocation(TraceEnd);
//			}
//		};
//
//		GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//		this->SetActorLocation(TraceEnd);
//		if (HasAuthority())
//		{
//			GetWorldTimerManager().SetTimer(ThrowItemHandle, this, &APickUpBaseActor::StopFalling, ThrowItemTime);
//		}
//	}
//}

//void APickUpBaseActor::Server_DropItem_Implementation()
//{
//	StartDropItem();
//}


//void APickUpBaseActor::StopFalling()
//{
//	SetItemState(EItemsState::EIS_Pickup);
//
//}

//void APickUpBaseActor::Server_StopFalling_Implementation()
//{
//	StopFalling();
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                   //
// =============================== OLD INVENTORY FUNCTIONALIDY ===================================== //
//                                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void APickUpBaseActor::BeginPlay()
{
	Super::BeginPlay();

	
	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass);

			GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetSphereComps()->SetGenerateOverlapEvents(true);
			GetBoxComps()->SetGenerateOverlapEvents(true);
		}
	}
	

	//SphereComps->OnComponentBeginOverlap.AddDynamic(this, &APickUpBaseActor::OnSphereOverlap);
}


bool APickUpBaseActor::ReplicateSubObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	
	return bWroteSomething;
}


void APickUpBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(APickUpBaseActor, ItemInstance);
	DOREPLIFETIME(APickUpBaseActor, CurrentItemState);
}


void APickUpBaseActor::InitActor(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}


void APickUpBaseActor::Interact(APMCharacter* CharacterReference)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Inter")));

	//Super::Interact(CharacterReference);
	if (CharacterReference && HasAuthority())
	{
		
		AActor* OtherActor = Cast<AActor>(CharacterReference);

		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventPayload);
	}
}


void APickUpBaseActor::OnEquipped()
{
	CurrentItemState = EItemState::Equipped;
	GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSphereComps()->SetGenerateOverlapEvents(false);
	GetBoxComps()->SetGenerateOverlapEvents(false);
}

void APickUpBaseActor::OnUnEquipped()
{
	CurrentItemState = EItemState::UnEquipped;
	GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickUpBaseActor::OnDropped()
{
	CurrentItemState = EItemState::Dropped;

	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector Forward = ActorOwner->GetActorForwardVector();

		const float DropItemDistance = 150.0f;
		const float DropItemTraceDistance = 10000.0f;

		const FVector TraceStart = Location + Forward* DropItemDistance;
		const FVector TraceEnd = TraceStart - FVector::UpVector * DropItemTraceDistance;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };
		FHitResult HitResult;
		FVector TargetLocation = TraceEnd;


		
		if (UKismetSystemLibrary::LineTraceSingleByProfile(this,TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, EDrawDebugTrace::Persistent, HitResult,true))
		{
			if (HitResult.bBlockingHit)
			{
				TargetLocation = HitResult.Location;
				//SetActorLocation(HitResult.Location);
			}
		};
		
		SetActorLocation(TraceEnd);

		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetSphereComps()->SetGenerateOverlapEvents(true);
		GetBoxComps()->SetGenerateOverlapEvents(true);
	};
}

void APickUpBaseActor::OnRep_ItemState()
{
	switch (CurrentItemState)
	{
	case EItemState::None:
		break;
	case EItemState::Equipped:

		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetSphereComps()->SetGenerateOverlapEvents(false);
		GetBoxComps()->SetGenerateOverlapEvents(false);

		break;
	case EItemState::UnEquipped:
		break;
	case EItemState::Dropped:
		break;
	default:

		GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetSphereComps()->SetGenerateOverlapEvents(true);
		GetBoxComps()->SetGenerateOverlapEvents(true);

		break;
	}
}



void APickUpBaseActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIntex, bFromSweep, SweepResult);

	
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventPayload);
	}
	
}*/




/*
	const FVector Location = GetActorLocation();
	const FVector Forward = GetOwner()->GetActorForwardVector();

	const float DropItemDistance = 150.0f;
	const float DropItemTraceDistance = 10000.0f;

	const FVector TraceStart = Location + Forward * DropItemDistance;
	const FVector TraceEnd = TraceStart - FVector::UpVector * DropItemTraceDistance;

	TArray<AActor*> ActorsToIgnore = { GetOwner() };
	FHitResult HitResult;
	FVector TargetLocation = TraceEnd;

	if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, EDrawDebugTrace::Persistent, HitResult, true))
	{
		if (HitResult.bBlockingHit)
		{
			TargetLocation = HitResult.Location;
			SetActorLocation(HitResult.Location);
		}
	};

	SetActorLocation(TraceEnd);


	GetSphereComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetBoxComps()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetSphereComps()->SetGenerateOverlapEvents(true);
	GetBoxComps()->SetGenerateOverlapEvents(true);
	*/