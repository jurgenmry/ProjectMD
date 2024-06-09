// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/InteractInterface.h"
#include "InteractableActorBase.generated.h"


/*
UENUM()
enum class EItemState : uint8
{
	Ready_For_Pickup UMETA(DisplayName = "Ready_For_Pickup"),
	Equip_Interping UMETA(DisplayName = "Equip_Interping"),
	Item_PickedUp UMETA(DisplayName = "Item_PickedUp"),
	Equipped UMETA(DisplayName = "Equipped"),
	NoState UMETA(DisplayName = "No_State") // This is for doors and others

	//Should add drop item as well 
};
*/


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

	/* Interact interface overrides */

	virtual void BeginFocus() override;

	virtual void EndFocus() override;

	virtual void Interact(class APMCharacter* InInteractOwner) override;
	//virtual void Interact_Implementation(const APMCharacter* InInteractOwner) override;



	/*For Inventory*/
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
