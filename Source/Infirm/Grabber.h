// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grabber.generated.h"


UCLASS()
class INFIRM_API AGrabber : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//----------------------------------PUBLIC ACTIONS-------------------------------------------
	void GrabItem();
	
private:
	//----------------------------------COMPONENTS-------------------------------------------
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Box Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Meshes", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	//----------------------------------PLAYER-------------------------------------------
	UPROPERTY()
	class AFirstPersonPlayer* FPP;
	UPROPERTY()
	class AFirstPersonController* FPC;

	//----------------------------------ITEMS-------------------------------------------
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Items To Grab", meta = (AllowPrivateAccess = "true"))
	TArray<FName> ItemsToGrab;
	UPROPERTY()
	TArray<UStaticMeshComponent*> SpawnedMeshes;
	
	//----------------------------------SOCKETS-------------------------------------------
	UPROPERTY(EditAnywhere)
	TArray<FName> ItemSpawnSockets;
	int32 CurrentSocketIndex = 0;
	int32 SocketsOccupied = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items To Grab", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FName> ItemSocketMap; // Maps socket names to their corresponding item names
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items To Grab", meta = (AllowPrivateAccess = "true"))
	TArray<FName> ItemsInSockets;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items To Grab", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* MeshToSpawn;

	//----------------------------------EVENTS-------------------------------------------
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};