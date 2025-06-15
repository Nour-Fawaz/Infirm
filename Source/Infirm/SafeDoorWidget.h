// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SafeDoorWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API USafeDoorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSafeDoor(class ASafeDoor* NewSafeDoor);

protected:

	//Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button0;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button5;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button6;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button7;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button8;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button9;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ClearButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* EnterButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

	//Code 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* InputText;
	FString UserInput;
	FText UserInputText;
	


private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ClickButton0();
	UFUNCTION()
	void ClickButton1();
	UFUNCTION()
	void ClickButton2();
	UFUNCTION()
	void ClickButton3();
	UFUNCTION()
	void ClickButton4();
	UFUNCTION()
	void ClickButton5();
	UFUNCTION()
	void ClickButton6();
	UFUNCTION()
	void ClickButton7();
	UFUNCTION()
	void ClickButton8();
	UFUNCTION()
	void ClickButton9();
	UFUNCTION()
	void ClickClearButton();
	UFUNCTION()
	void ClickEnterButton();
	UFUNCTION()
	void ClickExitButton();


	UFUNCTION()
	void InsertNumber(FString InputInt);
	UPROPERTY()
	int32 NumOfButtonsPressed = 0;

	class AFirstPersonController* FPC;
	class ASafeDoor* OwningSafe;

	//sound effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ButtonPressedSoundEffect;
	UPROPERTY()
	class UAudioComponent* MusicAudioComponent;
	void PlayButtonPressedSoundEffect();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* SafeOpenSoundEffect;
	UPROPERTY()
	class UAudioComponent* SafeOpenAudioComponent;
	
	
};
