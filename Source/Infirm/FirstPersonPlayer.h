// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Pickable.h"
#include "PickableData.h"
#include "FirstPersonPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;


UCLASS()
class INFIRM_API AFirstPersonPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonPlayer();

	//----------------------------------INVENTORY--------------------------------------------
	void RemoveEquippedItem();
	void RemoveEquippedItemActor();
	TArray<FName> GetEquippedItemActorTags();

	bool ItemEquipped = false;
	APickable* GetEquippedItemActor();
	UFUNCTION(BlueprintCallable)
	FPickableData GetEquippedItemData() const { return EquippedItemData; }
	UFUNCTION()
	void ClearEquippedItemData();

	//----------------------------------WIDGETS--------------------------------------------
	void SetGamePaused(bool NewGamePaused);

protected:
	virtual void BeginPlay() override;

	//----------------------------------INPUT--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputMappingContext* FirstPersonMappingInputContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* PickUpAction;
	void PickUpItem(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* PauseAction;
	void PauseGame(const FInputActionValue& Value);
	bool bGamePaused = false;

public:	
	virtual void Tick(float DeltaTime) override;

	//----------------------------------INPUT--------------------------------------------
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//----------------------------------COMPONENTS--------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capsule Component", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* EquippedItem;
	UPROPERTY()
	APickable* EquippedItemActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;
	
	//----------------------------------EVENTS--------------------------------------------
	UFUNCTION()
	void DropEquippedItem(APickable* ToPickUp);
	APickable* ItemToSetDown;
	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<class APickable> PickableClass;
	TArray<FName> EquippedItemActorTags;
	UFUNCTION()
	void EquipItem(APickable* TempPickable, AFirstPersonController* FPC);
	UPROPERTY()
	FPickableData EquippedItemData;

	//----------------------------------SOUND EFFECTS--------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PickUpSoundEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PutDownSoundEffect;
};