

#include "AnimNotifies/UAnimNotify_GameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

void UUAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), Payload.EventTag, Payload);
}
