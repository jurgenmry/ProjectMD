// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/PMOverlayWidgetController.h"

//System includes

//Custome includes
#include "AbilitySystem/PMBaseAttributeSet.h"
#include "Characters/PMCharacter.h"
#include "GameModes/PMPlayerState.h"
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"



void UPMOverlayWidgetController::BroadCastInitialValues()
{
	//Get Hold of the Attribute set 
	//UPMBaseAttributeSet* MainAttributeSet = Cast<UPMBaseAttributeSet>(AttributeSet);
	
	APMPlayerState* PS = Cast<APMPlayerState>(PlayerState);
	APMCharacter* Character = Cast <APMCharacter>(PS->GetPawn());
	
	//APMCharacter* Character = Cast<APMCharacter>();
	/*if (Character)
	{
		//FString TXT = "Character not null";
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TXT, 1);
	}
	*/

	/*Health Broadcast*/
	OnHealthChanged.Broadcast(PS->GetHealth());
	OnMaxHealthChanged.Broadcast(PS->GetMaxHealth());

	/*Rad Broadcast*/
	OnRadiationChanged.Broadcast(PS->GetRadiation());

	/* Oxigen Broadcast*/
	OnOxigenChanged.Broadcast(PS->GetOxigen());
	OnMaxOxigenChanged.Broadcast(PS->GetMaxOxigen());

	/*Stamina Broadcast*/
	OnStaminaChanged.Broadcast(PS->GetStamina());
	OnMaxStaminaChanged.Broadcast(PS->GetMaxStamina());
	OnStaminaRegenRateChanged.Broadcast(PS->GetStaminaRegenRate()); 
	// Need to check this one, should be in the character ?

	/* XP - GOLD - CLvL Broadcast*/
	OnXPChanged.Broadcast(PS->GetXP());
	OnGoldChanged.Broadcast(PS->GetGold());
	OnCharacterLevelChange.Broadcast(PS->GetCharacterLevel());

}

void UPMOverlayWidgetController::BindCallbacksToDependencies()
{
	
	const UPMBaseAttributeSet* MainAttributeSet = Cast<UPMBaseAttributeSet>(AttributeSet);
	
	/* Health listen Bindings*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetHealthAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data) 
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetMaxHealthAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

	/* Radiation listen Bindings*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetRadiationAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnRadiationChanged.Broadcast(Data.NewValue);
			});

	/* Oxigen listen Bindings*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetOxigenAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnOxigenChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetMaxOxigenAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxOxigenChanged.Broadcast(Data.NewValue);
			});

	/* Stamina listen Bindings*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetStaminaAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetMaxStaminaAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetStaminaRegenRateAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaRegenRateChanged.Broadcast(Data.NewValue);
			});

	/* XP - GOLD - CLvL listen Bindings*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetXPAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnXPChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetGoldAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnGoldChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAttributeSet->GetCharacterLevelAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnCharacterLevelChange.Broadcast(Data.NewValue);
			});


	UPMBaseAbilitySystemComponent* ABComps = Cast<UPMBaseAbilitySystemComponent>(AbilitySystemComponent);//->EffectAsseTags.AddLambda(
	
	ABComps->EffectAssetTags.AddLambda(
		[this, ABComps](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

				//We will only send specific types of tags
				//"A1". matchesTag.A, will return true, A matches tag A1 will return false
				//or if Message.Health matches tag message will return true, message tag message.health will return false

				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message")); //->Request Tag message
				if (Tag.MatchesTag(MessageTag)) //-> If it matches 
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag); //->Review the comparisson with DT
					//ABComps->AddLooseGameplayTag(Tag, 1);

					MessageWidgetRow.Broadcast(*Row); //->Broadcast it
				}

			}
		});
}



