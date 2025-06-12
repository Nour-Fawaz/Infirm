// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "Pickable.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerTextWidget.h"
#include "SafeDoorWidget.h"
#include "SafeDoor.h"
#include "PauseWidget.h"
#include "MainMenuWidget.h"


void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FPP = Cast<AFirstPersonPlayer>(PC->GetPawn());
		
	}

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (CurrentLevel == "MainMenu")
	{
		// Show UI and mouse
		if (MainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();
				SetInputMode(FInputModeUIOnly());
				SetShowMouseCursor(true);
			}
		}
	}
	else
	{
		// Set input to game
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	
}

void AFirstPersonController::ReadNoteWidget(APickable* CurrentPickable)
{

	UKismetSystemLibrary::PrintString(this, TEXT("IN READ NOTE FPC"), true, true, FColor::Red, 5.0f);
	NoteWidget = CreateWidget<UNoteWidget>(this, NoteWidgetClass);
	if (NoteWidget)
	{
		NoteWidget->SetText(CurrentPickable);
		NoteWidget->AddToPlayerScreen();
		// Freeze player input
		if (FPP)
		{
			FPP->DisableInput(this); 
		}
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}

}

void AFirstPersonController::CloseNoteWidget()
{
	if (NoteWidget)
	{
		//give player input again
		if (FPP)
		{
			FPP->EnableInput(this); 
		}
		this->SetInputMode(FInputModeGameOnly());
		this->SetShowMouseCursor(false);
		NoteWidget->RemoveFromParent();
		NoteWidget = nullptr;
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, TEXT("NoteWidget NULL cannot remove"), true, true, FColor::Red, 5.0f);
	}
}

void AFirstPersonController::DisplayerPlayerTextWidget(FName Object)
{
	UKismetSystemLibrary::PrintString(this, TEXT("IN Player TEXT FPC"), true, true, FColor::Red, 5.0f);
	PlayerTextWidget = CreateWidget<UPlayerTextWidget>(this, PlayerTextWidgetClass);
	if (PlayerTextWidget)
	{
		PlayerTextWidget->SetText(Object);
		PlayerTextWidget->AddToPlayerScreen();
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}
}

void AFirstPersonController::ClosePlayerTextWidget()
{
	if (PlayerTextWidget)
	{
		this->SetInputMode(FInputModeGameOnly());
		this->SetShowMouseCursor(false);
		PlayerTextWidget->RemoveFromParent();
		PlayerTextWidget = nullptr;
	}
}

void AFirstPersonController::DisplaySafePanelWidget(ASafeDoor* SafeDoorPanel)
{
	UE_LOG(LogTemp, Display, TEXT("IN SAFE PANEL NOTE FPC"))
	SafePanelWidget = CreateWidget<USafeDoorWidget>(this, SafeDoorWidgetClass);
	SafePanelWidget->SetSafeDoor(SafeDoorPanel);
	if (SafePanelWidget)
	{
		SafePanelWidget->AddToPlayerScreen();
		// Freeze player input
		if (FPP)
		{
			FPP->DisableInput(this);
		}
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}
}

void AFirstPersonController::CloseSafepanelWidget(ASafeDoor* SafeDoor)
{
	UE_LOG(LogTemp, Display, TEXT("Attempting to close Safe Panel Widget"));
	if (SafePanelWidget)
	{
		//give player input again
		if (FPP)
		{
			FPP->EnableInput(this);
		}
		this->SetInputMode(FInputModeGameOnly());
		this->SetShowMouseCursor(false);
		SafePanelWidget->RemoveFromParent();
		SafePanelWidget = nullptr;
		SafeDoor->DestroyAllWidgets();
		UE_LOG(LogTemp, Display, TEXT("Removed Safe panel widget in FPC CloseSafepanelWidget"));
		DestroyAllWidgets();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("SafePanelWidget NULL cannot remove"))
	}
}

void AFirstPersonController::DisplayPickUpWidget(APickable* Pickable)
{
	if (!DisplayWidgetClass || !Pickable)
		return;

	if (PickUpWidget == nullptr)
	{
		PickUpWidget = CreateWidget<UDisplayWidget>(this, DisplayWidgetClass);
		PickUpWidget->AddToViewport();
	}

	FocusedPickable = Pickable;

	if (Pickable->ActorHasTag("PickUp"))
	{
		PickUpWidget->SetText(0);
	}
	else if (Pickable->ActorHasTag("Note"))
	{
		PickUpWidget->SetText(5);
	}
	else if (Pickable->ActorHasTag("Searched"))
	{
		PickUpWidget->SetText(4);
	}
	else if (Pickable->ActorHasTag("Search"))
	{
		PickUpWidget->SetText(1);
	}
}

void AFirstPersonController::ClosePickUpWidget()
{
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent();
		PickUpWidget = nullptr;
	}

	FocusedPickable = nullptr;
}

