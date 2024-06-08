// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/InteractableActorBase.h"


//System includes:

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

//Custome includes
#include "UI/Widgets/PMBaseUserWidget.h" 
#include "Characters/PMCharacter.h"
#include "Controllers/PMMainCharacterPlayerController.h"
#include "UI/HUD/PMBaseHud.h"


AInteractableActorBase::AInteractableActorBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComps = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComps"));
	//BoxComps->SetupAttachment(GetRootComponent());
	BoxComps->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComps->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	SetRootComponent(BoxComps);
	BoxComps->SetHiddenInGame(true);
	BoxComps->SetIsReplicated(true);

	SphereComps = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComps"));
	SphereComps->SetSphereRadius(70.0f);
	SphereComps->SetupAttachment(GetRootComponent());
	SphereComps->SetHiddenInGame(true);

	

	InteractW = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractW"));
	InteractW->SetupAttachment(BoxComps);
}


void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComps->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereOverlap);
	SphereComps->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnSphereEndOverlap);
	InteractW->SetVisibility(false);
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractableActorBase::BeginFocus()
{
	InteractW->SetVisibility(true);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Begin Focus")), 1);
}


void AInteractableActorBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor)
	{
		APMCharacter* Character = Cast<APMCharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled())
		{
			Character->SetOverlappingActors(true);
			BeginFocus();
	
		}
	}
	//FString Overlapping = "Overlaping Interactable";
	//GEngine->AddOnScreenDebugMessage(10, 3.0f, FColor::Red, Overlapping, 1);
}

void AInteractableActorBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIntex)
{
	if (OtherActor != this && OtherActor)
	{
		
		APMCharacter* Character = Cast<APMCharacter>(OtherActor);
		if (Character && Character->IsLocallyControlled())
		{
			Character->SetOverlappingActors(false);
			EndFocus();
			//Character->//HUD->GetInteractionWidget()->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AInteractableActorBase::EndFocus()
{
	InteractW->SetVisibility(false);
}

void AInteractableActorBase::Interact(APMCharacter* InInteractOwner)
{
	//EndFocus();
}


/*
void AInteractableActorBase::Interact_Implementation(const APMCharacter* InInteractOwner)
{
	EndFocus();
}
*/


