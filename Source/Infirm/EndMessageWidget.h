// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UEndMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    //----------------------------------PUBLIC ACTIONS--------------------------------------------
    UFUNCTION(BlueprintCallable)
    void PlayFadeIn();

    UFUNCTION(BlueprintCallable)
    void PlayFadeOut();

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnFadeOutFinished();

    //----------------------------------WIDGET ATTRIBUTES--------------------------------------------
    UPROPERTY(meta = (BindWidget))
    class UButton* ContinueButton;

private:

    //----------------------------------EFFECTS--------------------------------------------
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeIn;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOut;
	
};
