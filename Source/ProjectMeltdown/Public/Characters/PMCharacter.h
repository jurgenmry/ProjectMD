// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PMBaseCharacter.h"
#include "PMCharacter.generated.h"


class USkeletalMeshComponent;
class UCameraComponent;


UCLASS()
class PROJECTMELTDOWN_API APMCharacter : public APMBaseCharacter
{
	GENERATED_BODY()


	APMCharacter();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;


	virtual int32 GetPlayerlevel() override;



	UPROPERTY()//Replicated)
	bool CanOverlap{false};

	/* For interacting */

	//Set for overlapp events with other actors
	FORCEINLINE bool SetOverlappingActors(bool IsOverlaping) { return CanOverlap = IsOverlaping; }


	UFUNCTION()
	void Interact();

	UFUNCTION(Server, Reliable)
	void Server_Interact(UObject* InterfaceContext);

	
	UPROPERTY()
	AActor* CurrentIntertactable;

	UPROPERTY(VisibleAnywhere, Category = "Interaction", Replicated)
	TScriptInterface<class IInteractInterface> TargetInteractable;
	
	void NoInteractableFound();
	void FoundInteractable(AActor* NewInteractable);

	/////////////////////////////////////////////////////////////////////
	//
	//                  CODE PART FOR THE INVENTORY 
	//
	////////////////////////////////////////////////////////////////////


	/* Swapping an Equipped item */
	void SwapItem(APickUpBaseActor* ItemToSwap);

	UFUNCTION(Server, Reliable)
	void Server_SwapItem(APMCharacter* InCharacterOwner, APickUpBaseActor* ItemToSwap);


	/* Equipping an Item */

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "CurentlyEquippedItem")
	class APickUpBaseActor* EquippedItem; //For the currently Equipped Item

	FORCEINLINE void SetEquippedItem(APickUpBaseActor* NewItem){ EquippedItem = NewItem; }
	FORCEINLINE APickUpBaseActor* GetEquippedItem() { return EquippedItem; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void EquipItem(); /* Takes an Item attach it to mesh*/

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItem(APMCharacter* InCharacterOwener, UObject* InterfaceContext);



	/* Dropping an Item */
	void DropItem();

	UFUNCTION(Server, Reliable)
	void Server_DropItem(APMCharacter* InCharacterOwener);





	/////////////////////////////////////////////////////////////////////
	//
	//                  CODE PART FOR OLD INVENTORY 
	//
	////////////////////////////////////////////////////////////////////

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	//class UInventoryComponent* CharacterInventory = nullptr;


protected:

	// Client only
	virtual void OnRep_PlayerState() override;

	virtual void InitAbilityActorInfo() override;


	/* FOR TRACING */

	void TraceForItems();
	bool PerformTrace(FHitResult& OutHitResult, FVector& OutHitLocation);


};
