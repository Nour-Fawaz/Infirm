// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"
#include "Components/Widget.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Pickable.h"


/*
function: sets text for current pickable
*/
void UNoteWidget::SetText(APickable* CurrentPickable)
{
	FString FileContent;
	FString FilePath = FPaths::ProjectContentDir();
	if (CurrentPickable->ActorHasTag("SpawnRoom"))
	{
		FilePath += "Texts/SpawnRoom.txt";
		RetText->SetText(FText::FromString(TEXT(
			"To my Dearest Mary,                                                 06/92\n\n"
			"I write to you with a heavy heart. Your sister Isabelle is growing worse by the hour, and I fear there is little more I can do to ease her suffering. Word has come that, at first light tomorrow, they will arrive to collect the sick, and they will take her with them.\n\n"
			"Forgive me for writing in such haste, but I only just received this news.\n\n"
			"There is no hope left, Mary, except one. She wouldn't tell me where she hid the book with the prayer. It's somewhere hard to reach. You'll need it for the ritual. But I do remember the ingredients. Don't waste a moment, begin your work as soon as this letter reaches you.\n\n"
			"Ever thy faithful and watchful servant,\n"
			"Jonah\n\n"
			"The ingredients be these:\n"
			"    - One Candle, pure and unblemished\n"
			"    - A bottle of salt\n"
			"    - A token belonging to the afflicted,\n"
			"      something once held dear"
		)));
		
	}
	else if (CurrentPickable->ActorHasTag("DiningRoom"))
	{
		FilePath += "Texts/DiningRoom.txt";
		RetText->SetText(FText::FromString(TEXT(
			"To my Most Dear and Esteemed Mary,                 04/92\n\n"
			"I hope this letter finds thee in good health and merry spirit. Hath the willow by thy window begun to show its leaves of red, yellow, and green? Doth the sky now shine clear, the clouds having passed, and the sun casting its warmth upon the wildflowers thou didst plant behind thy home?\n\n"
			"Alas, we have not been so blessed. Though many weeks have gone by, I still count the days, longing for the time of our return. Master Griggs hath said that we shall soon leave this desolate place and make our way home. I pray it be sooner rather than later.\n\n"
			"It is near six months since our six-month anniversary, that day still liveth bright in my memory. How I wish I could return to it and live it again. Dost thou still remember the grandfather clock I gave thy parents that day? Tell me, do they still use it?\n\n"
			"Ever thy faithful and devoted friend,\n"
			"Jonah"
		)));

	}
	else if (CurrentPickable->ActorHasTag("Kitchen"))
	{
		FilePath += "Texts/Kitchen.txt";
		RetText->SetText(FText::FromString(TEXT(
			"To my well-beloved friend Mary,                           05/92\n\n"
			"Please forgive me for not writing sooner. The days here have been filled with unrest. Master Griggs has been heavily burdened, caring for several women and young girls who have fallen ill with strange symptoms he cannot explain. I've done what I can, mixing remedies, preparing salves, and offering comfort, but despite our efforts, there has been little improvement.\n\n"
			"Lately, I've heard Master Griggs speak in troubling ways when he thinks no one is listening. I do not know what weighs on him so, but a kind of darkness seems to surround him. I fear what may happen next.\n\n"
			"But enough of my troubles, how is thy sister? In thy last letter, thou said her health was declining. I hope and pray she has not worsened. Please write to me soon and let me know how she fares.\n\n"
			"And Mary, happy anniversary. I did not forget. I only wish I could have spent the day with thee.\n\n"
			"With a heart turned ever toward thee,\n"
			"Jonah"
		)));
	}
}

void UNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPC->SetShowMouseCursor(true);
		FPC->SetInputMode(FInputModeUIOnly());
	}
	if (ActionButton)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("action button works"), true, true, FColor::Cyan, 5.0f);
		ActionButton->OnClicked.AddDynamic(this, &UNoteWidget::ActionButtonOnClicked);
	}
	if (NoteSoundEffect)
	{
		MusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), NoteSoundEffect, 0.4f);
		if (MusicAudioComponent)
		{
			MusicAudioComponent->bIsUISound = true;
			MusicAudioComponent->Play();
		}
	}
}

void UNoteWidget::ActionButtonOnClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Closed Note Widget"));

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	FPC->CloseNoteWidget();
}


