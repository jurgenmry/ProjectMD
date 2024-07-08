#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMeltdown/ProjectMeltdown.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"
#include "PMBaseCharacter.generated.h"


class UAnimMontage;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomMontageEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomMontageBlendedOut);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomMontageInterrupted);


UCLASS(Abstract)
class PROJECTMELTDOWN_API APMBaseCharacter : public ACharacter, public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	
	APMBaseCharacter(const class FObjectInitializer& ObjectInitializer);

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UPMBaseAttributeSet* GetAttributeSetBase() const;

	virtual int32 GetPlayerlevel() override;	// Set the Hit React direction in the Animation Blueprint


	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	UPROPERTY(EditAnywhere, Category = "MAIN|Abilities")
	TObjectPtr<UAnimMontage>HitReactMontage;


protected:
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TObjectPtr<class UPMBaseAbilitySystemComponent>AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<class UPMBaseAttributeSet> AttributeSet;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MAIN|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MAIN|Abilities")
	TSubclassOf<class UGameplayEffect> VitaltAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MAIN|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MAIN|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MAIN|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultOvertimeAttributes;


	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	virtual void ApplyGEToSelf(TSubclassOf<UGameplayEffect>GameplayEffectClass, float Level); //Apply effect to self


public:

	//////////////////////////////////////////////////////////////////////////////
	//																			//
	// *************       ATTRIBUTE GETTERS PMAttributeSet       ************* //
	//                                                                          //        
	//////////////////////////////////////////////////////////////////////////////


	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetRadiation() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxRadiation() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxStamina() const;

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMoveSpeedBaseValue() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetOxigen() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|BaseCharacter|Attributes")
	float GetMaxOxigen() const;


	//////////////////////////////////////////////////////////////////////////////
	//																			//
	// *************       ATTRIBUTE SETTERS PMAttributeSet       ************* //
	//                                                                          //        
	//////////////////////////////////////////////////////////////////////////////

	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/

	virtual void SetHealth(float Health);
	virtual void SetRadiation(float Radiation);
	virtual void SetStamina(float Stamina);
	virtual void SetOxigen(float Oxigen);



	//////////////////////////////////////////////////////////////////////////////
	//																			//
	// *************     FOR PLAYING ANIMATIONS ON CHARACTERS     ************* //
	//                                                                          //        
	//////////////////////////////////////////////////////////////////////////////


	// Function to play animation
	UFUNCTION(BlueprintCallable, Category = "MAIN|Animation")
	void PlayAnimationMulticast(UAnimMontage* Animation3P, UAnimMontage* Animation1P, USkeletalMeshComponent* AnimMesh1P, USkeletalMeshComponent* AnimMesh3P);

	// Function to call on the server to play animation
	UFUNCTION(Server, Reliable)
	void ServerPlayAnimation(UAnimMontage* Animation3P, USkeletalMeshComponent* AnimMesh3P);

	// Function to call on all clients to play animation
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAnimation(UAnimMontage* Animation3P, USkeletalMeshComponent* AnimMesh3P);

	// Delegate to handle montage end
	UPROPERTY(BlueprintAssignable, Category = "MAIN|Animation")
	FOnCustomMontageEnded  OnCustomMontageEnded;

	// Delegate to handle montage blend out
	UPROPERTY(BlueprintAssignable, Category = "MAIN|Animation")
	FOnCustomMontageBlendedOut  OnCustomMontageBlendedOut;

	// Delegate to handle montage interrupted
	UPROPERTY(BlueprintAssignable, Category = "MAIN|Animation")
	FOnCustomMontageInterrupted  OnCustomMontageInterrupted;

	// Internal function to handle montage end
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Internal function to handle montage blend out
	UFUNCTION()
	void HandleMontageBlendedOut(UAnimMontage* Montage, bool bInterrupted);

};
