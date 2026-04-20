// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_DS1AttackFinished.h"
#include "Characters/DS1Character.h"

UAnimNotify_DS1AttackFinished::UAnimNotify_DS1AttackFinished()
{
}

void UAnimNotify_DS1AttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADS1Character* OwnerCharacter = Cast<ADS1Character>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->AttackFinished(ComboResetDelay);
	}
}
