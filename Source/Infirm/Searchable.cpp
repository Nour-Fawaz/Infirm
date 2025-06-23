// Fill out your copyright notice in the Description page of Project Settings.


#include "Searchable.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"


ASearchable::ASearchable()
{
	PrimaryActorTick.bCanEverTick = true;

	//initilize components
	SearchableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickable Mesh"));
	RootComponent = SearchableMesh;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(RootComponent);
}

void ASearchable::BeginPlay()
{
	Super::BeginPlay();

	//bind events
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ASearchable::OverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ASearchable::OverlapEnd);

	//get FPC 
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (APC)
	{
		FPC = Cast<AFirstPersonController>(APC); //get player controller
	}
}

void ASearchable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (FPC)
	{
		FPC->DisplayWidgetTextByInt(1);
	}
}

void ASearchable::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (FPC)
	{
		FPC->DestroyDisplayWidget();
	}
}

void ASearchable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* 
* function: marks item as search and displays hidden message, and/or gives player found item
*/
void ASearchable::SearchItem()
{
	//displays character message for item
	if (HasHiddenMessage)
	{
		FPC->DisplayerPlayerTextWidget(HiddenItemTag);
		if (SearchSoundEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SearchSoundEffect, GetActorLocation());
		}
	}
	else
	{
		this->Tags.Empty();
		this->Tags.Add("Searched");
	}
	
	//if searchable has hidden item add it to player inventory
	if (HasHiddenItem)
	{
		UE_LOG(LogTemp, Display, TEXT("Searchable has hidden item: %s"), *HiddenItemTag.ToString());
		FPC->CheckInventoryName(HiddenItemTag);
	}
}

/* 
* function: destroys all widgets relevant to searchable
*/
void ASearchable::DestroyAllWidgets()
{
	if (FPC)
	{
		FPC->DestroyDisplayWidget();
	}
}