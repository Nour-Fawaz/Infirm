// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h" 
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "FirstPersonController.h"
#include "Door.h"
#include "Searchable.h"
#include "UObject/Class.h"
#include "SafeDoor.h"
#include "Drawer.h"
#include "Grabber.h"

// Sets default values
AFirstPersonPlayer::AFirstPersonPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//components
	CapsuleComp = GetCapsuleComponent();
	RootComponent = CapsuleComp;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(CapsuleComp); 
	CameraComp->bUsePawnControlRotation = true; //allow camera to rotate with pawn
	SkeletalMesh = GetMesh();
	EquippedItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Equipped Item"));
}

/* 
* function: "Removes" whatever is in player's hand 
*/
void AFirstPersonPlayer::RemoveEquippedItem()
{
	EquippedItem->SetStaticMesh(nullptr);
	ItemEquipped = false;
}

/*
* function: sets equipped item actor to nullptr
*/
void AFirstPersonPlayer::RemoveEquippedItemActor()
{
	EquippedItemActor = nullptr;
}

/*
* function: returns tags of currently equippeditem
*/
TArray<FName> AFirstPersonPlayer::GetEquippedItemActorTags()
{
	return EquippedItemActorTags;
}

APickable* AFirstPersonPlayer::GetEquippedItemActor()
{
	if (EquippedItemActor)
	{
		UE_LOG(LogTemp, Display, TEXT("EquippedItemActor Name (FPP): %s"), *EquippedItemActor->GetName());
		return EquippedItemActor;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EquippedItemActor is null FPP"));
		return nullptr;
	}
	
}

void AFirstPersonPlayer::ClearEquippedItemData()
{
	EquippedItemData = FPickableData(); // Resets the struct
}

void AFirstPersonPlayer::SetGamePaused(bool NewGamePaused)
{
	bGamePaused = NewGamePaused;
}

// Called when the game starts or when spawned
void AFirstPersonPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* FirstPersonPlayerController = Cast<APlayerController>(GetController());
	if (FirstPersonPlayerController)
	{
		UE_LOG(LogTemp, Display, TEXT("Have controller"));
		UEnhancedInputLocalPlayerSubsystem* FirstPersonPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(FirstPersonPlayerController->GetLocalPlayer());
		if (FirstPersonPlayerSubsystem)
		{
			FirstPersonPlayerSubsystem->AddMappingContext(FirstPersonMappingInputContext, 0);
		}
	}
	if (GetMesh())
	{
		UE_LOG(LogTemp, Display, TEXT("Have mesh"));

	}
	//setup socket for equippedItem
	FName BoneName(TEXT("Bone02Socket"));
	if (SkeletalMesh && SkeletalMesh->DoesSocketExist(BoneName))
	{
		// Bone exists, safe to attach
		EquippedItem->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BoneName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Bone '%s' does not exist on the skeletal mesh!"), *BoneName.ToString());
	}	
}

void AFirstPersonPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorRightVector(), MoveVector.X);
		AddMovementInput(GetActorForwardVector(), MoveVector.Y);
	}
}

void AFirstPersonPlayer::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

