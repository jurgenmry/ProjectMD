// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PMWidgetControllerBase.generated.h"


/*

					**************** Arquitecture ****************
					 THIS IS THE BASE CLASS OF WIDGET CONTROLLER


1. Create widget controller who will send the information of GAS to the UI
2. The information will be set in a struct (to easily add the information manually on the WBP)

	Player controller
	Player state
	Ability system component
	Attibute set

3.We create a HUD who will receive the information of the widget controller
4. In the hud we do the creation of the Widget controller and any other widget.
	in this case the overlay widget (that will add the necesary information on the screen

*/


class UAbilitySystemComponent;
class UAttributeSet;

class UPMBaseAbilitySystemComponent;
class UPMBaseAttributeSet;


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

		FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC,
		APlayerState* PS,
		UAbilitySystemComponent* ASComp,
		UAttributeSet* ASet)
		//UPMBaseAbilitySystemComponent* ASComp,
		//UPMBaseAttributeSet* ASet)
		:PlayerController(PC)
		, PlayerState(PS)
		, AbilitySystemComps(ASComp)
		, AttributeSet(ASet)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TWeakObjectPtr<UPMBaseAbilitySystemComponent> AbilitySystemComp = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TWeakObjectPtr<UPMBaseAttributeSet> AttributeSet = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComps = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;


};


UCLASS()
class PROJECTMELTDOWN_API UPMWidgetControllerBase : public UObject
{
	GENERATED_BODY()
		


public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WControllerParams);

	virtual void BroadCastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;


	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;



	//UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	//TObjectPtr<UPMBaseAbilitySystemComponent> AbilitySystemComponent;

	//UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	//TObjectPtr<UPMBaseAttributeSet> AttributeSet;

};
