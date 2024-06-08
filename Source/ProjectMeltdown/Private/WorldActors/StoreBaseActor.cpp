// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/StoreBaseActor.h"


//System includes:
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"

//Custome includes
#include "UI/Widgets/PMBaseUserWidget.h" 
#include "Characters/PMCharacter.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "UI/HUD/PMBaseHud.h"


AStoreBaseActor::AStoreBaseActor()
{
	StoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoreMesh"));
	StoreMesh->SetupAttachment(GetRootComponent());
}



void AStoreBaseActor::Interact(APMCharacter* InInteractOwner)
{
	//Do Something
}


/*
void AStoreBaseActor::Interact_Implementation(AActor* InInteractOwner)
{
	//Super::Interact(InInteractOwner);

	
	if (CharacterReference != nullptr)
	{
		APMMainCharacterPlayerController* PC = Cast<APMMainCharacterPlayerController>(CharacterReference->GetController());
		if (PC)
		{
			//We need to cast the result to our own hud
			APMBaseHud* BaseHud = Cast<APMBaseHud>(PC->GetHUD());
			if (BaseHud)
			{
				BaseHud->CreateWidgetStore();

				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
	
}
*/

