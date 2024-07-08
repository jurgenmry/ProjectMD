// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inventory/ItemStaticData.h"
#include "CharacterDataAsset.generated.h"


UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	EnemyRanger  UMETA(DisplayName = "Ranger"),
	EnemyMelee   UMETA(DisplayName = "Melee")
};


USTRUCT(BlueprintType)
struct FCharacterDataClassDefaultInfo
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Defaults")
	TSubclassOf<class UGameplayEffect> PrimaryAttributes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class Defaults")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	//class UCharacterAnimDataAsset* CharacterAnimDataAsset;
};


UCLASS(BlueprintType, Blueprintable)
class PROJECTMELTDOWN_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditDefaultsOnly, Category = "Common class Defaults")
	TMap<ECharacterClass, FCharacterDataClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common class Defaults")
	TSubclassOf<class UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common class Defaults")
	TSubclassOf<class UGameplayEffect> SecondaryAttributes;

	FCharacterDataClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

};
