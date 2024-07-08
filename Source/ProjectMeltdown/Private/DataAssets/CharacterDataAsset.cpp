// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/CharacterDataAsset.h"

FCharacterDataClassDefaultInfo UCharacterDataAsset::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
    return CharacterClassInformation.FindChecked(CharacterClass);
}
