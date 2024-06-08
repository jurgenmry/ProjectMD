// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controllers/PMWidgetControllerBase.h"
#include "GameplayTagContainer.h"
#include "PMOverlayWidgetController.generated.h"

/*
  1. Get Dynamic multicas delegates that will be used to broadcast the initial values of attibute set
  2. Create the signature variables for those delegates
  3. We brodcast the initial values
  4. We create callbacks functions to listen for attribute changes
  5. we bind the change callback to those functions
  6. Call it in the hud to listen for changes
*/


/*Struct to add message on screen widgets based on asset tags*/

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Messagetag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageToDisplay = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPMBaseUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ImageToDisplay = nullptr;
};


//All float attributes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);


//All Int attributes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntAttributeChangeSignature, int32, NewValue);

/*Broadcasting the gameplay Tags*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


UCLASS(BlueprintType, Blueprintable)
class PROJECTMELTDOWN_API UPMOverlayWidgetController : public UPMWidgetControllerBase
{
	GENERATED_BODY()

public:

	virtual void BroadCastInitialValues() override;



	/*Health*/
	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;


	/*Radiation*/
	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnRadiationChanged;


	/*Oxigen*/
	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnOxigenChanged;

	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxOxigenChanged;

	/*Stamina*/
	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnAttributeChangeSignature OnStaminaRegenRateChanged;

	/*XP - GOLD - Character Level */
	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnIntAttributeChangeSignature OnXPChanged;

	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnIntAttributeChangeSignature OnGoldChanged;


	/*Not sure about the character lvl, but leaving this here just in case*/
	UPROPERTY(BlueprintAssignable, category = "GAS|Attributes")
	FOnIntAttributeChangeSignature OnCharacterLevelChange;

	/*broadcast the widget row for the messages on screen*/
	UPROPERTY(BlueprintAssignable, category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRow;



	virtual void BindCallbacksToDependencies() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable>MessageWidgetDataTable;

	/* Template function to return Data table to be used for the display message*/

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

/* Template function to get the row from the data table and matches the tag*/
template<typename T>
inline T* UPMOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	T* Row = DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	if (Row)
	{
		return Row;
	}
	return nullptr;
}
