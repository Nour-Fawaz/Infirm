// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "EndMessageWidget.h"
#include "FirstPersonPlayer.h"
#include "Blueprint/UserWidget.h"

void AEndGameModeBase::BeginPlay()
{

	Super::BeginPlay();

	//get first person player
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());
	}

	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	//check level then display end game widget
	if (CurrentLevel == "EndGame" && EndMessageWidgetClass)
	{
		EndMessageWidget = CreateWidget<UEndMessageWidget>(GetWorld(), EndMessageWidgetClass);
		if (EndMessageWidget)
		{
			EndMessageWidget->AddToViewport();

			// Show mouse and lock UI input
			APC->SetShowMouseCursor(true);

			//create input mode for current level
			FInputModeUIOnly EndGameInputMode;
			EndGameInputMode.SetWidgetToFocus(EndMessageWidget->TakeWidget()); //display EndMessage widget
			EndGameInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //allow mouse input
			APC->SetInputMode(EndGameInputMode); //set input mode for player

			//play end game audio
			if (EndMusic)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), EndMusic, 0.5f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("EndMusic is null"));
			}
		}
	}
}
