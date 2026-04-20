// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotifyState_DS1ComboWindow.h"
#include "Characters/DS1Character.h"

UAnimNotifyState_DS1ComboWindow::UAnimNotifyState_DS1ComboWindow()
{
}

void UAnimNotifyState_DS1ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ADS1Character* OwnerCharacter = Cast<ADS1Character>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->EnableComboWindow();
	}
}

void UAnimNotifyState_DS1ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ADS1Character* OwnerCharacter = Cast<ADS1Character>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->DisableComboWindow();
	}
}
