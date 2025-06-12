// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UNoteWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	bool bOpenNote = false;
	void SetText(class APickable* CurrentPickable);

protected:


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ActionButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RetText;
	

private:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void ActionButtonOnClicked();

	class AFirstPersonController* FPC;

	//sound effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* NoteSoundEffect;
	UPROPERTY()
	class UAudioComponent* MusicAudioComponent;
	
};
