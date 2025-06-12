// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UPlayerTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	bool bDisplayText = false;
	void SetText(FName setText);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ActionButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RetText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ImageContainer;


private:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void ActionButtonOnClicked();

	class AFirstPersonController* FPC;

	UFUNCTION()
	void SetImage(FString ImageURL);
	
};