/*
* function: handles player and actor interactions
*/
void AFirstPersonPlayer::PickUpItem(const FInputActionValue& Value)
{
	//get FPC 
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonController* FPC = Cast<AFirstPersonController>(APC); //get player controller

	bool PickingUp = Value.Get<bool>();
	if (Controller)
	{
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		for (AActor* CurrentActor : OverlappingActors)
		{
			UE_LOG(LogTemp, Display, TEXT("current overlapped actor is: %s"), *CurrentActor->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Overlapping actors count: %d"), OverlappingActors.Num());

			if (CurrentActor->ActorHasTag("PickUp"))
			{
				APickable* TempPickable = Cast<APickable>(CurrentActor);
				UE_LOG(LogTemp, Display, TEXT("this actor is a pickable"));

				//if you are not already holding a pickable
				if (!ItemEquipped && FPC)
				{
					EquipItem(TempPickable, FPC);
					TempPickable->DestroyAllWidgets();
					UE_LOG(LogTemp, Display, TEXT("EquippedItemActor Name: %s"), *EquippedItemActor->GetName())
					break;
				}
				else if (ItemEquipped && FPC)
				{
					//drop equipped item
					FPC->DestroyAllWidgets();
					FPC->DestroyDisplayWidget();
					TempPickable->DestroyAllWidgets();
					DropEquippedItem(TempPickable);
					EquipItem(TempPickable, FPC);
					break;
				}
			}
			else if (CurrentActor->ActorHasTag("Searchable"))
			{
				UE_LOG(LogTemp, Display, TEXT("this is a searchable"));
				ASearchable* TempSearchable = Cast<ASearchable>(CurrentActor);
				if (TempSearchable && FPC)
				{
					TempSearchable->DestroyAllWidgets();
					TempSearchable->SearchItem();
				}	
			}
			else if (CurrentActor->ActorHasTag("Door"))
			{
				ADoor* TempDoor = Cast<ADoor>(CurrentActor);
				TempDoor->OpenDoor();
			}
			else if (CurrentActor->ActorHasTag("SafeDoor"))
			{
				ASafeDoor* TempSafe = Cast<ASafeDoor>(CurrentActor);
				TempSafe->DisplaySafePanel(TempSafe);
			}
			else if (CurrentActor->ActorHasTag("Drawer"))
			{
				ADrawer* TempDrawer = Cast<ADrawer>(CurrentActor);
				TempDrawer->OpenDrawer(TempDrawer);
			}
			else if (CurrentActor->ActorHasTag("Grabber"))
			{
				AGrabber* TempGrabber = Cast<AGrabber>(CurrentActor);
				TempGrabber->GrabItem();
			}
			else if (CurrentActor->ActorHasTag("Note"))
			{
				if (APickable* TempPickable = Cast<APickable>(CurrentActor))
				{
					FPC->DestroyDisplayWidget();
					UE_LOG(LogTemp, Warning, TEXT("This note is: %s"), *CurrentActor->GetName());
					FPC->ReadNoteWidget(TempPickable);
				}
			}	
		}
	}
}

void AFirstPersonPlayer::PauseGame(const FInputActionValue& Value)
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonController* FPC = Cast<AFirstPersonController>(APC); //get player controller

	bool Pausing = Value.Get<bool>();
	if (Controller)
	{
		if (!UGameplayStatics::IsGamePaused(GetWorld()))
		{
			FPC->PauseGame();
			UE_LOG(LogTemp, Warning, TEXT("PAUSING GAME, bGamePaused: %b"), bGamePaused);
		}
	}
}

// Called every frame
void AFirstPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonPlayer::Look);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &AFirstPersonPlayer::PickUpItem); // find how to fix for bool
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AFirstPersonPlayer::PauseGame);
	}
}

