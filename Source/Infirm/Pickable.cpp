// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"
#include "Components/SceneComponent.h"

APickable::APickable()
{
	PrimaryActorTick.bCanEverTick = true;

	//initilize components
	PickableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickable Mesh"));
	RootComponent = PickableMesh;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(RootComponent);
}

/*
* function: destroys pickable
*/
void APickable::PickUp()
{
	DestroyAllWidgets();
	this->Destroy();
	UE_LOG(LogTemp, Display, TEXT("Picked up"));
}

void APickable::BeginPlay()
{
	Super::BeginPlay();

	//bind events
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APickable::OverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &APickable::OverlapEnd);
}

void APickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PickUpWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("PICK UP WIDGET ALIVE"));
	}
}

/*
* function: destroys all widgets relevant to Pickable
*/
void APickable::DestroyAllWidgets()
{
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent();
		PickUpWidget = nullptr; 
		UE_LOG(LogTemp, Warning, TEXT("Destroying widget on: %s"), *GetName());
	}
}

TArray<FName> APickable::GetPickableTags(APickable* AHasTags)
{
	const TArray<FName>& ActorTags = AHasTags->Tags;
	UE_LOG(LogTemp, Display, TEXT("Getting Actor Tags"));

	//get list of pickable tags
	FString Output = TEXT("Tags: ");
	for (const FName& Tag : Tags)
	{
		Output += Tag.ToString() + TEXT(", ");
	}
	if (Tags.Num() > 0)
	{
		Output.LeftChopInline(2); // Remove extra commas and spaces
	}

	UE_LOG(LogTemp, Warning, TEXT("PickedUp actor Tags: %s"), *Output);

	return ActorTags;
}

void APickable::TransferActorTags(TArray<FName> TagsToTransfer, APickable* ATransferTo)
{
	if (ATransferTo)
	{
		// Add each tag to the second actor if it doesn't already have it
		for (const FName& Tag : TagsToTransfer)
		{
			if (!ATransferTo->Tags.Contains(Tag))
			{
				ATransferTo->Tags.Add(Tag);
			}
		}
	}
}


void APickable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUpWidget = nullptr;

	AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
	if (!CheckActor) return;

	AFirstPersonController* FPC = Cast<AFirstPersonController>(CheckActor->GetController());
	if (FPC)
	{
		FPC->DisplayPickUpWidget(this);
	}
}

void APickable::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
	if (!CheckActor) return;

	AFirstPersonController* FPC = Cast<AFirstPersonController>(CheckActor->GetController());
	if (FPC)
	{
		FPC->DestroyDisplayWidget();
	}
}