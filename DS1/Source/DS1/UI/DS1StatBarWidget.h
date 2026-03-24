// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DS1StatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UDS1StatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDS1StatBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativePreConstruct() override;

public:
	void SetRatio(float Ratio) const;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> StatBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBar)
	FLinearColor FillColorAndOpacity = FLinearColor::Red;
};
