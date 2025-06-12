// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EndMessageWidget.h"
#include "FirstPersonPlayer.h"
#include "Blueprint/UserWidget.h"

void AEndGameModeBase::BeginPlay()
{

	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		// Optional: store reference to your custom player class
		FPP = Cast<AFirstPersonPlayer>(PC->GetPawn());
	}

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // Remove "UEDPIE_0_" or similar

	if (CurrentLevel == "EndGame" && EndMessageWidgetClass)
	{
		EndMessageWidget = CreateWidget<UEndMessageWidget>(GetWorld(), EndMessageWidgetClass);
		if (EndMessageWidget)
		{
			EndMessageWidget->AddToViewport();
			EndMessageWidget->PlayFadeIn(); // Optional, can be done in NativeConstruct

			// Show mouse and lock UI input
			PC->SetShowMouseCursor(true);

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(EndMessageWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);

			if (EndMusic)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), EndMusic, 0.4f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("EndMusic is NULL!"));
			}
		}
	}
}
