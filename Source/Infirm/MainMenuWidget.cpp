// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"
#include "Components/Widget.h"
#include "HowToPlayWidget.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

void UMainMenuWidget::NativeConstruct()
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPC->SetShowMouseCursor(true);
		FPC->SetInputMode(FInputModeUIOnly());
	}
	if (MainMenuMusic)
	{
		MusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MainMenuMusic, 0.4f);
		if (MusicAudioComponent)
		{
			MusicAudioComponent->bIsUISound = true;
			MusicAudioComponent->Play();
		}
	}
	if (PlayButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Play button works"));
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayButtonOnClicked);
	}
	if (HowToPlayButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("How To Play Button works"));
		HowToPlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HowToPlayButtonOnClicked);
	}
	if (ExitButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exit Button works"));
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitButtonOnClicked);
	}
}

void UMainMenuWidget::PlayButtonOnClicked()
{
	if (MusicAudioComponent)
	{
		MusicAudioComponent->FadeOut(1.0f, 0.0f); // Fade out over 1 second
	}
	UGameplayStatics::OpenLevel(this, FName("/Game/Maps/House"));
}

void UMainMenuWidget::HowToPlayButtonOnClicked()
{
	if (HowToPlayWidgetClass)
	{
		APlayerController* PC = GetOwningPlayer();
		UHowToPlayWidget* HowToPlay = CreateWidget<UHowToPlayWidget>(PC, HowToPlayWidgetClass);

		if (HowToPlay)
		{
			HowToPlay->AddToViewport();
		}
	}
}

void UMainMenuWidget::ExitButtonOnClicked()
{
	if (MusicAudioComponent)
	{
		MusicAudioComponent->FadeOut(1.0f, 0.0f); // Fade out over 1 second
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
