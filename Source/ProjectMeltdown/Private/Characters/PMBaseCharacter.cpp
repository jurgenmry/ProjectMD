// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PMBaseCharacter.h"
 
//System includes
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTagsManager.h"
#include "GameFramework/CharacterMovementComponent.h"

//custome includes
#include "AbilitySystem/PMBaseAbilitySystemComponent.h"
#include "AbilitySystem/PMBaseAttributeSet.h"
#include "PMGameplayTags.h"
#include "Components/PMMovementComponent.h"


APMBaseCharacter::APMBaseCharacter(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPMMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;


	// Cache tags
	//HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Hit.Direction.Front"));
	//HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Hit.Direction.Back"));
	//HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Hit.Direction.Back"));
	//HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Hit.Direction.Left"));
	//DeadTag = FGameplayTag::RequestGameplayTag(FName("Movement.State.Dead"));
	//EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Movement.State.RemoveOnDeath"));
	
}

UAbilitySystemComponent* APMBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UPMBaseAttributeSet* APMBaseCharacter::GetAttributeSetBase() const
{
	return AttributeSet.Get();
}

int32 APMBaseCharacter::GetPlayerlevel()
{
	return 0;
}

void APMBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APMBaseCharacter::InitAbilityActorInfo()
{
}

void APMBaseCharacter::InitializeAttributes()
{
	ApplyGEToSelf(DefaultAttributes, 1.f);
	ApplyGEToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyGEToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyGEToSelf(DefaultOvertimeAttributes, 1.f);
}

void APMBaseCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility));

		//FString AbilityName = StartupAbility ? StartupAbility->GetName() : FString("Unknown");
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Added Ability %s"), *AbilityName), 1);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void APMBaseCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

void APMBaseCharacter::ApplyGEToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

float APMBaseCharacter::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float APMBaseCharacter::GetRadiation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetRadiation();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxRadiation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxRadiation();
	}

	return 0.0f;
}

float APMBaseCharacter::GetStamina() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetStamina();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxStamina() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMoveSpeed() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue();
	}

	return 0.0f;
}

float APMBaseCharacter::GetOxigen() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetOxigen();
	}

	return 0.0f;
}

float APMBaseCharacter::GetMaxOxigen() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxOxigen();
	}

	return 0.0f;
}

void APMBaseCharacter::SetHealth(float Health)
{
	if (AttributeSet)
	{
		AttributeSet->SetHealth(Health);
	}
}

void APMBaseCharacter::SetRadiation(float Radiation)
{
	if (AttributeSet)
	{
		AttributeSet->SetRadiation(Radiation);
	}
}

void APMBaseCharacter::SetStamina(float Stamina)
{
	if (AttributeSet)
	{
		AttributeSet->SetStamina(Stamina);
	}
}

void APMBaseCharacter::SetOxigen(float Oxigen)
{
	if (AttributeSet)
	{
		AttributeSet->SetOxigen(Oxigen);
	}
}

void APMBaseCharacter::PlayAnimationMulticast(UAnimMontage* Animation3P, UAnimMontage* Animation1P, USkeletalMeshComponent* AnimMesh1P, USkeletalMeshComponent* AnimMesh3P)
{
	if (HasAuthority())
	{
		// Play animation on the server
		ServerPlayAnimation(Animation3P, AnimMesh3P);
	}

	// Play first-person animation locally on the client
	if (Animation1P && AnimMesh1P)
	{
		UAnimInstance* AnimInstance1P = AnimMesh1P->GetAnimInstance();
		if (AnimInstance1P && !AnimInstance1P->Montage_IsPlaying(Animation1P))
		{
			AnimInstance1P->Montage_Play(Animation1P);
		}
	}
}

void APMBaseCharacter::ServerPlayAnimation_Implementation(UAnimMontage* Animation3P, USkeletalMeshComponent* AnimMesh3P)
{
	MulticastPlayAnimation(Animation3P, AnimMesh3P);
}

void APMBaseCharacter::MulticastPlayAnimation_Implementation(UAnimMontage* Animation3P, USkeletalMeshComponent* AnimMesh3P)
{
	if (Animation3P && AnimMesh3P)
	{
		UAnimInstance* AnimInstance3P = AnimMesh3P->GetAnimInstance();
		if (AnimInstance3P && !AnimInstance3P->Montage_IsPlaying(Animation3P))
		{
			// Bind the events
			AnimInstance3P->OnMontageEnded.AddDynamic(this, &APMBaseCharacter::HandleMontageEnded);
			AnimInstance3P->OnMontageBlendingOut.AddDynamic(this, &APMBaseCharacter::HandleMontageBlendedOut);
			AnimInstance3P->Montage_Play(Animation3P);
		}
	}
}

void APMBaseCharacter::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnCustomMontageEnded.Broadcast();

	// Unbind the events
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &APMBaseCharacter::HandleMontageEnded);
	}
}

void APMBaseCharacter::HandleMontageBlendedOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		OnCustomMontageInterrupted.Broadcast();
	}
	else
	{
		OnCustomMontageBlendedOut.Broadcast();
	}

	// Unbind the events
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &APMBaseCharacter::HandleMontageBlendedOut);
	}
}












/*
EGDHitReactDirection APMBaseCharacter::GetHitReactDirection(const FVector& ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EGDHitReactDirection::Front;
		}
		else
		{
			return EGDHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EGDHitReactDirection::Right;
		}
		else
		{
			return EGDHitReactDirection::Left;
		}
	}

	return EGDHitReactDirection::Front;
}

bool APMBaseCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}


void APMBaseCharacter::PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser)
{
	if (IsAlive())
	{
		if (HitDirection == HitDirectionLeftTag)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Left);
		}
		else if (HitDirection == HitDirectionFrontTag)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Front);
		}
		else if (HitDirection == HitDirectionRightTag)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Right);
		}
		else if (HitDirection == HitDirectionBackTag)
		{
			ShowHitReact.Broadcast(EGDHitReactDirection::Back);
		}
	}
}

bool APMBaseCharacter::PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser)
{
	return true;
}

void APMBaseCharacter::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
}

void APMBaseCharacter::FinishDying()
{
	Destroy();
}
*/