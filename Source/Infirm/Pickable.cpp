// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"
#include "Components/SceneComponent.h"

// Sets default values
APickable::APickable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickable Mesh"));
	RootComponent = PickableMesh;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(RootComponent);
}

void APickable::PickUp()
{
	DestroyAllWidgets();
	this->Destroy();
	UKismetSystemLibrary::PrintString(this, TEXT("Picked up"), true, true, FColor::Cyan, 5.0f);
}

//FName APickable::GetLocationTag(APickable* CurrentPickable)
//{
//	if (CurrentPickable)
//	{
//		const TArray<FName> PickableTags = CurrentPickable->Tags;
//		for (const FName& Tag : PickableTags)
//		{
//			if (Tag != "Note")
//			{
//				UE_LOG(LogTemp, Display, TEXT("Location Tag: %s"), *Tag.ToString());
//				return Tag;
//				
//			}
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Display, TEXT("Actor Not Valid"));
//	}
//	return NAME_None;
//}

//void APickable::ReadNote()
//{
//	//get overlapping actors
//	TArray<AActor*> OverlappingActors;
//	GetOverlappingActors(OverlappingActors);
//	AFirstPersonPlayer* TempActor = nullptr;
//	for (AActor* CurrentActor : OverlappingActors)
//	{
//		TempActor = Cast<AFirstPersonPlayer>(CurrentActor);
//	}
//	
//	if (TempActor)
//	{
//		AFirstPersonController* FPC = TempActor->GetController<AFirstPersonController>();
//		FPC->ReadNoteWidget();
//		//NoteWidget = CreateWidget<UNoteWidget>(FPC, NoteWidgetClass); //create widget
//		//check(NoteWidget);  
//		//UKismetSystemLibrary::PrintString(this, TEXT("IN READ NOTE"), true, true, FColor::Cyan, 5.0f);
//		//NoteWidget->AddToViewport();
//	}
//
//
//}

// Called when the game starts or when spawned
void APickable::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APickable::OverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &APickable::OverlapEnd);
	
}

// Called every frame
void APickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PickUpWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("PICK UP WIDGET ALIVE"));
	}

}

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
	//////////////////////////////
	FString Output = TEXT("Tags: ");
	for (const FName& Tag : Tags)
	{
		Output += Tag.ToString() + TEXT(", ");
	}

	// Remove the trailing comma and space
	if (Tags.Num() > 0)
	{
		Output.LeftChopInline(2); // Remove ", "
	}

	UE_LOG(LogTemp, Warning, TEXT("PickedUp actor Tags: %s"), *Output);
	//////////////////////////
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
	if (!FPC) return;

	FPC->DisplayPickUpWidget(this);

}

void APickable::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
	if (!CheckActor) return;

	AFirstPersonController* FPC = Cast<AFirstPersonController>(CheckActor->GetController());
	if (!FPC) return;

	FPC->DestroyDisplayWidget();
}
