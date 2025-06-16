// Fill out your copyright notice in the Description page of Project Settings.


#include "EndMessageWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

/*
* function: to be played when constructued
*/
void UEndMessageWidget::PlayFadeIn()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

/*
* function: to be played when ContinueButton is clicked
*/
void UEndMessageWidget::PlayFadeOut()
{
	if (FadeOut)
	{
		PlayAnimation(FadeOut);

		FWidgetAnimationDynamicEvent EndEvent;
		UE_LOG(LogTemp, Display, TEXT("FadeOut finished"));
		EndEvent.BindDynamic(this, &UEndMessageWidget::OnFadeOutFinished);
		BindToAnimationFinished(FadeOut, EndEvent);
	}
}

void UEndMessageWidget::NativeConstruct()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UEndMessageWidget::PlayFadeOut);
	}
}

/*
* function: once widget fades away open the main menu level
*/
void UEndMessageWidget::OnFadeOutFinished()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}
