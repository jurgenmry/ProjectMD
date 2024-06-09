// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PMCharacter.h"

//System includes
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"

//Custome includes

#include "GameModes/PMPlayerState.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"
#include "UI/HUD/PMBaseHud.h"
#include "Interfaces/InteractInterface.h"
#include "WorldActors/InteractableActorBase.h"
#include "WorldActors/PickUpBaseActor.h"


/*Inventory*/

#include "Inventory/InventoryComponent.h"

APMCharacter::APMCharacter()
	: EquippedItem(nullptr)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	/* Old Inventory Stuff*/
	//can be added on player state to avoid loosing items
	//CharacterInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("CharacterInventory"));
	//CharacterInventory->SetIsReplicated(true);
}

void APMCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		SetActorTickEnabled(true);	
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void APMCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItems();
}

void APMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void APMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//DOREPLIFETIME(APMCharacter, CharacterInventory);

	DOREPLIFETIME(APMCharacter, EquippedItem);
}

int32 APMCharacter::GetPlayerlevel()
{
	APMPlayerState* MainPlayerState = GetPlayerState<APMPlayerState>();
	if (MainPlayerState!= nullptr)
	{
		MainPlayerState->GetCurrentLvl();
	}
	return 1;
}


void APMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void APMCharacter::InitAbilityActorInfo()
{
	APMPlayerState* MainPlayerState = GetPlayerState<APMPlayerState>();
	if (MainPlayerState)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		//AbilitySystemComponent = Cast<UGDAbilitySystemComponent>(PS->GetAbilitySystemComponent());?? why casting
		

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
		AbilitySystemComponent = Cast<UPMBaseAbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent());
		AbilitySystemComponent->AbilityActorInfoSet();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSet = MainPlayerState->GetAttributeSetBase();


		//Custome code
		//AbilitySystemComponent->AbilityActorInfoSet();

		AttributeSet->SetHealth(100.0f);
		InitializeAttributes();
	
		//Get need the player controller to access the hud, so we need to cast to our own PC
		APMMainCharacterPlayerController* PC = Cast<APMMainCharacterPlayerController>(GetController());
		if (PC)
		{
			//We need to cast the result to our own hud
			APMBaseHud* BaseHud = Cast<APMBaseHud>(PC->GetHUD());
			if (BaseHud)
			{
				BaseHud->InitOverlay(PC, MainPlayerState, AbilitySystemComponent, AttributeSet);
			}

			/*
			if (CharacterInventory)
			{
				CharacterInventory->InitializeInventoryComponent(PC,MainPlayerState, AbilitySystemComponent);
			}
			*/
		}

		/* Custome code based on Trenak (Needs Review for implementation)

		//Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.

		InitializeAttributes();
		InitializeFloatingStatusBar();

		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.

		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

		// End respawn specific things

		AddStartupEffects();

		AddCharacterAbilities();

		*/

	}

}



/////////////////////////////////////////////////////////////////////
//
//                  CODE PART FOR INTERACTING
//
////////////////////////////////////////////////////////////////////


void APMCharacter::TraceForItems()
{
	if (CanOverlap)
	{
		FVector HitLocation; //Maybe this need to be adjusted
		FHitResult ItemTraceresult;
		PerformTrace(ItemTraceresult, HitLocation);

		if (ItemTraceresult.bBlockingHit)
		{   //Test code
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

			// Working
			/*
			if (ItemTraceresult.GetActor() != CurrentIntertactable)
			{
				IInteractInterface* interactableInterface = nullptr;

				interactableInterface = Cast<IInteractInterface>(ItemTraceresult.GetActor());
				//CurrentIntertactable = interactableInterface ? ItemTraceresult.GetActor() : nullptr;

				if (interactableInterface != nullptr)
				{
					CurrentIntertactable = ItemTraceresult.GetActor();
					return;
				}
				else
				{
					CurrentIntertactable = nullptr;
				}

			}
			*/

			/*
			if (ItemTraceresult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				//CurrentIntertactable = Cast<APickUpBaseActor>(ItemTraceresult.GetActor());
				if (ItemTraceresult.GetActor() != CurrentIntertactable)
				{
					CurrentIntertactable = ItemTraceresult.GetActor();
					return;
				}
			}
			*/
		}	
	}

	else
	{
		NoInteractableFound();
	}
}

