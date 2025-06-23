// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTextWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonController.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Slate/SlateBrushAsset.h"


void UPlayerTextWidget::SetText(FName setText)
{
	if (setText == "KitchenDrawer")
	{
		RetText->SetText(FText::FromString("Aye, I cannot open it! It is fast shut. I shall need a tool to pry it loose."));
	}
	else if (setText == "FamilyPhoto")
	{
		RetText->SetText(FText::FromString("I do recall when this was taken... 'twas near four years past, if memory serves me right."));
		SetImage("/Game/Assets/Images/FamilyPhoto.FamilyPhoto");
	}
	else if (setText == "GarageKey")
	{
		RetText->SetText(FText::FromString("A screwdriver... mayhap I might use this to pick open a door."));
		SetImage("/Game/Assets/Images/LockPick.LockPick");
	}
	else if (setText == "BathroomPhoto")
	{
		RetText->SetText(FText::FromString("Such a fair painting... I recall them taking sweet Isabelle in a carriage much the same as that."));
	}
	else if (setText == "LivingRoomBookcase")
	{
		RetText->SetText(FText::FromString("Jonah gave me this book nearly a year ago, on the day we'd been together six months. It was such a joyful day, I think on it often. I wish we could have done the same last month, when our one anniversary year came, but these past months have brought such troubles, and things have gone badly since."));
		SetImage("/Game/Assets/Images/JonahBook.JonahBook");
	}
	else if (setText == "KitchenPantry")
	{
		RetText->SetText(FText::FromString("I did reckon the salt was hereabouts... 'tis ought to be somewhere near."));
	}
	else if (setText == "SpawnRoomSuitcase")
	{
		RetText->SetText(FText::FromString("Poor Isabelle... I did pack this in great haste to go to her side, but this morn I received word from Jonah. I must needs complete the ritual with all swiftness, she must be made well ere the night is through."));
	}
	else if (setText == "SisterPerfumeBathroom")
	{
		RetText->SetText(FText::FromString("I must not forget Isabelle's perfume... mayhap the scent shall bring her some comfort."));
	}
	else if (setText == "CurtainGarage")
	{
		RetText->SetText(FText::FromString("Hm... I've quite forgot what lies behind these curtains. 'Tis like to be naught of any good, I reckon."));
	}
	else if (setText == "GarageFoldedTable")
	{
		RetText->SetText(FText::FromString("It's been so long since I've been in here... I really ought to clean it up."));
	}
	else if (setText == "SisterRoomCloset")
	{
		RetText->SetText(FText::FromString("Her closet keeps growing more and more empty, for I must bring her more clothing each time I visit. This ritual must work... I dearly hope sweet Jonah wrote the right ingredients in his note."));
	}
	else if (setText == "AlarmClockJonah")
	{
		RetText->SetText(FText::FromString("Jonah brought Mother and Father a gift when first they met. I warned him a timepiece was an odd choice, but he thought they might like something a bit different."));
	}
	else if (setText == "SisterRoomPaint")
	{
		RetText->SetText(FText::FromString("I dearly miss the days when Isabelle would spend hours painting. She would only use the brush Mother and Father gave her for her birthday. I miss them... I miss them all so much."));
	}
}

void UPlayerTextWidget::NativeConstruct()
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (FPC)
	{
		FPC->SetShowMouseCursor(true);
		FPC->SetInputMode(FInputModeUIOnly());
	}
	if (ActionButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Action Button for PlayerTextWidget Works"));
		ActionButton->OnClicked.AddDynamic(this, &UPlayerTextWidget::ActionButtonOnClicked);
	}
}

void UPlayerTextWidget::ActionButtonOnClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Action button in PlayerTExtWidget pressed."));

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC); //get player controller
	FPC->ClosePlayerTextWidget(); 
}

void UPlayerTextWidget::SetImage(FString ImageURL)
{
	UE_LOG(LogTemp, Display, TEXT("Loading image for PlayerTextWidget"));
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, *ImageURL);
	if (Texture && ImageContainer)
	{
		UE_LOG(LogTemp, Display, TEXT("Image loaded for PlayerTextWidget"));
		FSlateBrush Brush;
		Brush.SetResourceObject(Texture);
		Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
		ImageContainer->SetBrush(Brush);
	}
}
