// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSource.h"
#include "Components/PointLightComponent.h"

// Sets default values
ALightSource::ALightSource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Light Mesh
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Mesh"));
	RootComponent = LightMesh;
	LightEmitter = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Emitter"));
	LightEmitter->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALightSource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

