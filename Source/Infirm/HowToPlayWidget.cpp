// Fill out your copyright notice in the Description page of Project Settings.


#include "HowToPlayWidget.h"
#include "Components/Button.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"
//#include "Components/Widget.h"

void UHowToPlayWidget::NativeConstruct()
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPC->SetShowMouseCursor(true);
		FPC->SetInputMode(FInputModeUIOnly());
	}
	if (BackButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Back button works"));
		BackButton->OnClicked.AddDynamic(this, &UHowToPlayWidget::BackButtonOnClicked);
	}
}

void UHowToPlayWidget::BackButtonOnClicked()
{
	RemoveFromParent();
}
