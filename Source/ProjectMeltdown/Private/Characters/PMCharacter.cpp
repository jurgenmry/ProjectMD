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
#include "AbilitySystemBlueprintLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

//Custome includes

#include "GameModes/PMPlayerState.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"
#include "UI/HUD/PMBaseHud.h"
#include "Components/PMMovementComponent.h"

// Input component
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "InputActionValue.h"

//Inventory

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "WorldActors/InteractableActorBase.h"


APMCharacter::APMCharacter(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	//: EquippedItem(nullptr)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);



	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


	//GetMesh()->SetupAttachment(FirstPersonCameraComponent);//Previous Semiworking

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	//Mesh1P->SetupAttachment(GetMesh()); // Previous semi working
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh3P"));
	Mesh3P->SetOnlyOwnerSee(false);
	Mesh3P->SetupAttachment(GetCapsuleComponent());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->ComponentTags.Add(FName("Inventory"));
	InventoryComponent->SetIsReplicated(true);


	StimuliSourceCharacter = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceCharacter"));
	StimuliSourceCharacter->bAutoRegister = true;


	// Initialize of Variables
	CrouchedMeshRelativeLocation = FVector(0.0f, 0.0f, -55.0f);
	UnCrouchedMeshRelativeLocation = FVector(0.0f,0.0f,-100.0f);
	GetMesh1P()->SetRelativeLocation(FVector(0.314146, 0.000000,-169.997533));
	InitialMesh1PRelativeLocation = GetMesh1P()->GetRelativeLocation();

	CrouchSpeed = 0.f;
	CrouchEyeOffset = FVector::ZeroVector;

	CurrentlyEquippedWeaponIndex = -1;
	
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

	float CrouchInterpTime = FMath::Min(1, CrouchSpeed * DeltaSeconds);
	CrouchEyeOffset = (1.0f - CrouchInterpTime) * CrouchEyeOffset;

	/*
	if(IsLocallyControlled() && Mesh1P)
	{
		FVector TargetLocation = GetCharacterMovement()->IsCrouching()
			? FVector(InitialMesh1PRelativeLocation.X, InitialMesh1PRelativeLocation.Y, InitialMesh1PRelativeLocation.Z)
			: FVector(InitialMesh1PRelativeLocation.X, InitialMesh1PRelativeLocation.Y, InitialMesh1PRelativeLocation.Z + 4);

		FVector CurrentLocation = FMath::VInterpTo(Mesh1P->GetRelativeLocation(), TargetLocation, DeltaSeconds, CrouchSpeed);
		Mesh1P->SetRelativeLocation(CurrentLocation);
	}
	*/
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

	//DOREPLIFETIME(APMCharacter, EquippedItem);
	DOREPLIFETIME(APMCharacter, InventoryComponent);//, COND_OwnerOnly);
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

void APMCharacter::Die()
{
	CharacterMulticastHandleDeath();
}

void APMCharacter::CharacterMulticastHandleDeath_Implementation()
{
	GetMesh1P()->SetSimulatePhysics(true);
	GetMesh3P()->SetSimulatePhysics(true);

	GetMesh1P()->SetEnableGravity(true);
	GetMesh3P()->SetEnableGravity(true);

	GetMesh1P()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh1P()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetMesh3P()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh3P()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APMCharacter::AdjustMeshOnCrouch(bool bCrouching)
{
	FVector NewLocation = bCrouching ? CrouchedMeshRelativeLocation : UnCrouchedMeshRelativeLocation;

	if (GetMesh3P())
	{
		GetMesh3P()->SetRelativeLocation(NewLocation);
	}
}

void APMCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.0f)
	{
		return;
	}
	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;
	AdjustMeshOnCrouch(true);
}

void APMCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.0f)
	{
		return;
	}

	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight - HalfHeightAdjust;
	AdjustMeshOnCrouch(false);
}

