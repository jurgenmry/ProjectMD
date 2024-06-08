// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldActors/InteractableActorBase.h"
#include "StoreBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMELTDOWN_API AStoreBaseActor : public AInteractableActorBase
{
	GENERATED_BODY()

public:

	AStoreBaseActor();

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* StoreMesh;

	virtual void Interact(APMCharacter* InInteractOwner) override; 

	//virtual void Interact_Implementation(AActor* InInteractOwner) override;
	
};
