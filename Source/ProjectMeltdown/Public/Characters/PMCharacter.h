// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PMBaseCharacter.h"
#include "InputActionValue.h"

#include "Inventory/ItemStaticData.h"
#include "PMCharacter.generated.h"


class USkeletalMeshComponent;
class UCameraComponent;
class UInventoryComponent;

UCLASS()
class PROJECTMELTDOWN_API APMCharacter : public APMBaseCharacter
{
	GENERATED_BODY()


	APMCharacter();


public:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	virtual int32 GetPlayerlevel() override;


	// Action Tags
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CrouchTags;


	// Inventory 
	UInventoryComponent* GetInventoryComponent() const;

	//Input mappings
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* DropItemInputAction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipNextInputAction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* UnequipInputAction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipItem; //CD

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipItem1InputAction;    //CD

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipItem2InputAction;  //CD

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipItem3InputAction;  //CD

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* EquipItem4InputAction;  //CD

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* CrouchInputAction;


	UPROPERTY(EditDefaultsOnly)
	class UInputAction* AttackInputAction;


	// Input actions: 

	void OnDropItemTriggered(const FInputActionValue& Value);

	void OnEquipNextTriggered(const FInputActionValue& Value);

	void OnUnequipTriggered(const FInputActionValue& Value);


	void OnEquipItem(const FInputActionValue& Value); //CD
	void OnEquipItem1Triggered(const FInputActionValue& Value); //CD
	void OnEquipItem2Triggered(const FInputActionValue& Value); //CD
	void OnEquipItem3Triggered(const FInputActionValue& Value); //CD
	void OnEquipItem4Triggered(const FInputActionValue& Value); //CD

	void OnCrouchActionStarted(const FInputActionValue& Value);

	void OnCrouchActionEnded(const FInputActionValue& Value);

	void OnAttackActionStarted(const FInputActionValue& Value);

	void OnAttackActionEnded(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackStartedEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackEndedEventTag;

	UFUNCTION(BlueprintCallable)
	FCharacterAnimationData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(const FCharacterAnimationData& InCharacterData);

	virtual void FinishDying() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	UInventoryComponent* InventoryComponent = nullptr;

protected:

	// Client only
	virtual void OnRep_PlayerState() override;

	virtual void InitAbilityActorInfo() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



//Character data

	UPROPERTY()//ReplicatedUsing = OnRep_CharacterData)
	FCharacterAnimationData CharacterAnimDataData;

	//UFUNCTION()
	//void OnRep_CharacterData();

	//virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly)
	class UCharacterAnimDataAsset* CharacterAnimDataAsset;




	/////////////////////////////////////////////////////////////////////
	//
	//                  CODE PART FOR THE INVENTORY 
	//
	////////////////////////////////////////////////////////////////////

	

	/* FOR TRACING */
	//void TraceForItems();
	//bool PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation);
	//UPROPERTY()//Replicated)
	//bool CanOverlap{false};
	/* For interacting */
	//Set for overlapp events with other actors
	//FORCEINLINE bool SetOverlappingActors(bool IsOverlaping) { return CanOverlap = IsOverlaping; }

	//UFUNCTION()
	//void Interact();

	//UFUNCTION(Server, Reliable)
	//void Server_Interact(UObject* InterfaceContext);

	//UPROPERTY()
	//AActor* CurrentIntertactable;

	//UPROPERTY(VisibleAnywhere, Category = "Interaction", Replicated)
	//TScriptInterface<class IInteractInterface> TargetInteractable;

	//void NoInteractableFound();
	//void FoundInteractable(AActor* NewInteractable);


public: 

	//Old Inventory

	/////////////////////////////////////////////////////////////////////
	//
	//                  CODE PART FOR THE INVENTORY 
	//
	////////////////////////////////////////////////////////////////////

	/*
	// Inventory itself
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<APickUpBaseActor*> Inventory;

	const int32 INVENTORY_CAPACITY{ 3 };


	// Equipping an Item

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "CurentlyEquippedItem")
	class APickUpBaseActor* EquippedItem; //For the currently Equipped Item

	FORCEINLINE void SetEquippedItem(APickUpBaseActor* NewItem) { EquippedItem = NewItem; }
	FORCEINLINE APickUpBaseActor* GetEquippedItem() { return EquippedItem; }

	UFUNCTION(BlueprintCallable)
	void EquipItem();


	//void Server_EquipItem(APickUpBaseActor* ItemToEquip);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItem(APMCharacter* InCharacterOwener, UObject* InterfaceContext);


	//Dropping an Item
	void DropItem();

	UFUNCTION(Server, Reliable)
	void Server_DropItem(APMCharacter* InCharacterOwener);
	//void Server_DropItem(); //v2



	// Swapping an Equipped item
	void SwapItem(APickUpBaseActor* ItemToSwap);

	UFUNCTION(Server, Reliable)
	void Server_SwapItem(APickUpBaseActor* ItemToSwap);//v2
	//v1 void Server_SwapItem(APMCharacter* InCharacterOwner, APickUpBaseActor* ItemToSwap);


	// Swapping the items based on Key Press

	UFUNCTION(BlueprintCallable)
	void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);

	UFUNCTION(Server, Reliable)
	void Server_ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);


	//Key Press functionality

	UFUNCTION(BlueprintCallable)
	void OneKeyPressedCharacter();

	UFUNCTION(BlueprintCallable)
	void TwoKeyPressedCharacter();

	UFUNCTION(BlueprintCallable)
	void ThreeKeyPressedCharacter();

	UFUNCTION(BlueprintCallable)
	void FourKeyPressedCharacter();

	*/

	//UPROPERTY(BlueprintAssignable) //Delegate to send information about the slot index to inventory bar
	//FEquippedItemDelegate EquippedItemDelegate;

	/////////////////////////////////////////////////////////////////////
	//
	//                  CODE PART FOR OLD INVENTORY 
	//
	////////////////////////////////////////////////////////////////////

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	//class UInventoryComponent* CharacterInventory = nullptr;

};