void APMCharacter::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	if (GetFirstPersonCameraComponent())
	{
		GetFirstPersonCameraComponent()->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void APMCharacter::Multicast_AdjustMeshForCrouch_Implementation(bool bCrouching)
{
	AdjustMeshOnCrouch(bCrouching);
}

void APMCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
	Multicast_AdjustMeshForCrouch(true);
}

void APMCharacter::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
	Multicast_AdjustMeshForCrouch(false);
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
		MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
		AbilitySystemComponent = Cast<UPMBaseAbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent());
		AbilitySystemComponent->AbilityActorInfoSet();
		AttributeSet = MainPlayerState->GetAttributeSetBase();

		InitializeAttributes();
		AddCharacterAbilities();
	
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
			if (InventoryComponent)
			{
				InventoryComponent->InitCustomeComponent(AbilitySystemComponent);
			}
		}
	}
}


// Inventory
UInventoryComponent* APMCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}


// Input

void APMCharacter::OnDropItemTriggered(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::DropItemTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::DropItemTag, EventPayload);
}

void APMCharacter::OnEquipNextTriggered(const FInputActionValue& Value)
{

	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipNextTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipNextTag, EventPayload);
}

void APMCharacter::OnUnequipTriggered(const FInputActionValue& Value)
{

	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::UnequipTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::UnequipTag, EventPayload);

	// Reset the currently equipped weapon index
	CurrentlyEquippedWeaponIndex = -1;
}

void APMCharacter::OnEquipItem(const FInputActionValue& Value)
{
	UInventoryComponent* IVComponent = GetInventoryComponent();
	if (IVComponent && IVComponent->bCanTraceItemActorTag && IVComponent->TracedItemInstance)
	{
		FGameplayEventData EventPayload;
		// Use the actor associated with the traced item as the instigator
		AInteractableActorBase* Ins = Cast<AInteractableActorBase>(IVComponent->TracedItemInstance->GetItemActor());
		EventPayload.Instigator = Ins;
		EventPayload.OptionalObject = IVComponent->TracedItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipItemActorTag, EventPayload);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Interact key pressed: Payload sent with OptionalObject: %s"), IVComponent->TracedItemInstance ? *IVComponent->TracedItemInstance->GetName() : TEXT("None")));
		
	}
}

void APMCharacter::OnEquipItem1Triggered(const FInputActionValue& Value)
{
	if (CurrentlyEquippedWeaponIndex == 0)
	{
		// If item 1 is already equipped, do nothing
		return;
	}

	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipItem1Tag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipItem1Tag, EventPayload);
	CurrentlyEquippedWeaponIndex = 0;
}

void APMCharacter::OnEquipItem2Triggered(const FInputActionValue& Value)
{
	if (CurrentlyEquippedWeaponIndex == 1)
	{
		// If item 2 is already equipped, do nothing
		return;
	}

	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipItem2Tag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipItem2Tag, EventPayload);
	CurrentlyEquippedWeaponIndex = 1;
}

void APMCharacter::OnEquipItem3Triggered(const FInputActionValue& Value)
{
	if (CurrentlyEquippedWeaponIndex == 2)
	{
		// If item 3 is already equipped, do nothing
		return;
	}

	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipItem3Tag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipItem3Tag, EventPayload);
	CurrentlyEquippedWeaponIndex = 2;
}

void APMCharacter::OnEquipItem4Triggered(const FInputActionValue& Value)
{
	if (CurrentlyEquippedWeaponIndex == 3)
	{
		// If item 4 is already equipped, do nothing
		return;
	}

	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipItem4Tag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipItem4Tag, EventPayload);

	CurrentlyEquippedWeaponIndex = 3;
}

void APMCharacter::OnCrouchActionStarted(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(CrouchTags, true);
	}
}

void APMCharacter::OnCrouchActionEnded(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{

		AbilitySystemComponent->CancelAbilities(&CrouchTags);
	}
}

