// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/PMBaseHud.h"

//System includes
#include "Blueprint/UserWidget.h"

//Custome Includes
#include "UI/Widgets/PMBaseUserWidget.h" 
#include "UI/Controllers/PMOverlayWidgetController.h"



UPMOverlayWidgetController* APMBaseHud::GetWidgetController(const FWidgetControllerParams& WCParams)
{
	//Check if the widget has a controller
	//if its null, create a new controller for the widget
	//Add the params to the widget controller

	if (OverlayWController == nullptr)
	{
		OverlayWController = NewObject<UPMOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWController->SetWidgetControllerParams(WCParams);
		OverlayWController->BindCallbacksToDependencies();

		//FString MSG = "Widget controller Not Null";
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, MSG, 1);

		return OverlayWController;
	}

	//FString MSG = "Widget controller Null";
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, MSG, 1);

	return OverlayWController;
}

void APMBaseHud::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ABSC, UAttributeSet* ATSet)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), BaseOverlayWidgetClass);
	BaseOverlayWidget = Cast<UPMBaseUserWidget>(Widget);

	const FWidgetControllerParams WControllerParams(PC,PS,ABSC,ATSet);
	UPMOverlayWidgetController* OverlayWC = GetWidgetController(WControllerParams);

	BaseOverlayWidget->SetWidgetController(OverlayWC);

	if (BaseOverlayWidget != nullptr && OverlayWC != nullptr)
	{
		OverlayWC->BroadCastInitialValues();
		BaseOverlayWidget->AddToViewport();
	}

	else
	{
		FString MSG = "Widget Class & Widget Controller not Initlialized, fill HUD";
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, MSG, 1);
	}
}

void APMBaseHud::CreateWidgetStore()
{
	
	UUserWidget* StoreW = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass);
	StoreWidget = Cast<UPMBaseUserWidget>(StoreW);
	if (StoreWidget)
	{
		StoreWidget->AddToViewport();
		//FString MSG = "Store Widget Controller Initialized";
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, MSG, 1);
	}
	else
	{
		FString MSG = "fill in HUD store menu";
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, MSG, 1);
	}
}

void APMBaseHud::BeginPlay()
{
	Super::BeginPlay();
	/*
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass);
	StoreWidget = Cast<UPMBaseUserWidget>(Widget);
	StoreWidget->AddToViewport();
	StoreWidget->SetVisibility(ESlateVisibility::Collapsed);
	*/
}

