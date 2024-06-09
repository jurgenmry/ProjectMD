// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldActors/InteractableActorBase.h"
#include "Components/StaticMeshComponent.h"

#include "Inventory/ItemStaticData.h"

/*Interfaces*/
//#include "Interfaces/InventoryInterface.h"
#include "Interfaces/InteractInterface.h"

#include "PickUpBaseActor.generated.h"




USTRUCT(BlueprintType)
struct FPickUpItemInfo  : public FTableRowBase
{
	GENERATED_BODY()

	FPickUpItemInfo() {}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PickUpName = TEXT("PICKUP_NAME");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> PickUpStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDisplayPickupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* HudIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshOffset;

};


UENUM(BlueprintType)
enum class EItemsState : uint8
{
	EIS_Pickup          UMETA(DisplayName = "PICKUP"),
	EIS_EquipInterping  UMETA(DisplayName = "INTERPING"),
	EIS_PickedUP        UMETA(DisplayName = "PICKED_UP"),
	EIS_Equipped        UMETA(DisplayName = "EQUIP"),
	EIS_Falling         UMETA(DisplayName = "FALLING"),

	EIS_MAX UMETA(DisplayName = "DefaultMax")

};


UCLASS()
class PROJECTMELTDOWN_API APickUpBaseActor : public AInteractableActorBase //, public IInventoryInterface //public AActor, public IInteractInterface //public AInteractableActorBase
{
	GENERATED_BODY()

public:

	APickUpBaseActor();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;



	//virtual void Interact_Implementation(const class APMCharacter* InInteractOwner) override;
	virtual void Interact(class APMCharacter* InInteractOwner) override;


	UPROPERTY(EditAnywhere,BlueprintReadWrite, Replicated)
	UStaticMeshComponent* PickUpMesh; 

	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return PickUpMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemTableName;

	void SetItemProperties(EItemsState State);

	//Single Player Needs to be re-worked on multiplayer

	FORCEINLINE EItemsState GetItemState() const { return ItemState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ItemState) // Might need to be replicated 
	EItemsState ItemState = EItemsState::EIS_Pickup;
	
	void SetItemState(EItemsState State);

	UFUNCTION()
	void OnRep_ItemState(EItemsState OldState);







	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                                                                   //
	// =============================== NEW INTERFACE INVENTORY     ===================================== //
	//                                                                                                   //
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Pick Up Item */
	virtual void EquipItem(class APMCharacter* InPickupOwner) override;

	void StartPickUp(APMCharacter* InPickUpOwner);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItemActor(APMCharacter* InPickUpOwner);


	/* Drop  Item */
	virtual void DropItem(APMCharacter* InPickUpOwner) override;

	void StartDropItem();

	UFUNCTION(Server, Reliable)
	void Server_DropItem();


	/* Drop Item but add Impulse to it */

	FTimerHandle ThrowItemHandle;
	float ThrowItemTime = 0.7;

	UPROPERTY(Replicated)
	bool bItemFalling{false};

	void ThrowItem();

	UFUNCTION(Server, Reliable)
	void Server_ThrowItem(const FVector& ImpulseVector);
	
	void StopFalling();

	virtual void SwapItem() override;
	virtual void PickUpItem() override;
	


	

   ///////////////////////////////////////////////////////////////////////////////////////////////////////
   //                                                                                                   //
   // =============================== OLD INVENTORY FUNCTIONALIDY ===================================== //
   //                                                                                                   //
   ///////////////////////////////////////////////////////////////////////////////////////////////////////
   

	///////////////////////////////////////////////////
	/// 
	///           COMPONENTS : Added for testing
	///
	///////////////////////////////////////////////////
	
	/*
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* BoxComps;

	FORCEINLINE UBoxComponent* GetBoxComps() const { return BoxComps; }

	UPROPERTY(EditAnywhere, Category = "Components")
	class USphereComponent* SphereComps;

	FORCEINLINE USphereComponent* GetSphereComps() const { return SphereComps; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* InteractW;
	*/

	//virtual void BeginFocus() override;
	//virtual void EndFocus() override;

	/*
	void InitActor(UInventoryItemInstance* InItemInstance);
	virtual bool ReplicateSubObjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);

	
	virtual void OnEquipped();
	virtual void OnUnEquipped();
	virtual void OnDropped();
	

	UPROPERTY(Replicated)
	class UInventoryItemInstance* ItemInstance{ nullptr };


	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	EItemState CurrentItemState = EItemState::None;

	UFUNCTION()
	void OnRep_ItemState();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;


protected:


	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIntex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIntex);
*/
	
};
