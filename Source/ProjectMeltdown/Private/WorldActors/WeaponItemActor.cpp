// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/WeaponItemActor.h"


#include "Inventory/InventoryItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/ItemStaticData.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/UAG_PhysicalMaterial.h"


//Custome includes
#include "Net/UnrealNetwork.h"

AWeaponItemActor::AWeaponItemActor()
{

}


const UWeaponStaticData* AWeaponItemActor::GetWeaponStaticData() const
{
	return ItemInstance ? Cast<UWeaponStaticData>(ItemInstance->GetItemStaticData()) : nullptr;
}

FVector AWeaponItemActor::GetMuzzleLocation() const
{
	return MeshComponent ? MeshComponent->GetSocketLocation(TEXT("Muzzle")) : GetActorLocation();
}

void AWeaponItemActor::InitInternal()
{
	Super::InitInternal();

	if (const UWeaponStaticData* WeaponData = GetWeaponStaticData())
	{
		if (WeaponData->SkeletalMesh)
		{
			USkeletalMeshComponent* SkeletalComp = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("MeshComponent"));
			if (SkeletalComp)
			{
				SkeletalComp->RegisterComponent();
				SkeletalComp->SetSkeletalMesh(WeaponData->SkeletalMesh);
				SkeletalComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				SkeletalComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				MeshComponent = SkeletalComp;
			}
		}
		else if (WeaponData->StaticMesh)
		{
			UStaticMeshComponent* StaticComp = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("MeshComponent"));
			StaticComp->RegisterComponent();
			StaticComp->SetStaticMesh(WeaponData->StaticMesh);
			StaticComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			StaticComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			MeshComponent = StaticComp;
		}
	}
}


void AWeaponItemActor::PlayWeaponEffects(const FHitResult& InHitResult)
{
	if (HasAuthority())
	{
		MulticastPlayWeaponEffects(InHitResult);
	}
	else
	{
		PlayWeaponEffectsInternal(InHitResult);
	}
}

void AWeaponItemActor::MulticastPlayWeaponEffects_Implementation(const FHitResult& InHitResult)
{
	if (!Owner || Owner->GetLocalRole() != ROLE_AutonomousProxy)
	{
		PlayWeaponEffectsInternal(InHitResult);
	}
}

void AWeaponItemActor::PlayWeaponEffectsInternal(const FHitResult& InHitResult)
{
	
	if (InHitResult.PhysMaterial.Get())
	{
		UUAG_PhysicalMaterial* PhysicalMaterial = Cast<UUAG_PhysicalMaterial>(InHitResult.PhysMaterial.Get());

		if (PhysicalMaterial)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->PointImpactSound, InHitResult.ImpactPoint, 1.f);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PhysicalMaterial->PointImpactVFX, InHitResult.ImpactPoint);	
		}
	}

	if (const UWeaponStaticData* WeaponData = GetWeaponStaticData())
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponData->AttackSound, GetActorLocation(), 1.f);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, WeaponData->MuzzleVFX, GetMuzzleLocation());
	}
}





