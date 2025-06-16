// Fill out your copyright notice in the Description page of Project Settings.


#include "Searchable.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayWidget.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"

// Sets default values
ASearchable::ASearchable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SearchableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickable Mesh"));
	RootComponent = SearchableMesh;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ASearchable::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ASearchable::OverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ASearchable::OverlapEnd);

	//get FPC 
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (APC)
	{
		FPC = Cast<AFirstPersonController>(APC); //get player controller
	}
	
}

/* 
Function: 
	-Display to Player this is a searchable item if not searched
	-Display to player that this item has already been searched
*/
void ASearchable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUpWidget = nullptr;
	/*AFirstPersonPlayer* CheckActor = Cast<AFirstPersonPlayer>(OtherActor);
	check(CheckActor);*/
	if (FPC && DisplayWidgetClass) //check actor and display widget set
	{
		
		check(FPC); // check if controller valid
		PickUpWidget = CreateWidget<UDisplayWidget>(FPC, DisplayWidgetClass); //create widget

		//check what type of pickable
	    if (ActorHasTag("Searched"))
		{
			PickUpWidget->SetText(6); //make pickup widget display "empty"
		}
		else if (ActorHasTag("Search"))
		{
			PickUpWidget->SetText(1); //make pickup widget display "search"
		}
		check(PickUpWidget); //check widget is valiud
		PickUpWidget->AddToPlayerScreen(); //add to player screen

	}
}

/* Function: removes widget from player screen when no longer in proximity of searchable */
void ASearchable::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent(); //remove widget from player viewport
		PickUpWidget = nullptr;
	}
}

// Called every frame
void ASearchable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* 
Function:
	-Marks the item as searched
	-sets equipped item as hidden item (meshes)
*/
void ASearchable::SearchItem()
{
	//indicate that this actor has been searched
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent(); //remove widget from player viewport
		PickUpWidget = nullptr;
	}

	//RETURN THE ITEMFOUND like if you found incantation return "Incantion"
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
	//set the equipped item mesh equal to the hidden mesh
	if (HasHiddenItem)
	{
		//FString MeshName = HiddenMesh->GetName();
		UKismetSystemLibrary::PrintString(this, HiddenItemTag.ToString() , true, true, FColor::Blue, 5.0f);
		FPC->CheckInventoryName(HiddenItemTag);

	}
}

/* Function: Destroys all widgets displayed on player screen contributing from this class*/
void ASearchable::DestroyAllWidgets()
{
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent();
		PickUpWidget = nullptr;
	}
}

void ASearchable::CheckSearchableWidget(ASearchable* CurrentsSearchable)
{
}

