// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PMBaseHud.generated.h"

/*
									HUD Logic

	 The hud will be in charge of draw all the widgets to the viewport.
	 We need a reference / creation of the widget and its controller
	 We need the class and the Object for both (Widget and Controller):

	 Steps:

	 1) In this case the Widget that handles the drawing its type overlay Widget,
	 we need object and its class:

		Object: OverlayWidget
		class : OverlayWidgetClass

	2) We also need the controller that handles the data and send it to the widget and its class,
	(we also need object & class), for this particular case will be:

		Object: OverlayWidgetController
		class : OverlayWidgetControllerClass

	3) We need a function that with get hold of the widget controller, if the widget controller
	was created do nothing, if not create a new one:

		GetOverlayWidgetController(const FWidgetControllerParams& Widgetparams)

	4) The widget controller needs to get hold of the Struct variables (FWidgetControllerParams),
	we need to create a function will get the variables and initialize them:

			void InitOverlay(APlayerController* PC,
					APlayerState* PS,
					UAbilitySystemComponent* ABSC,
					UAttributeSet* ATriSet);

	5) In the function InitOverlay:

		.Create the widget that will be added to the viewport
				*Need to cast to the specific one we want
		.Create the controller for that widget
				*Get the specific controller for that widget
		.Add the widget to the viewport

	6) Init overlay will be called in the actor because it needs
	the 4 variables.
 */


class UPMBaseUserWidget;
class UPMOverlayWidgetController;
struct FWidgetControllerParams;

UCLASS()
class PROJECTMELTDOWN_API APMBaseHud : public AHUD
{
	GENERATED_BODY()

	
public:


	UPROPERTY()
	TObjectPtr<UPMOverlayWidgetController> OverlayWController;

	UPROPERTY()
	TObjectPtr<UPMBaseUserWidget> BaseOverlayWidget;

	UPROPERTY()
	TObjectPtr<UPMBaseUserWidget> StoreWidget;

	UFUNCTION()
	UPMOverlayWidgetController* GetWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(class APlayerController* PC,
		class APlayerState* PS,
		class UAbilitySystemComponent* ABSC,
		class UAttributeSet* ATSet);

	UFUNCTION()
	void CreateWidgetStore();


private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPMBaseUserWidget> BaseOverlayWidgetClass;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UPMOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPMBaseUserWidget>StoreWidgetClass;



protected:
	
	virtual void BeginPlay()override;
	
};
