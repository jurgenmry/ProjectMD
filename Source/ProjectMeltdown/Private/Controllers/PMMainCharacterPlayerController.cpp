// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PMMainCharacterPlayerController.h"


//System includes
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

//Custome Includes
#include "Characters/PMCharacter.h"
#include "Inventory/InventoryComponent.h"


APMMainCharacterPlayerController::APMMainCharacterPlayerController()
{
	bReplicates = true;
}

void APMMainCharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PMContext, 0);
	}

}

void APMMainCharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APMMainCharacterPlayerController::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APMMainCharacterPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APMMainCharacterPlayerController::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APMMainCharacterPlayerController::InteractWithObject);

		/*
		//EquipItem
		EnhancedInputComponent->BindAction(UnEquipItemAction, ETriggerEvent::Started, this, &APMMainCharacterPlayerController::OnUnEquipItemAction);

		//DropItem
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &APMMainCharacterPlayerController::OnDropItemAction);

		//Equipnext
		EnhancedInputComponent->BindAction(EquipNextItemAction, ETriggerEvent::Started, this, &APMMainCharacterPlayerController::OnEquipNextItemAction);
		*/	
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APMMainCharacterPlayerController::Move(const FInputActionValue& Value)
{


	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(FowardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void APMMainCharacterPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void APMMainCharacterPlayerController::Jump()
{
	ACharacter* ControlledCharacter = GetPawn<APMCharacter>();
	if (!IsValid(ControlledCharacter))
		return;
	ControlledCharacter->Jump();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Pressing Interaction")));
}

void APMMainCharacterPlayerController::InteractWithObject()
{
	APMCharacter* ControlledCharacter = Cast<APMCharacter>(GetCharacter());//GetPawn<APMCharacter>();
	if (!IsValid(ControlledCharacter))
		return;
	ControlledCharacter->EquipItem();//Interact();
}



/*
void APMMainCharacterPlayerController::OnDropItemAction(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::DropItemTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::DropItemTag, EventPayload);
}

void APMMainCharacterPlayerController::OnEquipNextItemAction(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipNextTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipNextTag, EventPayload);
}

void APMMainCharacterPlayerController::OnUnEquipItemAction(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::UnEquipTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::UnEquipTag, EventPayload);
}
*/
