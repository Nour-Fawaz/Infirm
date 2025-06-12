// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"
#include "Components/Widget.h"

void UPauseWidget::NativeConstruct()
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPC->SetShowMouseCursor(true);
		FPC->SetInputMode(FInputModeUIOnly());
	}
	if (ResumeButton)
	{
		UE_LOG(LogTemp, Error, TEXT("Resume button works"));
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::ResumeButtonOnClicked);
	}
	if (MainMenuButton)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenu Button works"));
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseWidget::MainMenuButtonOnClicked);
	}
	if (ExitButton)
	{
		UE_LOG(LogTemp, Error, TEXT("Exit Button works"));
		ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::ExitButtonOnClicked);
	}
}

void UPauseWidget::ResumeButtonOnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("RESUME BUTTON CLICKED"));

	if (FPC)
	{
		FPC->UnPauseGame();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FPC was nullptr in ResumeButtonOnClicked!"));
	}
}

void UPauseWidget::MainMenuButtonOnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Going TO MAIN MENU"));

	if (FPC)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		FPC->SetInputMode(FInputModeGameOnly());
		FPC->SetShowMouseCursor(false);
	}

	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

void UPauseWidget::ExitButtonOnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