bool APMCharacter::PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation)
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
		/*DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Red,
			false, -1, 0,
			1.0f);*/
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void APMCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndFocus();
	}
	
	if (CurrentIntertactable)
	{
		TargetInteractable = CurrentIntertactable;
		TargetInteractable->EndFocus();
	}

	CurrentIntertactable = NewInteractable;
	TargetInteractable = NewInteractable;
	TargetInteractable->BeginFocus();
}

void APMCharacter::NoInteractableFound()
{
	if (CurrentIntertactable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		CurrentIntertactable = nullptr;	
		TargetInteractable = nullptr;
	}
}


void APMCharacter::Interact()
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		if (HasAuthority())
		{
			TargetInteractable->Interact(this);
			//NoInteractableFound();
		}
		else
		{
			Server_Interact(TargetInteractable->_getUObject());
		}
	}
}

void APMCharacter::Server_Interact_Implementation(UObject* InterfaceContext)//IInteractInterface* Interface)
{
	if (IInteractInterface* interactInterface = Cast<IInteractInterface>(InterfaceContext))
	{
		interactInterface->Interact(this);
		NoInteractableFound();
	}
}



/////////////////////////////////////////////////////////////////////
//
//                  CODE PART FOR THE INVENTORY 
//
////////////////////////////////////////////////////////////////////

void APMCharacter::EquipItem()
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		if (HasAuthority())
		{
			TargetInteractable->EquipItem(this);
			
		}
		else
		{
			Server_EquipItem(this, TargetInteractable.GetObject());
		}

	}
}

bool APMCharacter::Server_EquipItem_Validate(APMCharacter* InCharacterOwner, UObject* InterfaceContext)
{
	// Add validation logic here if needed
	return true;
}

void APMCharacter::Server_EquipItem_Implementation(APMCharacter* InCharacterOwener, UObject* InterfaceContext)
{
	//InOwner = this;
	if (IInteractInterface* interactInterface = Cast<IInteractInterface>(InterfaceContext))
	{
		interactInterface->EquipItem(InCharacterOwener);
	}
}


void APMCharacter::DropItem()
{
	if (EquippedItem)
	{
		if (HasAuthority())
		{
			EquippedItem->DropItem(this);
			SetEquippedItem(nullptr);
		}
		else
		{
			Server_DropItem(this);
		}
	}
}


void APMCharacter::Server_DropItem_Implementation(APMCharacter* InCharacterOwener)
{
	EquippedItem->DropItem(InCharacterOwener);
	//SetEquippedItem(nullptr);
}


/*
void APMCharacter::EquipItem(APickUpBaseActor* ItemToEquip)
{
	if (HasAuthority())
	{
		if (ItemToEquip)
		{
			//Get the hand Socket of the character
			const USkeletalMeshSocket* HandSocket = GetMesh1P()->GetSocketByName(FName("ItemSocket"));
			if (HandSocket)
			{
				HandSocket->AttachActor(ItemToEquip, GetMesh1P());
			}
			EquippedItem = ItemToEquip;
			EquippedItem->SetItemState(EItemsState::EIS_Equipped);
		}
	}

	else
	{
		OnRep_EquipItem(ItemToEquip);
	}

}

void APMCharacter::OnRep_EquipItem(APickUpBaseActor* InItemToEquip)
{
	if (InItemToEquip)
	{
		//Get the hand Socket of the character
		const USkeletalMeshSocket* HandSocket = GetMesh1P()->GetSocketByName(FName("ItemSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(InItemToEquip, GetMesh1P());
		}
		EquippedItem = InItemToEquip;
		EquippedItem->SetItemState(EItemsState::EIS_Equipped);
	}
}
*/

