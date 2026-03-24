// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Define.h"
#include "Blueprint/UserWidget.h"
#include "DS1PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UDS1PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDS1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

public:
	void OnAttributeChanged(EDS1AttributeType AttributeType, float InValue);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UDS1StatBarWidget> StaminaBarWidget;
};
