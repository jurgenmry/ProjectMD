// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UAG_PhysicalMaterial.generated.h"

class USoundBase;
class UNiagaraSystem;

UCLASS()
class PROJECTMELTDOWN_API UUAG_PhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalMaterial)
	USoundBase* FootstepSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalMaterial)
	USoundBase* PointImpactSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalMaterial)
	UNiagaraSystem* PointImpactVFX = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicalMaterial)
	//UNiagaraSystem* WeaponMuzzleVFX = nullptr;

};
