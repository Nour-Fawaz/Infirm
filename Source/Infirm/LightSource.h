// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSource.generated.h"

UCLASS()
class INFIRM_API ALightSource : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightSource();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	//----------------------------------COMPONENTS--------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshes", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light", meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* LightEmitter;

};
