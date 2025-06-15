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
	// Sets default values for this actor's properties
	ASafeDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DisplaySafePanel(ASafeDoor* CurrentSafeDoor);
	void SetIsOpen(bool NewOpenStatus);
	bool GetIsOpen();
	FString GetSafeCode();
	void DestroyAllWidgets();

	UFUNCTION()
	void OpenSafeDoor();


private:

	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;

	//Player
	UPROPERTY()
	AFirstPersonPlayer* FPP;
	UPROPERTY()
	class AFirstPersonController* FPC;

	//widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USafeDoorWidget> SafeDoorWidgetClass;
	UPROPERTY()
	class USafeDoorWidget* SafePanelWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Door", meta = (AllowPrivateAccess = "true"))
	bool isOpen;

	//attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe Door Code", meta = (AllowPrivateAccess = "true"))
	FString Code;

	//movement
	FRotator OriginalRotation;
	FRotator TargetRotation;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationOffset;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	double RotationSpeed = 0;
	bool Opening = false;
	

	//events
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
