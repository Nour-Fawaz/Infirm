// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class INFIRM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//----------------------------------BUTTONS--------------------------------------------
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlayButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HowToPlayButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

private:
	virtual void NativeConstruct() override;

	//----------------------------------PLAYER--------------------------------------------
	class AFirstPersonController* FPC;

	//----------------------------------BUTTON FUNCTIONS--------------------------------------------
	UFUNCTION()
	void PlayButtonOnClicked();
	UFUNCTION()
	void HowToPlayButtonOnClicked();
	UFUNCTION()
	void ExitButtonOnClicked();

	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UHowToPlayWidget> HowToPlayWidgetClass;
	UPROPERTY()
	class UHowToPlayWidget* HowToPlayWidget;

	//----------------------------------AUDIO--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* MainMenuMusic;
	UPROPERTY()
	class UAudioComponent* MusicAudioComponent;
};