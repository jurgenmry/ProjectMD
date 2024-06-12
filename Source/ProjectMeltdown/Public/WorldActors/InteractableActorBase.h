// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Inventory/ItemStaticData.h"
//#include "Interfaces/InteractInterface.h"
#include "InteractableActorBase.generated.h"


class UInventoryItemInstance;
class USphereComponent;


UCLASS()
class PROJECTMELTDOWN_API AInteractableActorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActorBase();

	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void Init(UInventoryItemInstance* InInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_ItemInstance)
	UInventoryItemInstance* ItemInstance = nullptr;

	UFUNCTION()
	void OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance);

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	EItemState ItemState = EItemState::None;

	UFUNCTION()
	void OnRep_ItemState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

	UPROPERTY(EditAnywhere, Replicated)
	int32 Quantity = 1;

	virtual void InitInternal();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


//old Implementation
/*
UCLASS()
class PROJECTMELTDOWN_API AInteractableActorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	
	AInteractableActorBase();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	//================================================================================//
	// Variables & Properties
	//================================================================================//

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* BoxComps;

	FORCEINLINE UBoxComponent* GetBoxComps() const { return BoxComps; }

	UPROPERTY(EditAnywhere, Category = "Components")
	class USphereComponent* SphereComps;

	FORCEINLINE USphereComponent* GetSphereComps() const { return SphereComps; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Components")
	class UWidgetComponent* InteractW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UStaticMeshComponent* PickUpMesh;

	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return PickUpMesh; }

	//Interact interface overrides 

	virtual void BeginFocus() override;

	virtual void EndFocus() override;

	virtual void Interact(class APMCharacter* InInteractOwner) override;
	//virtual void Interact_Implementation(const APMCharacter* InInteractOwner) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	///For Inventory
	virtual void PickUpItem() override {};
	virtual void SwapItem() override{};
	virtual void EquipItem(class APMCharacter* InPickupOwner) override {};
	virtual void DropItem(APMCharacter* InPickUpOwner) override {};


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

	
	

};
*/
