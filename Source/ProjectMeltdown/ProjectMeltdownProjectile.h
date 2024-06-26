// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Inventory/ItemStaticData.h"
#include "ProjectMeltdownProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AProjectMeltdownProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectMeltdownProjectile();

	const UProjectileStaticData* GetProjectileStaticData() const;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TSubclassOf<UProjectileStaticData> ProjectileDataClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	class UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	void DebugDrawPath() const;

	UPROPERTY()
	class UStaticMeshComponent* StaticMeshComponent = nullptr;

	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