//void AFirstPersonController::OpenSafeDoor(FString Code, ASafeDoor* SafeDoor)
//{
//	//opens safe door
//	this->CloseSafepanelWidget();
//	SafeDoor->DestroyAllWidgets();
//	FName CodeFName = FName(*Code);
//	FPP->Tags.Add(CodeFName);
//
//	//need to tell safe it is open
//}

void AFirstPersonController::DestroyDisplayWidget()
{
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent();
		PickUpWidget = nullptr;
		UE_LOG(LogTemp, Display, TEXT("Destroyed Display Widget in FPC"))
	}
}

void AFirstPersonController::PauseGame()
{
	UE_LOG(LogTemp, Warning, TEXT("PAUSING GAME in FPC"));
	PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
	if (PauseWidget)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		PauseWidget->AddToPlayerScreen();
		FPP->SetGamePaused(true);

		// Set input mode to Game and UI without locking the mouse
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);

		SetShowMouseCursor(true);
	}
}

void AFirstPersonController::UnPauseGame()
{
	UE_LOG(LogTemp, Warning, TEXT("UNPAUSING GAME in FPC"));
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (FPP)
	{
		//FPP->EnableInput(this);
		FPP->SetGamePaused(false);
	}
	SetInputMode(FInputModeGameOnly());
	UE_LOG(LogTemp, Warning, TEXT("SetInputModeGameOnly called in UnPauseGame"));

	SetShowMouseCursor(false);

	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
		UE_LOG(LogTemp, Display, TEXT("Removed pause widget"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseWidget was nullptr"));
	}

	//DestroyAllWidgets(); // optional safety
}


void AFirstPersonController::DestroyAllWidgets()
{
	if (NoteWidget)
	{
		NoteWidget->RemoveFromParent();
		NoteWidget = nullptr;
	}

	if (PlayerTextWidget)
	{
		PlayerTextWidget->RemoveFromParent();
		PlayerTextWidget = nullptr;
	}

	if (SafePanelWidget)
	{
		SafePanelWidget->RemoveFromParent();
		SafePanelWidget = nullptr;
	}

	/*SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);*/
}

void AFirstPersonController::CheckExistingQidgets()
{
	if (NoteWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("NOTE WIDGET ALIVE"));
	}

	if (PlayerTextWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("PLAYER TEXT WIDGET ALIVE"));
	}

	if (SafePanelWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("SAFE PANEL WIDGET ALIVE"));
	}
	if (PickUpWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("PICK UP WIDGET ALIVE FPC"));
	}
}



