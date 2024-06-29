// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/InteractableActorBase.h"


//System includes:

#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BillboardComponent.h"

//Custome includes
#include "Controllers/PMMainCharacterPlayerController.h"
#include "Characters/PMCharacter.h"


/*
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"

//Custome includes
#include "UI/Widgets/PMBaseUserWidget.h" 
#include "Characters/PMCharacter.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "UI/HUD/PMBaseHud.h"
*/

AInteractableActorBase::AInteractableActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	//SceneR = CreateDefaultSubobject<UBillboardComponent>(TEXT("SceneR"));
	//SetRootComponent(SceneR);
	//SceneR->SetMobility(EComponentMobility::Movable);
	//SceneR->SetIsReplicated(true);


	BaseSK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseSK"));
	SetRootComponent(BaseSK);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	SphereComponent->SetupAttachment(BaseSK);
	//SphereComponent->SetIsReplicated(true);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());

	

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereOverlapEnd);
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass, Quantity);

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ItemInstance created: %s"), *ItemInstance->GetName()));
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SphereComponent->SetGenerateOverlapEvents(true);

			InitInternal();
		}
	}

	if (ItemInstance && ItemInstance->GetItemStaticData()->ItemWidgetClass)
	{
		InteractWidget->SetWidgetClass(ItemInstance->GetItemStaticData()->ItemWidgetClass);/// CreateWidget<UUserWidget>(GetWorld(), );
		if (InteractWidget)
		{
			InteractWidget->SetVisibility(false);
		}
	}
}

void AInteractableActorBase::InitInternal()
{

}



void AInteractableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableActorBase, ItemInstance);
	DOREPLIFETIME(AInteractableActorBase, ItemState);
	DOREPLIFETIME(AInteractableActorBase, Quantity);
}


void AInteractableActorBase::Init(UInventoryItemInstance* InInstance)
{
	ItemInstance = InInstance;
	InitInternal();
}

void AInteractableActorBase::OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance)
{
	if (IsValid(ItemInstance) && !IsValid(OldItemInstance))
	{
		InitInternal();
	}
}

void AInteractableActorBase::OnEquipped()
{
	ItemState = EItemState::Equipped;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);

}

void AInteractableActorBase::OnUnequipped()
{
	ItemState = EItemState::None;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

void AInteractableActorBase::OnDropped()
{
	ItemState = EItemState::Dropped;

	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector Forward = ActorOwner->GetActorForwardVector();

		const float droppItemDist = 200.f;
		const float droppItemTraceDist = 10000.f;

		const FVector TraceStart = Location + Forward * droppItemDist;
		const FVector TraceEnd = TraceStart - FVector::UpVector * droppItemTraceDist;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };

		FHitResult TraceHit;

		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;

		FVector TargetLocation = TraceEnd;

		EDrawDebugTrace::Type DebugDrawType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true, ActorsToIgnore, DebugDrawType, TraceHit, true))
		{
			if (TraceHit.bBlockingHit)
			{
				TargetLocation = TraceHit.Location;
			}
		}

		SetActorLocation(TargetLocation);

		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
	}
}

void AInteractableActorBase::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::Equipped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;
	default:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		break;
	}
}

void AInteractableActorBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor != this && OtherActor)
	{	
		APMCharacter* Character = Cast<APMCharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled())
		{
			InteractWidget->SetVisibility(true);
		}
	}
	
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventPayload);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Sphere overlap: Payload sent to %s with ItemInstance: %s"), *OtherActor->GetName(), ItemInstance ? *ItemInstance->GetName() : TEXT("None")));
		//UE_LOG(LogTemp, Log, TEXT("Sphere overlap: Payload sent to %s with ItemInstance: %s"), *OtherActor->GetName(), ItemInstance ? *ItemInstance->GetName() : TEXT("None"));
	}
}

void AInteractableActorBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	
	APMCharacter* Character = Cast<APMCharacter>(OtherActor);
	if (Character && Character->IsLocallyControlled())
	{
		InteractWidget->SetVisibility(false);
	}
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::RemoveCanTraceItemActorTag;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::RemoveCanTraceItemActorTag, EventPayload);
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Sphere end overlap: Payload sent to %s with ItemInstance: %s"), *OtherActor->GetName(), ItemInstance ? *ItemInstance->GetName() : TEXT("None")));
		//UE_LOG(LogTemp, Log, TEXT("Sphere end overlap: Payload sent to %s with ItemInstance: %s"), *OtherActor->GetName(), ItemInstance ? *ItemInstance->GetName() : TEXT("None"));

	}
}

bool AInteractableActorBase::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);

	return WroteSomething;
}









//Old Implementation
/*
AInteractableActorBase::AInteractableActorBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	SetRootComponent(PickUpMesh);
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickUpMesh->SetIsReplicated(true);
	PickUpMesh->SetSimulatePhysics(false);

	BoxComps = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComps"));
	BoxComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComps->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxComps->SetupAttachment(GetRootComponent());
	BoxComps->SetHiddenInGame(true);
	BoxComps->SetIsReplicated(true);

	SphereComps = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComps"));
	SphereComps->SetSphereRadius(70.0f);
	SphereComps->SetupAttachment(GetRootComponent());
	SphereComps->SetHiddenInGame(true);


	InteractW = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractW"));
	InteractW->SetupAttachment(BoxComps);
}


void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComps->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereOverlap);
	SphereComps->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereEndOverlap);
	InteractW->SetVisibility(false);
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractableActorBase::BeginFocus()
{
	InteractW->SetVisibility(true);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Begin Focus")), 1);
}


void AInteractableActorBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor)
	{
		APMCharacter* Character = Cast<APMCharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled())
		{
			Character->SetOverlappingActors(true);
			BeginFocus();
	
		}
	}
	//FString Overlapping = "Overlaping Interactable";
	//GEngine->AddOnScreenDebugMessage(10, 3.0f, FColor::Red, Overlapping, 1);
}

void AInteractableActorBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex)
{
	if (OtherActor != this && OtherActor)
	{
		
		APMCharacter* Character = Cast<APMCharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled())
		{
			Character->SetOverlappingActors(false);
			EndFocus();
			//Character->//HUD->GetInteractionWidget()->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AInteractableActorBase::EndFocus()
{
	InteractW->SetVisibility(false);
}

void AInteractableActorBase::Interact(APMCharacter* InInteractOwner)
{
	//EndFocus();
}

void AInteractableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableActorBase, PickUpMesh);
}



//void AInteractableActorBase::Interact_Implementation(const APMCharacter* InInteractOwner)
//{
//	EndFocus();
//}

*/

