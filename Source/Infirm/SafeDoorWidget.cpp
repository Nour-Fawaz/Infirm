// Fill out your copyright notice in the Description page of Project Settings.


#include "SafeDoorWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "SafeDoor.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"


/*
* function: sets safe door this widget is active on
*/
void USafeDoorWidget::SetSafeDoor(ASafeDoor* NewSafeDoor)
{
	if (NewSafeDoor)
	{
		OwningSafe = NewSafeDoor;
		UE_LOG(LogTemp, Display, TEXT("OwningSafe Door set in SafeDoor Widget"))
	}
}

void USafeDoorWidget::NativeConstruct()
{

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPC->SetShowMouseCursor(true);
		FPC->SetInputMode(FInputModeUIOnly());
	}
	if (Button0)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 0 Works"))
		Button0->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton0);
	}
	if (Button1)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 1 Works"))
		Button1->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton1);
	}
	if (Button2)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 2 Works"))
			Button2->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton2);
	}
	if (Button3)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 3 Works"))
			Button3->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton3);
	}
	if (Button4)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 4 Works"))
			Button4->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton4);
	}
	if (Button5)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 5 Works"))
			Button5->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton5);
	}
	if (Button6)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 6 Works"))
			Button6->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton6);
	}
	if (Button7)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 7 Works"))
			Button7->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton7);
	}
	if (Button8)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 8 Works"))
			Button8->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton8);
	}
	if (Button9)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 9 Works"))
			Button9->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickButton9);
	}
	if (ClearButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Clear Button Works"))
			ClearButton->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickClearButton);
	}
	if (EnterButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Button 1 Works"))
			EnterButton->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickEnterButton);
	}
	if (ExitButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Exit Button Works"))
			ExitButton->OnClicked.AddDynamic(this, &USafeDoorWidget::ClickExitButton);
	}
}

void USafeDoorWidget::ClickButton0()
{
	InsertNumber(TEXT("0"));
}

void USafeDoorWidget::ClickButton1()
{
	InsertNumber(TEXT("1"));
}

void USafeDoorWidget::ClickButton2()
{
	InsertNumber(TEXT("2"));
}

void USafeDoorWidget::ClickButton3()
{
	InsertNumber(TEXT("3"));
}

void USafeDoorWidget::ClickButton4()
{
	InsertNumber(TEXT("4"));
}

void USafeDoorWidget::ClickButton5()
{
	InsertNumber(TEXT("5"));
}

void USafeDoorWidget::ClickButton6()
{
	InsertNumber(TEXT("6"));
}

void USafeDoorWidget::ClickButton7()
{
	InsertNumber(TEXT("7"));
}

void USafeDoorWidget::ClickButton8()
{
	InsertNumber(TEXT("8"));
}

void USafeDoorWidget::ClickButton9()
{
	InsertNumber(TEXT("9"));
}

void USafeDoorWidget::ClickClearButton()
{
	PlayButtonPressedSoundEffect();
	UserInput = TEXT("");
	NumOfButtonsPressed = 0;
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickEnterButton()
{
	if (OwningSafe)
	{
		PlayButtonPressedSoundEffect();
		if (InputText && InputText->GetText().ToString() == OwningSafe->GetSafeCode()) //if player guessed code is correct open safe
		{
			if (SafeOpenSoundEffect)
			{
				SafeOpenAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SafeOpenSoundEffect, 0.4f);
				if (SafeOpenAudioComponent)
				{
					SafeOpenAudioComponent->bIsUISound = true;
					SafeOpenAudioComponent->Play();
				}
			}
			OwningSafe->OpenSafeDoor();
		}
	}
}

void USafeDoorWidget::ClickExitButton()
{
	UE_LOG(LogTemp, Display, TEXT("Clicked Exit Button for SafeDoorWidget"));
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	FPC->CloseSafepanelWidget(OwningSafe);
}

/*
* function: inserts number called into widget display and counts input
*/
void USafeDoorWidget::InsertNumber(FString InputInt)
{
	if (NumOfButtonsPressed < 4)
	{
		PlayButtonPressedSoundEffect();
		UserInput += InputInt;
		UserInputText = FText::FromString(UserInput);
		InputText->SetText(UserInputText);
		NumOfButtonsPressed++;
	}
}

void USafeDoorWidget::PlayButtonPressedSoundEffect()
{
	if (ButtonPressedSoundEffect)
	{
		MusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), ButtonPressedSoundEffect, 0.4f);
		if (MusicAudioComponent)
		{
			MusicAudioComponent->bIsUISound = true;
			MusicAudioComponent->Play();
		}
	}
}