// Fill out your copyright notice in the Description page of Project Settings.


#include "PMAssetManager.h"

//Custome includes
#include "PMGameplayTags.h"

UPMAssetManager& UPMAssetManager::Get()
{
	// TODO: insert return statement here
	check(GEngine); //Crash on purpose if we dont have GEngine
	UPMAssetManager* PMAssetManager = Cast<UPMAssetManager>(GEngine->AssetManager);
	return *PMAssetManager;
}

void UPMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FPMGameplayTags::InitializeNativeGameplayTags();
}
