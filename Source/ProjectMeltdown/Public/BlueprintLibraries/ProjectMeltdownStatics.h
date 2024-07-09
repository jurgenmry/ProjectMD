// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inventory/ItemStaticData.h"
#include "DataAssets/CharacterDataAsset.h"
#include "ProjectMeltdownStatics.generated.h"

class AProjectMeltdownProjectile;
class AbilitySystemComponent;

UCLASS()
class PROJECTMELTDOWN_API UProjectMeltdownStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PMAbilitySystemLibrary|Combat")
	static const UItemStaticData* GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass);
	

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "PMAbilitySystemLibrary|Combat")
	static void ApplyRadialDamage(UObject* WorldContextObject, AActor* DamageCauser, FVector Location, float Radius, float DamageAmount, TArray<TSubclassOf<class UGameplayEffect>> DamageEffects,
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, ETraceTypeQuery TraceType);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "PMAbilitySystemLibrary|Combat")
	static AProjectMeltdownProjectile* LaunchProjectile(UObject* WorldContextObject, TSubclassOf<UProjectileStaticData> ProjectileDataClass, FTransform Transform, AActor* Owner, APawn* Instigator);

	UFUNCTION(BlueprintCallable, Category = "PMAbilitySystemLibrary|CharacterDefaults")
	static void InitializeEnemyDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "PMAbilitySystemLibrary|CharacterDefaults")
	static void GiveStartupAbilitites(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
};