void APMCharacter::OnAttackActionStarted(const FInputActionValue& Value)
{

	FGameplayEventData EventPayload;
	EventPayload.EventTag = AttackStartedEventTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AttackStartedEventTag, EventPayload);
}

void APMCharacter::OnAttackActionEnded(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = AttackEndedEventTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AttackEndedEventTag, EventPayload);

}

void APMCharacter::OnSprintActionStarted(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(SprintTags, true);
	}
}

void APMCharacter::OnSprintActionEnded(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&SprintTags);
	}
}



void APMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent)
	{
		if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if (EquipNextInputAction)
			{
				PlayerEnhancedInputComponent->BindAction(EquipNextInputAction, ETriggerEvent::Started, this, &APMCharacter::OnEquipNextTriggered);
			}

			if (DropItemInputAction)
			{
				PlayerEnhancedInputComponent->BindAction(DropItemInputAction, ETriggerEvent::Started, this, &APMCharacter::OnDropItemTriggered);
			}

			if (UnequipInputAction)
			{
				PlayerEnhancedInputComponent->BindAction(UnequipInputAction, ETriggerEvent::Started, this, &APMCharacter::OnUnequipTriggered);
			}

			if (EquipItem)
			{
				PlayerEnhancedInputComponent->BindAction(EquipItem, ETriggerEvent::Started, this, &APMCharacter::OnEquipItem);
			}

			if (EquipItem1InputAction)
			{
				PlayerEnhancedInputComponent->BindAction(EquipItem1InputAction, ETriggerEvent::Started, this, &APMCharacter::OnEquipItem1Triggered);
			}

			if (EquipItem2InputAction)
			{
				PlayerEnhancedInputComponent->BindAction(EquipItem2InputAction, ETriggerEvent::Started, this, &APMCharacter::OnEquipItem2Triggered);
			}

			if (EquipItem3InputAction)
			{
				PlayerEnhancedInputComponent->BindAction(EquipItem3InputAction, ETriggerEvent::Started, this, &APMCharacter::OnEquipItem3Triggered);
			}

			if (EquipItem4InputAction)
			{
				PlayerEnhancedInputComponent->BindAction(EquipItem4InputAction, ETriggerEvent::Started, this, &APMCharacter::OnEquipItem4Triggered);
			}

			if (CrouchInputAction)
			{
				PlayerEnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &APMCharacter::OnCrouchActionStarted);
				PlayerEnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &APMCharacter::OnCrouchActionEnded);
			}

			if (SprintInputAction)
			{
				PlayerEnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Triggered, this, &APMCharacter::OnSprintActionStarted);
				PlayerEnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &APMCharacter::OnCrouchActionEnded);
			}

			if (AttackInputAction)
			{
				PlayerEnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Started, this, &APMCharacter::OnAttackActionStarted);
				PlayerEnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Completed, this, &APMCharacter::OnSprintActionEnded);
			}
		}
	}

}



// DATA


FCharacterAnimationData APMCharacter::GetCharacterData3P() const
{
	return CharacterAnimDataData3P;
}

void APMCharacter::SetCharacterData3P(const FCharacterAnimationData& InCharacterData)
{
	CharacterAnimDataData3P = InCharacterData;

	//InitFromCharacterData(CharacterData);
}

FCharacterAnimationData APMCharacter::GetCharacterData1FP() const
{
	return CharacterAnimDataData1FP;
}

void APMCharacter::SetCharacterData1FP(const FCharacterAnimationData& InCharacterData)
{
	CharacterAnimDataData1FP = InCharacterData;
}

/*
void APMCharacter::FinishDying()
{

}
*/





/*
void APMCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}
*/

/*
void APMCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication)
{

}
*/







//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                MOST OF THIS CODE WAS WORKING. COMMENTD TO IMPLEMENT NEW INVENTORY
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////







/////////////////////////////////////////////////////////////////////
//
//                  CODE PART FOR INTERACTING
//
////////////////////////////////////////////////////////////////////

/*
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
		}	
	}

	else
	{
		NoInteractableFound();
	}
}
*/

