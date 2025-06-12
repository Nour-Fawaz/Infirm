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
	// Sets default values for this actor's properties
	ASearchable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SearchItem(UStaticMeshComponent* CurrentEquippedItem, bool& HasEquippedItem);

	bool Searched = false;
	void DestroyAllWidgets();
	

	UPROPERTY(EditAnywhere, Category = "Hidden Item")
	bool HasHiddenItem = false;

private:
	//components
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Meshes", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SearchableMesh;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Capsule Component", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	
	//hidden Item
	UPROPERTY(EditInstanceOnly, Category = "Hidden Item", meta = (AllowPrivateAccess = "true"))
	FName HiddenItemTag;

	//sound effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* SearchSoundEffect;

	//widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDisplayWidget> DisplayWidgetClass;
	UPROPERTY()
	class UDisplayWidget* PickUpWidget;
	UFUNCTION()
	void CheckSearchableWidget(ASearchable* CurrentsSearchable);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	bool HasHiddenMessage = false;

	//player
	class AFirstPersonController* FPC;
};
