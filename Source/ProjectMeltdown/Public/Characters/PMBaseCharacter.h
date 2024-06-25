#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMeltdown/ProjectMeltdown.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/CombatInterface.h"
#include "PMBaseCharacter.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EGDHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, APMBaseCharacter*, Character);


UCLASS(Abstract)
class PROJECTMELTDOWN_API APMBaseCharacter : public ACharacter, public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	
	APMBaseCharacter();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UPMBaseAttributeSet* GetAttributeSetBase() const;

	virtual int32 GetPlayerlevel() override;

	/* NEW CODE */

	// Set the Hit React direction in the Animation Blueprint
	UPROPERTY(BlueprintAssignable)
	FCharacterBaseHitReactDelegate ShowHitReact;

	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable)
	EGDHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;


	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDCharacter")
	virtual void FinishDying();

	// Death Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GASDocumentation|Animation")
	UAnimMontage* DeathMontage;

	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	/* END OF NEW CODE */

protected:
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TObjectPtr<class UPMBaseAbilitySystemComponent>AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<class UPMBaseAttributeSet> AttributeSet;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
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

};
