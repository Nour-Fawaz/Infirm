// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "FirstPersonPlayer.h"
#include "Door.generated.h"

UCLASS()
class INFIRM_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//----------------------------------PUBLIC ACTIONS--------------------------------------------
	UFUNCTION()
	void OpenDoor();

private:
	//----------------------------------COMPONENTS--------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	//----------------------------------PLAYER--------------------------------------------
	UPROPERTY()
	AFirstPersonPlayer* FPP;
	UPROPERTY()
	class AFirstPersonController* FPC;


	//----------------------------------ATTRIBUTES--------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Attributes")
	bool bLocked = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	bool bClosed = true;
	UPROPERTY(EditAnywhere, Category = "Passkey")
	FName Passkey;

	//----------------------------------MOVEMENT PROPERTIES--------------------------------------------
	FRotator OriginalRotation;
	FRotator TargetRotation;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationOffset;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	double RotationSpeed = 0;
	bool Opening = false;

	//----------------------------------WIDGETS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDisplayWidget> DisplayWidgetClass;
	UPROPERTY()
	class UDisplayWidget* LockedDoorWidget;
	void DestroyAllWidgets();

	//----------------------------------SOUND EFFECTS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* OpenDoorSoundEffect;
	
	
	//----------------------------------EVENTS--------------------------------------------
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
