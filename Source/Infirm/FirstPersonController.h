// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NoteWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.generated.h"

UCLASS()
class INFIRM_API AFirstPersonController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//----------------------------------WIDGETS--------------------------------------------
	UFUNCTION()
	void ReadNoteWidget(class APickable* CurrentPickable);
	UFUNCTION()
	void CloseNoteWidget();

	UFUNCTION()
	void DisplayerPlayerTextWidget(FName Object);
	UFUNCTION()
	void ClosePlayerTextWidget();

	UFUNCTION()
	void DisplaySafePanelWidget(class ASafeDoor* SafeDoor);
	UFUNCTION()
	void CloseSafepanelWidget(class ASafeDoor* SafeDoor);

	UFUNCTION()
	void DisplayPickUpWidget(class APickable* Pickable);
	UFUNCTION()
	void ClosePickUpWidget();
	UPROPERTY()
    class APickable* FocusedPickable;

	UFUNCTION()
	void DisplayWidgetTextByInt(int textVal);
	UFUNCTION()
	void DestroyDisplayWidget();

	UFUNCTION()
	void PauseGame();
	UFUNCTION()
	void UnPauseGame();

	UFUNCTION()
	void DestroyAllWidgets();
	UFUNCTION()
	void CheckExistingQidgets();

	//----------------------------------INVENTORY--------------------------------------------
	UFUNCTION()
	void CheckInventory(class APickable* PickedItem, bool bPicked);
	UFUNCTION()
	void CheckInventoryName(const FName& NewItem);
	UFUNCTION()
	void RemoveFromInventory(const FName ItemToRemove);
	UFUNCTION()
	bool InInventory(FName ItemToCheck);
	UFUNCTION()
	void CheckInventoryByTag(TArray<FName>ItemTags, bool HaveItem);
	
private:
	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UNoteWidget> NoteWidgetClass;
	UPROPERTY()
	class UNoteWidget* NoteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerTextWidget> PlayerTextWidgetClass;
	UPROPERTY()
	class UPlayerTextWidget* PlayerTextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USafeDoorWidget> SafeDoorWidgetClass;
	UPROPERTY()
	class USafeDoorWidget* SafePanelWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDisplayWidget> DisplayWidgetClass;
	UPROPERTY()
	class UDisplayWidget* PickUpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPauseWidget> PauseWidgetClass;
	UPROPERTY()
	class UPauseWidget* PauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY()
	class UMainMenuWidget* MainMenuWidget;

	//----------------------------------INVENTORY--------------------------------------------
	UPROPERTY()
	bool HaveCandle = false;
	UPROPERTY()
	bool HavePossession = false;
	UPROPERTY()
	bool HaveIncantation = false;
	UPROPERTY()
	bool HaveGarageKey = false;
	UPROPERTY()
	bool HaveKitchenKey = false;
	UPROPERTY()
	bool HaveSalt = false;
	UPROPERTY()
	bool HaveSafeKey = false;
	
	//----------------------------------PLAYER--------------------------------------------
	UPROPERTY()
	AFirstPersonPlayer* FPP;
};