/*
NOTE: We will treat the tags as an inventory but the BOOLS as constants. Meaning if the character got a key and used it, they still "have" the key.
*/
void AFirstPersonController::CheckInventory(APickable* PickedItem, bool bPicked)
{
	//picking up item
	if (bPicked)
	{
		UE_LOG(LogTemp, Display, TEXT("Item Picked Up: %s"), *PickedItem->GetName());
		if (PickedItem->ActorHasTag("Candle"))
		{
			HaveCandle = true;
			const FString msg = FString::Printf(TEXT("Candle Bool Status: %d"), HaveCandle);
			UKismetSystemLibrary::PrintString(this, *msg, true, true, FColor::Cyan, 5.0f);
		}
		else if (PickedItem->ActorHasTag("Possession"))
		{
			HavePossession = true;
			const FString msg = FString::Printf(TEXT("Possession Bool Status: %d"), HavePossession);
			UKismetSystemLibrary::PrintString(this, *msg, true, true, FColor::Cyan, 5.0f);
		}
		else if (PickedItem->ActorHasTag("Incantation"))
		{
			HaveIncantation = true;
			const FString msg = FString::Printf(TEXT("Incantation Bool Status: %d"), HaveIncantation);
			UKismetSystemLibrary::PrintString(this, *msg, true, true, FColor::Cyan, 5.0f);
		}
		else if (PickedItem->ActorHasTag("KitchenKey"))
		{
			HaveKitchenKey = true;
			const FString msg = FString::Printf(TEXT("kitchen key Bool Status: %d"), HaveKitchenKey);
			UKismetSystemLibrary::PrintString(this, *msg, true, true, FColor::Cyan, 5.0f);
			if (FPP)
			{
				FPP->Tags.Add("KitchenKey");
			}
		}
		else if (PickedItem->ActorHasTag("Salt"))
		{
			HaveSalt = true;
			const FString msg = FString::Printf(TEXT("Salt Bool Status: %d"), HaveSalt);
			UKismetSystemLibrary::PrintString(this, *msg, true, true, FColor::Cyan, 5.0f);
		}
		else if (PickedItem->ActorHasTag("SafeKey"))
		{
			HaveSafeKey = true;
			const FString msg = FString::Printf(TEXT("SafeKey Bool Status: %d"), HaveSafeKey);
			UKismetSystemLibrary::PrintString(this, *msg, true, true, FColor::Cyan, 5.0f);
			if (FPP)
			{
				FPP->Tags.Add("SafeKey");
			}
		}
		
	}
	//using item 
	else
	{
		if (PickedItem->ActorHasTag("Candle"))
		{
			HaveCandle = false;
			UE_LOG(LogTemp, Display, TEXT("Candle bool status: %d"), HaveCandle);
		}
		else if (PickedItem->ActorHasTag("Possession"))
		{
			HavePossession = false;
			UE_LOG(LogTemp, Display, TEXT("Possession bool status: %d"), HavePossession);
		}
		else if (PickedItem->ActorHasTag("Incantation"))
		{
			HaveIncantation = false;
			UE_LOG(LogTemp, Display, TEXT("Incantation bool status: %d"), HaveIncantation);
		}
		else if (PickedItem->ActorHasTag("Salt"))
		{
			HaveSalt = false;
			UE_LOG(LogTemp, Display, TEXT("Salt bool status: %d"), HaveSalt);
		}
		else if (PickedItem->ActorHasTag("KitchenKey"))
		{
			HaveKitchenKey = false;
			if (FPP)
			{
				FPP->Tags.Remove(FName("KitchenKey"));
			}
			UE_LOG(LogTemp, Display, TEXT("KitchenKey bool status: %d"), HaveKitchenKey);
		}
		else if (PickedItem->ActorHasTag("SafeKey"))
		{
			HaveSafeKey = false;
			if (FPP)
			{
				FPP->Tags.Remove(FName("SafeKey"));
			}
			UE_LOG(LogTemp, Display, TEXT("SafeKey bool status: %d"), HaveKitchenKey);
		}
		/*else if (PickedItem->ActorHasTag("GarageKey"))
		{
			if (FPP)
				{
					FPP->Tags.Remove("GarageKey");
				}
			UE_LOG(LogTemp, Display, TEXT("GarageKey removed from inventory"));
		}
		else if (PickedItem->ActorHasTag("KitchenKey"))
		{
			if (FPP)
			{
				FPP->Tags.Remove("GarageKey");
			}
			UE_LOG(LogTemp, Display, TEXT("KitchenKey removed form inventory"));
		}*/
	}


}

void AFirstPersonController::CheckInventoryName(const FName& NewItem)
{
	if (NewItem == "GarageKey")
	{
		HaveGarageKey = true;
		if (FPP)
		{
			FPP->Tags.Add("GarageKey");
		}
		
	}
	if (NewItem == "SafeKey")
	{
		HaveSafeKey = true;
		if (FPP)
		{
			FPP->Tags.Add("SafeKey");
		}
	}

}

void AFirstPersonController::RemoveFromInventory(const FName ItemToRemove)
{
	if (ItemToRemove == "Candle")
	{
		HaveCandle = false;
		UE_LOG(LogTemp, Display, TEXT("Candle bool status: %d"), HaveCandle);

	}
	else if (ItemToRemove == "Possession")
	{
		HavePossession = false;
		UE_LOG(LogTemp, Display, TEXT("Possession bool status: %d"), HavePossession);
	}
	else if (ItemToRemove == "Salt")
	{
		HaveSalt = false;
		UE_LOG(LogTemp, Display, TEXT("Salt bool status: %d"), HaveSalt);
	}
	else if (ItemToRemove == "Incantation")
	{
		HaveIncantation = false;
		UE_LOG(LogTemp, Display, TEXT("Incantation bool status: %d"), HaveIncantation);

	}
	else if (ItemToRemove == "KitchenKey")
	{
		//HaveKitchenKey = false;
		if (FPP)
		{
			FPP->Tags.Remove(FName("KitchenKey"));
		}
		UE_LOG(LogTemp, Display, TEXT("KitchenKey bool status: %d"), HaveKitchenKey);
	}
	else if (ItemToRemove == "GarageKey")
	{
		//HaveGarageKey = false;
		if (FPP)
		{
			FPP->Tags.Remove(FName("GarageKey"));
		}
		UE_LOG(LogTemp, Display, TEXT("GarageKey bool status: %d"), HaveGarageKey);
	}
}

bool AFirstPersonController::InInventory(FName ItemToCheck)
{
	if (ItemToCheck == "Candle")
	{
		return HaveCandle;
	}
	else if (ItemToCheck == "Possession")
	{
		return HavePossession;
	}
	else if (ItemToCheck == "Incantation")
	{
		return HaveIncantation;
	}
	else if (ItemToCheck == "Salt")
	{
		return HaveSalt;
	}
	else if (ItemToCheck == "KitchenKey")
	{
		return HaveKitchenKey;
	}
	else if (ItemToCheck == "GarageKey")
	{
		return HaveGarageKey;
	}

	return false;
}
