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
function: retreives text files for current pickable
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
			"I write thee with a heavy heart. Thy sister Isabelle’s affliction groweth worse by the hour. I fear there is little more I may do to ease her sufferings. Word hath come that, come the morrow at first light, they shall arrive to gather the sick, and with them shall they take her.\n\n"
			"Forgive me that I send this with such haste, for the news hath only just reached mine ears.\n\n"
			"There is no hope left, Mary, save one. She would not reveal unto me the place where she did secret the slip bearing the prayer. Yet I do recall the ingredients required. Lose not a moment, begin thy work as soon as this letter finds thee.\n\n"
			"Ever thy faithful and watchful servant,\n"
			"Jonah\n\n"
			"The ingredients be these:\n"
			"    - One Candle, pure and unblemished\n"
			"    - A pinch of salt\n"
			"    - A token belonging to the afflicted,\n"
			"      something once held dear"
		)));
		
	}
	else if (CurrentPickable->ActorHasTag("DiningRoom"))
	{
		FilePath += "Texts/DiningRoom.txt";
		RetText->SetText(FText::FromString(TEXT(
			"To my Most Dear and Esteemed Mary,                 04/92\n\n"
			"I pray this letter findeth thee in health and cheerful spirit. Hath the willow by thy window yet put forth her leaves of crimson, amber, and green? Doth the firmament now shine clear, its clouds dispersed, casting warm rays upon the wildflowers thou didst bid grow behind thy dwelling?\n\n"
			"Alas, such blessings have not visited us here. Though many weeks have passed, still do I mark the moments with longing, awaiting the hour of our return. Master Griggs hath made known unto me that we shall soon depart this barren place, and make haste for home. I pray it be sooner rather than later.\n\n"
			"Ever thy faithful and devoted friend,\n"
			"Jonah"
		)));

	}
	else if (CurrentPickable->ActorHasTag("Kitchen"))
	{
		FilePath += "Texts/Kitchen.txt";
		RetText->SetText(FText::FromString(TEXT(
			"To my well-beloved friend Mary,                           05/92\n\n"
			"I pray thou wilt forgive me for not having written thee sooner. The days here have been filled with unrest. Master Griggs hath been sorely burdened, tending to several women and young maids struck with strange afflictions for which he can find no earthly cause. I have offered what aid I can, mixing draughts, preparing salves, and giving comfort where I am able. Yet, for all our labour, little good hath come of it.\n\n"
			"Of late, I have heard Master Griggs speak in troubling tones when he thinks himself alone. I know not what weighs so heavy on his thoughts, but there is a darkness that seems to grow about him. I fear what may yet come.\n\n"
			"But enough of my woes, tell me, how doth thy sister? In thy last letter, thou didst say her health declined. I pray her condition hath not worsened further. Do write me with haste and let me know how she fares.\n\n"
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
	UKismetSystemLibrary::PrintString(this, TEXT("CLICKED BUTTON"), true, true, FColor::Cyan, 5.0f);

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	FPC->CloseNoteWidget();
}


