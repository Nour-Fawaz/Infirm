// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ResumeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* MainMenuButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

private:
	virtual void NativeConstruct() override;

	class AFirstPersonController* FPC;
	UFUNCTION()
	void ResumeButtonOnClicked();
	UFUNCTION()
	void MainMenuButtonOnClicked();
	UFUNCTION()
	void ExitButtonOnClicked();
	
};
