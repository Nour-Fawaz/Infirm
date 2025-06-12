// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlayButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HowToPlayButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

private:
	virtual void NativeConstruct() override;

	//player
	class AFirstPersonController* FPC;

	//button functions
	UFUNCTION()
	void PlayButtonOnClicked();
	UFUNCTION()
	void HowToPlayButtonOnClicked();
	UFUNCTION()
	void ExitButtonOnClicked();

	//widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UHowToPlayWidget> HowToPlayWidgetClass;
	UPROPERTY()
	class UHowToPlayWidget* HowToPlayWidget;

	//music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* MainMenuMusic;
	UPROPERTY()
	class UAudioComponent* MusicAudioComponent;
	
};
