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
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

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
		// Set input for game
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	
}

void AFirstPersonController::ReadNoteWidget(APickable* CurrentPickable)
{
	UE_LOG(LogTemp, Display, TEXT("In readNoteWidget FPC for: %s"), *CurrentPickable->GetName());
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
		UE_LOG(LogTemp, Warning, TEXT("NoteWidget NULL cannot remove"));
	}
}

void AFirstPersonController::DisplayerPlayerTextWidget(FName Object)
{
	UE_LOG(LogTemp, Display, TEXT("DisplayerPlayerTextWidget called in FPC for: %s"), *Object.ToString());
	PlayerTextWidget = CreateWidget<UPlayerTextWidget>(this, PlayerTextWidgetClass);
	if (PlayerTextWidget)
	{
		PlayerTextWidget->SetText(Object);
		PlayerTextWidget->AddToPlayerScreen();
		// Freeze player input
		if (FPP)
		{
			FPP->DisableInput(this);
		}
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}
}

void AFirstPersonController::ClosePlayerTextWidget()
{
	if (PlayerTextWidget)
	{
		if (FPP)
		{
			FPP->EnableInput(this);
		}
		this->SetInputMode(FInputModeGameOnly());
		this->SetShowMouseCursor(false);
		PlayerTextWidget->RemoveFromParent();
		PlayerTextWidget = nullptr;
	}
}

void AFirstPersonController::DisplaySafePanelWidget(ASafeDoor* SafeDoorPanel)
{
	UE_LOG(LogTemp, Display, TEXT("In DisplaySafePanelWidget FPC"))
	SafePanelWidget = CreateWidget<USafeDoorWidget>(this, SafeDoorWidgetClass);
	//assign SafeDoor for widget
	SafePanelWidget->SetSafeDoor(SafeDoorPanel);
	if (SafePanelWidget)
	{
		SafePanelWidget->AddToPlayerScreen();
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
		UE_LOG(LogTemp, Display, TEXT("SafePanelWidget null can't remove"))
	}
}

void AFirstPersonController::DisplayPickUpWidget(APickable* Pickable)
{
	//if widget class or pickable invalid return
	if (!DisplayWidgetClass || !Pickable)
		return;

	//if pick up widget doesnt exist create
	if (!PickUpWidget)
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

/*
* function: sets text in display widget without pickable
*/
void AFirstPersonController::DisplayWidgetTextByInt(int textVal)
{
	if (!DisplayWidgetClass)
		return;

	if (PickUpWidget == nullptr)
	{
		PickUpWidget = CreateWidget<UDisplayWidget>(this, DisplayWidgetClass);
		PickUpWidget->SetText(textVal);
		PickUpWidget->AddToViewport();
	}
}

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
	UE_LOG(LogTemp, Warning, TEXT("Pausing game in FPC"));
	DestroyDisplayWidget();
	DestroyAllWidgets();
	PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
	if (PauseWidget)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		PauseWidget->AddToPlayerScreen();
		FPP->SetGamePaused(true);

		//set input mode to Game and UI while allowing movement of mouse
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	}
}

void AFirstPersonController::UnPauseGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Unpausing game in FPC"));
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (FPP)
	{
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
		UE_LOG(LogTemp, Warning, TEXT("PauseWidget was null"));
	}
}

/*
* function: destroys all widgets except pause, main menu and display widget
*/
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
}

/*
* function: checks for alive widgets
*/
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
	if (PauseWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("PAUSE WIDGET ALIVE FPC"));
	}
	if (MainMenuWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("MAIN MENU WIDGET ALIVE FPC"));
	}
}



/*
* function: adds/removes item from inventory through pickables
* NOTE: Tags will be treated as part of Player Inventory. Booleans will be constants (once an item is obtained it will remain).
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
			UE_LOG(LogTemp, Display, TEXT("Candle Bool Status: %d"), HaveCandle);
		}
		else if (PickedItem->ActorHasTag("Possession"))
		{
			HavePossession = true;
			UE_LOG(LogTemp, Display, TEXT("Possession Bool Status: %d"), HavePossession);
		}
		else if (PickedItem->ActorHasTag("Incantation"))
		{
			HaveIncantation = true;
			UE_LOG(LogTemp, Display, TEXT("Incantation Bool Status: %d"), HaveIncantation);
		}
		else if (PickedItem->ActorHasTag("KitchenKey"))
		{
			HaveKitchenKey = true;
			UE_LOG(LogTemp, Display, TEXT("KitchenKey Bool Status: %d"), HaveKitchenKey);
			if (FPP)
			{
				FPP->Tags.Add("KitchenKey");
			}
		}
		else if (PickedItem->ActorHasTag("Salt"))
		{
			HaveSalt = true;
			UE_LOG(LogTemp, Display, TEXT("Salt Bool Status: %d"), HaveSalt);
		}
		else if (PickedItem->ActorHasTag("SafeKey"))
		{
			HaveSafeKey = true;
			UE_LOG(LogTemp, Display, TEXT("SafeKey Bool Status: %d"), HaveSafeKey);
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
	}
}

/*
* function: adds item to inventory through tags
*/
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

/*
* function: removes item from inventory through tags
*/
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
		if (FPP)
		{
			FPP->Tags.Remove(FName("KitchenKey"));
		}
		UE_LOG(LogTemp, Display, TEXT("KitchenKey bool status: %d"), HaveKitchenKey);
	}
	else if (ItemToRemove == "GarageKey")
	{
		if (FPP)
		{
			FPP->Tags.Remove(FName("GarageKey"));
		}
		UE_LOG(LogTemp, Display, TEXT("GarageKey bool status: %d"), HaveGarageKey);
	}
}

/*
* function: checks if item is in inventory
*/
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