/*
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
}*/

/*
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
}*/

/*
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
*/

/*
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
}*/

/*
void APMCharacter::Server_Interact_Implementation(UObject* InterfaceContext)//IInteractInterface* Interface)
{
	if (IInteractInterface* interactInterface = Cast<IInteractInterface>(InterfaceContext))
	{
		interactInterface->Interact(this);
		NoInteractableFound();
	}
}
*/



/////////////////////////////////////////////////////////////////////
//
//                  CODE PART FOR THE INVENTORY 
//
////////////////////////////////////////////////////////////////////


//New Testing code: Working Huyeeaahhh !

//Equip An Item 
/*
void APMCharacter::EquipItem()
{

	if (IsValid(TargetInteractable.GetObject()))
	{
		APickUpBaseActor* ItemToEquip = Cast<APickUpBaseActor>(TargetInteractable.GetObject());
		if (ItemToEquip)
		{
			if (HasAuthority())
			{
				Server_EquipItem(this, TargetInteractable.GetObject()); 
			}
			else
			{
				Server_EquipItem(this, TargetInteractable.GetObject());
			}
		}
	}

}
*/

/*
bool APMCharacter::Server_EquipItem_Validate(APMCharacter* InCharacterOwner, UObject* InterfaceContext) 
{
	return true;
}
*/
 
/*
void APMCharacter::Server_EquipItem_Implementation(APMCharacter* InCharacterOwner, UObject* InterfaceContext) 
{
	
	//v3 Working Properly replicated. Do not work with input
	if (IInteractInterface* interactInterface = Cast<IInteractInterface>(InterfaceContext))
	{
		APickUpBaseActor* ItemToEquip = Cast<APickUpBaseActor>(InterfaceContext);
		if (ItemToEquip)
		{
			if (InCharacterOwner->EquippedItem != ItemToEquip)
			{
				if (InCharacterOwner->Inventory.Num() < INVENTORY_CAPACITY)
				{
					if (InCharacterOwner->EquippedItem)
					{
						InCharacterOwner->Inventory.Add(InCharacterOwner->EquippedItem);
						InCharacterOwner->EquippedItem->SetItemState(EItemsState::EIS_PickedUP);
					}
					ItemToEquip->EquipItem(InCharacterOwner);
				}
				else // Inventory is full
				{
					Server_SwapItem(ItemToEquip);
				}
			}
			else
			{
				interactInterface->EquipItem(InCharacterOwner);
			}
		}
	}
}
*/

//Dropping An Item
/*
void APMCharacter::DropItem()
{
	if (EquippedItem)
	{
		if (HasAuthority())
		{
			Server_DropItem(this);
			// working on server SetEquippedItem(nullptr);
		}
		else
		{
			Server_DropItem(this);
		}
	}	
}
*/

/*
void APMCharacter::Server_DropItem_Implementation(APMCharacter* InCharacterOwner)
{
	
	if (EquippedItem)
	{
		EquippedItem->DropItem(this);
		EquippedItem = nullptr;
	}
}
*/


// Swapping An Item 
/*
void APMCharacter::SwapItem(APickUpBaseActor* ItemToSwap)
{

	if (HasAuthority())
	{
		Server_SwapItem(ItemToSwap);
	}
	else
	{
		Server_SwapItem(ItemToSwap);
	}
}*/

/*
void APMCharacter::Server_SwapItem_Implementation(APickUpBaseActor* ItemToSwap)//v1 APMCharacter* InCharacterOwner, APickUpBaseActor* ItemToSwap)
{

	if (EquippedItem)
	{
		EquippedItem->DropItem(this);
	}
	EquippedItem = ItemToSwap;
	ItemToSwap->EquipItem(this);
}*/