/*
* function: drops currently equipped item and spawns it in the world
*/
void AFirstPersonPlayer::DropEquippedItem(APickable* ToPickUp)
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonController* FPC = Cast<AFirstPersonController>(APC); //get player controller
	if (ItemEquipped && EquippedItem && ToPickUp)
	{
		UE_LOG(LogTemp, Log, TEXT("Can Set Down Item"));

		// Get temp pickable transform
		FVector SpawnLocation = ToPickUp->GetActorLocation();
		FRotator SpawnRotation = ToPickUp->GetActorRotation();
		UE_LOG(LogTemp, Log, TEXT("TempPickable Location: %s. TempPickable Rotation: %s"), *SpawnLocation.ToString(), *SpawnRotation.ToString());

		ToPickUp->Destroy();

		//drop what player is currently holding
		TSubclassOf<APickable> ClassToSpawn = EquippedItemData.PickableClass;
		if (ClassToSpawn)
		{
			UE_LOG(LogTemp, Log, TEXT("Class to spawn passed"));
			//spawn item at specified location and rotation
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			APickable* DroppedItem = GetWorld()->SpawnActor<APickable>(ClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			if (DroppedItem)
			{
				//adjust dropped item mesh and tags
				UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(DroppedItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				if (MeshComp)
				{
					MeshComp->SetStaticMesh(EquippedItemData.Mesh);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("DroppedItem has no mesh component"));	
				}
				DroppedItem->Tags = EquippedItemData.Tags; //set tags of dropped item to currently equipped item tags

				//remove old item from inventry
				FPC->CheckInventoryByTag(EquippedItemData.Tags, false); //remove item from inventory by tags
				UE_LOG(LogTemp, Log, TEXT("Dropped item: %s"), *DroppedItem->GetName());
			}
		}
		if (PutDownSoundEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PutDownSoundEffect, GetActorLocation());
		}
		// Clear current held item
		EquippedItem->SetStaticMesh(nullptr);
		ItemEquipped = false;
		EquippedItemActor = nullptr;
	}
}

/*
* function: equips item that is passed in function
*/
void AFirstPersonPlayer::EquipItem(APickable* CurrentPickable, AFirstPersonController* AFPC)
{
	AFPC->DestroyDisplayWidget();
	if (!CurrentPickable)
	{
		UE_LOG(LogTemp, Error, TEXT("Tried to equip a null pickable!"));
		return;
	}
	if (PickUpSoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSoundEffect, GetActorLocation());
	}
	if (CurrentPickable)
	{
		//set equipped item actor
		UE_LOG(LogTemp, Warning, TEXT("Before assignment: EquippedItemActor=%s"), EquippedItemActor ? *EquippedItemActor->GetName() : TEXT("nullptr"));
		// Save data before picking up the pickable
		UStaticMeshComponent* CurrentPickableMeshComp = Cast<UStaticMeshComponent>(CurrentPickable->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		UStaticMesh* CurrentPickableMesh = CurrentPickableMeshComp ? CurrentPickableMeshComp->GetStaticMesh() : nullptr;
		EquippedItemData.PickableClass = CurrentPickable->GetClass();
		EquippedItemData.Mesh = CurrentPickableMesh;
		EquippedItemData.Tags = CurrentPickable->Tags; 
		EquippedItemData.DisplayName = CurrentPickable->GetName(); 
		EquippedItemActor = CurrentPickable;
		UE_LOG(LogTemp, Warning, TEXT("After assignment: EquippedItemActor=%s"), EquippedItemActor ? *EquippedItemActor->GetName() : TEXT("nullptr"));
		UE_LOG(LogTemp, Warning, TEXT("EquippedItemData: PickableClass=%s, Mesh=%s, Tags=%s, DisplayName=%s"),
			*EquippedItemData.PickableClass->GetName(),
			*EquippedItemData.Mesh->GetName(),
			*FString::JoinBy(EquippedItemData.Tags, TEXT(", "), [](const FName& Tag) { return Tag.ToString(); }),
			*EquippedItemData.DisplayName);
		
		CurrentPickable->PickUp();
		ItemEquipped = true;

		//set equipped item mesh
		if (!CurrentPickableMeshComp)
		{
			UE_LOG(LogTemp, Error, TEXT("Pickable has no mesh component!"));
			return;
		}
		if (!CurrentPickableMesh)
		{
			UE_LOG(LogTemp, Error, TEXT("Pickable mesh component has no mesh!"));
			return;
		}
		EquippedItem->SetStaticMesh(CurrentPickableMesh);
		UE_LOG(LogTemp, Display, TEXT("EquippedItem mesh: %s"), *EquippedItem->GetStaticMesh()->GetName());
		UE_LOG(LogTemp, Display, TEXT("EquippedItemActor mesh: %s"), *EquippedItemActor->GetName());

		//update inventory
		AFPC->CheckInventory(CurrentPickable, true);
	}
}





