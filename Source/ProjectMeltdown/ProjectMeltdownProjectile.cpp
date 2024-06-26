// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectMeltdownProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BlueprintLibraries/ProjectMeltdownStatics.h"


static TAutoConsoleVariable<int32> CVarShowProjectiles(
	TEXT("ShowDebugProjectiles"),
	0,
	TEXT("Draws debug info about projectiles")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

AProjectMeltdownProjectile::AProjectMeltdownProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetReplicateMovement(true);
	bReplicates = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AProjectMeltdownProjectile::OnProjectileStop);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetIsReplicated(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	StaticMeshComponent->bReceivesDecals = false;
}

const UProjectileStaticData* AProjectMeltdownProjectile::GetProjectileStaticData() const
{
	if (IsValid(ProjectileDataClass))
	{
		return GetDefault<UProjectileStaticData>(ProjectileDataClass);
	}

	return nullptr;
}

void AProjectMeltdownProjectile::BeginPlay()
{
	Super::BeginPlay();

	const UProjectileStaticData* ProjectileData = GetProjectileStaticData();

	if (ProjectileData && ProjectileMovementComponent)
	{
		if (ProjectileData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(ProjectileData->StaticMesh);
		}

		ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
		ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = ProjectileData->GravityMultiplayer;

		ProjectileMovementComponent->Velocity = ProjectileData->InitialSpeed * GetActorForwardVector();

	}

	const int32 DebugShowProjectile = CVarShowProjectiles.GetValueOnAnyThread();

	if (DebugShowProjectile)
	{
		DebugDrawPath();
	}
}

void AProjectMeltdownProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	const UProjectileStaticData* ProjectileData = GetProjectileStaticData();

	if (ProjectileData)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileData->OnStopSFX, GetActorLocation(), 1.f);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileData->OnStopVFX, GetActorLocation());
	}

	Super::EndPlay(EndPlayReason);
}

void AProjectMeltdownProjectile::DebugDrawPath() const
{
	const UProjectileStaticData* ProjectileData = GetProjectileStaticData();

	if (ProjectileData)
	{
		FPredictProjectilePathParams PredictProjectilePathParams;
		PredictProjectilePathParams.StartLocation = GetActorLocation();
		PredictProjectilePathParams.LaunchVelocity = ProjectileData->InitialSpeed * GetActorForwardVector();
		PredictProjectilePathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
		PredictProjectilePathParams.bTraceComplex = true;
		PredictProjectilePathParams.bTraceWithCollision = true;
		PredictProjectilePathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
		PredictProjectilePathParams.DrawDebugTime = 3.f;
		PredictProjectilePathParams.OverrideGravityZ = ProjectileData->GravityMultiplayer == 0.f ? 0.0001f : ProjectileData->GravityMultiplayer;

		FPredictProjectilePathResult PredictProjectilePathResult;
		if (UGameplayStatics::PredictProjectilePath(this, PredictProjectilePathParams, PredictProjectilePathResult))
		{
			DrawDebugSphere(GetWorld(), PredictProjectilePathResult.HitResult.Location, 50, 10, FColor::Red);
		}
	}
}

void AProjectMeltdownProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	const UProjectileStaticData* ProjectileData = GetProjectileStaticData();

	if (ProjectileData)
	{
		UProjectMeltdownStatics::ApplyRadialDamage(this, GetOwner(), GetActorLocation(),
			ProjectileData->DamageRadius,
			ProjectileData->BaseDamage,
			ProjectileData->Effects,
			ProjectileData->RadialDamageQueryTypes,
			ProjectileData->RadialDamageTraceType);
	}

	Destroy();
}

void AProjectMeltdownProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectMeltdownProjectile, ProjectileDataClass);
}