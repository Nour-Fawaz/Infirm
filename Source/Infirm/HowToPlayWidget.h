// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HowToPlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UHowToPlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;

private:
	virtual void NativeConstruct() override;

	class AFirstPersonController* FPC;

	UFUNCTION()
	void BackButtonOnClicked();
};
