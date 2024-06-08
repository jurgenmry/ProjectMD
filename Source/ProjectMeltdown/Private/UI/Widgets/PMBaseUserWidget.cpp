// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/PMBaseUserWidget.h"

void UPMBaseUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
