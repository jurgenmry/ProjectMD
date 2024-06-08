// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintLibraries/ProjectMeltdownStatics.h"

const UItemStaticData* UProjectMeltdownStatics::GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass)
{
	if (IsValid(ItemDataClass))
	{
		return GetDefault<UItemStaticData>(ItemDataClass);
	}
	return nullptr;
}
