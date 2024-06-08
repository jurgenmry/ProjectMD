// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/PMWidgetControllerBase.h"

//System includes
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

//custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"



void UPMWidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WControllerParams)
{
	PlayerController = WControllerParams.PlayerController;
	PlayerState = WControllerParams.PlayerState;
	AbilitySystemComponent = WControllerParams.AbilitySystemComps;
	AttributeSet = WControllerParams.AttributeSet;
}

void UPMWidgetControllerBase::BroadCastInitialValues()
{
}

void UPMWidgetControllerBase::BindCallbacksToDependencies()
{
}
