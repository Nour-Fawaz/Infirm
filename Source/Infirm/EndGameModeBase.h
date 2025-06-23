// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndGameModeBase.generated.h"


UCLASS()
class INFIRM_API AEndGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

private:
	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UEndMessageWidget> EndMessageWidgetClass;
	UPROPERTY()
	class UEndMessageWidget* EndMessageWidget;

	//----------------------------------PLAYER--------------------------------------------
	UPROPERTY()
	AFirstPersonPlayer* FPP;

	//----------------------------------MUSIC--------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* EndMusic;
};