// Exchange Inventory Based on Key press 
/*
void APMCharacter::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex)
{

	if (CurrentItemIndex == NewItemIndex || NewItemIndex >= Inventory.Num() || !Inventory.IsValidIndex(NewItemIndex))
	{
		return;
	}

	APickUpBaseActor* OldEquippedItem = EquippedItem;
	APickUpBaseActor* NewItem = Inventory[NewItemIndex];

	if (OldEquippedItem)
	{
		OldEquippedItem->SetItemState(EItemsState::EIS_PickedUP);
		OldEquippedItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Inventory[CurrentItemIndex] = OldEquippedItem;
	}

	if (NewItem)
	{
		NewItem->SetItemState(EItemsState::EIS_Equipped);
		NewItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandSocket"));
		EquippedItem = NewItem;
		Inventory[NewItemIndex] = nullptr;
	}

	if (!HasAuthority())
	{
		Server_ExchangeInventoryItems(CurrentItemIndex, NewItemIndex);
	}

	//Sort of working.
	//
	//if ((CurrentItemIndex == NewItemIndex) || NewItemIndex >= Inventory.Num()) return;
	//
	//	auto OldEquippedItem = EquippedItem;
	//auto NewItem = Cast<APickUpBaseActor>(Inventory[NewItemIndex]);
	//
	//if (OldEquippedItem)
	//{
	//	OldEquippedItem->SetItemState(EItemsState::EIS_PickedUP);
	//	Inventory[CurrentItemIndex] = OldEquippedItem;
	//}
	//
	//NewItem->SetItemState(EItemsState::EIS_Equipped);
	//EquippedItem = NewItem;
	//
	//Inventory[NewItemIndex] = nullptr; // Optional, to remove the reference
	//if (!HasAuthority())
	//{
	//	Server_ExchangeInventoryItems(CurrentItemIndex, NewItemIndex);
	//}
	//
//}
*/

/*
void APMCharacter::Server_ExchangeInventoryItems_Implementation(int32 CurrentItemIndex, int32 NewItemIndex)
{
	ExchangeInventoryItems(CurrentItemIndex, NewItemIndex);
}
*/

/*void APMCharacter::OneKeyPressedCharacter()
{
	if (EquippedItem && EquippedItem->GetSlotIndex() == 0) return;
	if (Inventory.IsValidIndex(0) && Inventory[0])
	{
		ExchangeInventoryItems(EquippedItem ? EquippedItem->GetSlotIndex() : -1, 0);
	}
}

void APMCharacter::TwoKeyPressedCharacter()
{
	if (EquippedItem && EquippedItem->GetSlotIndex() == 1) return;
	if (Inventory.IsValidIndex(1) && Inventory[1])
	{
		ExchangeInventoryItems(EquippedItem ? EquippedItem->GetSlotIndex() : -1, 1);
	}
}

void APMCharacter::ThreeKeyPressedCharacter()
{
	if (EquippedItem && EquippedItem->GetSlotIndex() == 2) return;
	if (Inventory.IsValidIndex(2) && Inventory[2])
	{
		ExchangeInventoryItems(EquippedItem ? EquippedItem->GetSlotIndex() : -1, 2);
	}
}

void APMCharacter::FourKeyPressedCharacter()
{
	if (EquippedItem && EquippedItem->GetSlotIndex() == 3) return;
	if (Inventory.IsValidIndex(3) && Inventory[3])
	{
		ExchangeInventoryItems(EquippedItem ? EquippedItem->GetSlotIndex() : -1, 3);
	}
}*/





/*Working Except for the last 2 fuctions*/

/*
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

// Not working
void APMCharacter::SwapItem(APickUpBaseActor* ItemToSwap)
{
	ItemToSwap->DropItem(this);
	ItemToSwap->EquipItem(this);
}

void Server_SwapItem_Implementation(APMCharacter* InCharacterOwner, APickUpBaseActor* ItemToSwap)
{
	Server_DropItem(InCharacterOwner);
	Server_EquipItem(InCharacterOwner, TargetInteractable.GetObject());
}
*/




/////////////////////////////////////////////////////////////////////


//Old code
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

