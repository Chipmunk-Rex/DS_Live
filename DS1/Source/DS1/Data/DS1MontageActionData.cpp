// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DS1MontageActionData.h"

UAnimMontage* UDS1MontageActionData::GetMontageGroupForTag(const FGameplayTag& GroupTag, const int32 Index)
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FDS1MontageGroup& MontageGroup = MontageGroupMap[GroupTag];

		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}

	return nullptr;
}
