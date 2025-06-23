// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSource.h"
#include "Components/PointLightComponent.h"

// Sets default values
ALightSource::ALightSource()
{
	PrimaryActorTick.bCanEverTick = true;

	//initialize components
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Mesh"));
	RootComponent = LightMesh;
	LightEmitter = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Emitter"));
	LightEmitter->SetupAttachment(RootComponent);

}

void ALightSource::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALightSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

