// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstPersonPlayer.h"
#include "SafeDoor.generated.h"

UCLASS()
class INFIRM_API ASafeDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASafeDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//----------------------------------WIDGETS--------------------------------------------
	void DisplaySafePanel(ASafeDoor* CurrentSafeDoor);
	void DestroyAllWidgets();

	//----------------------------------GET ATTRIBUTES--------------------------------------------
	void SetIsOpen(bool NewOpenStatus);
	bool GetIsOpen();
	FString GetSafeCode();
	
	//----------------------------------PUBLIC ACTIONS--------------------------------------------
	UFUNCTION()
	void OpenSafeDoor();


private:
	//----------------------------------COMPONENTS--------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;

	//----------------------------------PLAYER--------------------------------------------
	UPROPERTY()
	AFirstPersonPlayer* FPP;
	UPROPERTY()
	class AFirstPersonController* FPC;

	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USafeDoorWidget> SafeDoorWidgetClass;
	UPROPERTY()
	class USafeDoorWidget* SafePanelWidget;

	//----------------------------------ATTRIBUTES--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Door Code", meta = (AllowPrivateAccess = "true"))
	FString Code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Door", meta = (AllowPrivateAccess = "true"))
	bool isOpen;

	//----------------------------------MOVEMENTS--------------------------------------------
	FRotator OriginalRotation;
	FRotator TargetRotation;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationOffset;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	double RotationSpeed = 0;
	bool Opening = false;
	
	//----------------------------------EVENTS--------------------------------------------
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};