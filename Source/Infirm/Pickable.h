// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteWidget.h"
#include "Blueprint/UserWidget.h"
#include "Pickable.generated.h"

UCLASS(Blueprintable)
class INFIRM_API APickable : public AActor
{
	GENERATED_BODY()
	
public:	
	APickable();

	//----------------------------------PUBLIC ACTIONS--------------------------------------------
	UFUNCTION(BlueprintCallable)
	void PickUp();

protected:
	virtual void BeginPlay() override;

	//----------------------------------EVENTS--------------------------------------------
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void Tick(float DeltaTime) override;

	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	bool bDispNote = false;

	//----------------------------------WIDGETS--------------------------------------------
	UFUNCTION(BlueprintCallable)
	void DestroyAllWidgets();
	UFUNCTION(BlueprintCallable)
	TArray<FName> GetPickableTags(APickable* AHasTags);
	UFUNCTION(BlueprintCallable)
	void TransferActorTags(TArray<FName> TagsToTransfer, APickable* ATransferTo);

private:
	//----------------------------------COMPONENTS--------------------------------------------
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Meshes", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PickableMesh;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Capsule Component", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDisplayWidget> DisplayWidgetClass;
	UPROPERTY()
	class UDisplayWidget* PickUpWidget;
};