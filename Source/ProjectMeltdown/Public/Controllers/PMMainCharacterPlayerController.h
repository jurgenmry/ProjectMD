// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PMMainCharacterPlayerController.generated.h"



UCLASS()
class PROJECTMELTDOWN_API APMMainCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 

	APMMainCharacterPlayerController();

	virtual void BeginPlay() override;

private: 

	/*Input Mapping Context*/
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PMContext;


	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> JumpAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> InteractAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> DropItemAction;


	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> DropItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> EquipNextItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UInputAction> UnEquipItemAction;
	*/

protected:

	virtual void SetupInputComponent() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/*Jump*/
	void Jump();

	/*Interact*/
	void InteractWithObject();

	void DropEquippedItem();

	/*
	void OnDropItemAction(const FInputActionValue& Value);
	void OnEquipNextItemAction(const FInputActionValue& Value);
	void OnUnEquipItemAction(const FInputActionValue& Value);
	*/


	// Server only
	//virtual void OnPossess(APawn* InPawn) override;

	//virtual void OnRep_PlayerState() override;

	
};
