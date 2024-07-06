// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PMMovementComponent.h"

//System includes
#include "AbilitySystemComponent.h"

//Custome includes
#include "Characters/PMBaseCharacter.h"

UPMMovementComponent::UPMMovementComponent()
{
	SprintSpeedMultiplier = 1.4f;
	ADSSpeedMultiplier = 0.5f;

	MaxWalkSpeed = 350.0f;
}

float UPMMovementComponent::GetMaxSpeed() const
{
	APMBaseCharacter* Owner = Cast<APMBaseCharacter>(GetOwner());

	if (Owner)
	{
		/*
		if (!Owner->IsAlive())
		{
			return 0.0f;
		}
		*/

		if (RequestToStartSprinting)
		{
			return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
		}

		if (RequestToStartADS)
		{
			return Owner->GetMoveSpeed() * ADSSpeedMultiplier;
		}

		return Owner->GetMoveSpeed();
	}

	else 
	{
			return Super::GetMaxSpeed();
	}
}


void UPMMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}



FNetworkPredictionData_Client* UPMMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UPMMovementComponent* MutableThis = const_cast<UPMMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FGDNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}


void UPMMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UPMMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UPMMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UPMMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}

void UPMMovementComponent::FGDSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UPMMovementComponent::FGDSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UPMMovementComponent::FGDSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FGDSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartADS != ((FGDSavedMove*)&NewMove)->SavedRequestToStartADS)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UPMMovementComponent::FGDSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UPMMovementComponent* CharacterMovement = Cast<UPMMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UPMMovementComponent::FGDSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UPMMovementComponent* CharacterMovement = Cast<UPMMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UPMMovementComponent::FGDNetworkPredictionData_Client::FGDNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UPMMovementComponent::FGDNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FGDSavedMove());
}



