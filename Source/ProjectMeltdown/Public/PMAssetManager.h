// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PMAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMELTDOWN_API UPMAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UPMAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
