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
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("0");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton1()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("1");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton2()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("2");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton3()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("3");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton4()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("4");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton5()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("5");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton6()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("6");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton7()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("7");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton8()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("8");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickButton9()
{
	PlayButtonPressedSoundEffect();
	UserInput += TEXT("9");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickClearButton()
{
	PlayButtonPressedSoundEffect();
	UserInput = TEXT("");
	UserInputText = FText::FromString(UserInput);
	InputText->SetText(UserInputText);
}

void USafeDoorWidget::ClickEnterButton()
{
	if (OwningSafe)
	{
		PlayButtonPressedSoundEffect();
		if (InputText && InputText->GetText().ToString() == OwningSafe->GetSafeCode())
		{
			//FPC->OpenSafeDoor(OwningSafe->GetSafeCode(), OwningSafe); // get rid of
			//OwningSafe->SetIsOpen(true);
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
	UKismetSystemLibrary::PrintString(this, TEXT("CLICKED BUTTON"), true, true, FColor::Cyan, 5.0f);

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	FPC->CloseSafepanelWidget(OwningSafe);
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


