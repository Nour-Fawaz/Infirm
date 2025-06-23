// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Searchable.generated.h"


UCLASS()
class INFIRM_API ASearchable : public AActor
{
	GENERATED_BODY()
	
public:	
	ASearchable();

protected:
	virtual void BeginPlay() override;

	//----------------------------------EVENTS--------------------------------------------
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void Tick(float DeltaTime) override;

	//----------------------------------PUBLIC ACTIONS AND ATTRIBUTES--------------------------------------------
	void SearchItem();
	bool Searched = false;
	void DestroyAllWidgets();
	UPROPERTY(EditAnywhere, Category = "Hidden Item")
	bool HasHiddenItem = false;

private:
	//----------------------------------COMPONENTS--------------------------------------------
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Meshes", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SearchableMesh;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Capsule Component", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	
	//----------------------------------HIDDEN ITEM--------------------------------------------
	UPROPERTY(EditInstanceOnly, Category = "Hidden Item", meta = (AllowPrivateAccess = "true"))
	FName HiddenItemTag;

	//----------------------------------AUDIO--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* SearchSoundEffect;

	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDisplayWidget> DisplayWidgetClass;
	UPROPERTY()
	class UDisplayWidget* PickUpWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	bool HasHiddenMessage = false;

	//----------------------------------PLAYER--------------------------------------------
	class AFirstPersonController* FPC;
};