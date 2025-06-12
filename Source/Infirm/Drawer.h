// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstPersonPlayer.h"
#include "Drawer.generated.h"

UCLASS()
class INFIRM_API ADrawer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDrawer(ADrawer* CurrentDrawer);

private:

	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	//widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerTextWidget> PlayerTextWidgetClass;
	UPROPERTY()
	class UPlayerTextWidget* PlayerTextWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UDisplayWidget> DisplayWidgetClass;
	UPROPERTY()
	class UDisplayWidget* LockedDrawerWidget;
	void DestroyAllWidgets();

	//sound effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* OpenDrawerSoundEffect;

	//Player
	UPROPERTY()
	AFirstPersonPlayer* FPP;
	UPROPERTY()
	class AFirstPersonController* FPC;

	//attributes
	UPROPERTY(EditAnywhere, Category = "Attributes")
	bool bLocked = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	bool bClosed = true;
	UPROPERTY(EditAnywhere, Category = "Passkey")
	FName Passkey;
	
	// Movement proeprties
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MoveOffset;//how far mover should move when activated
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveTime = 2.5;//amount of time it takes to move to target
	bool ShouldMove = false; //boolean to activate mover
	FVector OriginalLocation;
	FVector TargetLocation;
	bool Opening = false;

	//overlap functions
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
