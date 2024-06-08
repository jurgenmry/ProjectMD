// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStaticData.generated.h"


class APickUpBaseActor;


UENUM(BlueprintType)
enum class EItemState : uint8
{
	None       UMETA(DisplayName = "NONE"),
	Equipped   UMETA(DisplayName = "EQUIPPED"),
	UnEquipped UMETA(DisplayName = "UN_EQUIPPED"),
	Dropped    UMETA(DisplayName = "Dropped"),
};


UCLASS(BlueprintType, Blueprintable)
class PROJECTMELTDOWN_API UItemStaticData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ObjectName;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APickUpBaseActor> ItemActorClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachedSocket = "NAME_NONE";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanEquip = false;
};
