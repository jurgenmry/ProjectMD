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

// Sets default values
AInteractableActorBase::AInteractableActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereOverlap);
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

		const float droppItemDist = 100.f;
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
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventPayload);
	}
}

bool AInteractableActorBase::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);

	return WroteSomething;
}

// Called when the game starts or when spawned
void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass, Quantity);

			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SphereComponent->SetGenerateOverlapEvents(true);

			InitInternal();
		}
	}
}

void AInteractableActorBase::InitInternal()
{

}

// Called every frame
void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableActorBase, ItemInstance);
	DOREPLIFETIME(AInteractableActorBase, ItemState);
	DOREPLIFETIME(AInteractableActorBase, Quantity);
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

