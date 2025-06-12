// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "FirstPersonController.h"
#include "Door.h"
#include "Searchable.h"
#include "Chaos/DebugDrawQueue.h"
#include "UObject/Class.h"
#include "SafeDoor.h"
#include "TriggerComponent.h"
#include "Drawer.h"

// Sets default values
AFirstPersonPlayer::AFirstPersonPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
/* Function: "Removes" whatever is in player's hand */
void AFirstPersonPlayer::RemoveEquippedItem()
{
	EquippedItem->SetStaticMesh(nullptr);
	ItemEquipped = false;
}

void AFirstPersonPlayer::RemoveEquippedItemActor()
{
	EquippedItemActor = nullptr;
}

TArray<FName> AFirstPersonPlayer::GetEquippedItemActorTags()
{
	return EquippedItemActorTags;
}

APickable* AFirstPersonPlayer::GetEquippedItemActor()
{
	if (EquippedItemActor)
	{
		return EquippedItemActor;
	}
	else
	{
		return nullptr;
	}
	
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
	/*if (SkeletalMesh && SkeletalMesh->SkeletalMesh)
	{
		const int32 NumBones = SkeletalMesh->GetNumBones();
		for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
		{
			FName BoneName = SkeletalMesh->GetBoneName(BoneIndex);
			UE_LOG(LogTemp, Log, TEXT("Bone %d: %s"), BoneIndex, *BoneName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh or its SkeletalMesh asset is null."));
	}*/
	FName BoneName(TEXT("Bone02Socket"));
	if (SkeletalMesh && SkeletalMesh->DoesSocketExist(BoneName))
	{
		// Bone exists, safe to attach
		EquippedItem->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BoneName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Bone '%s' does not exist on the skeletal mesh!"), *BoneName.ToString());
		// Handle error (e.g., fallback attachment, skip, etc.)
	}
	
	//overlapping
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFirstPersonPlayer::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff

	
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
				//UKismetSystemLibrary::PrintString(this, TEXT("This is a pickable"), true, true, FColor::Cyan, 5.0f);
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
				else if (ItemEquipped && FPC && EquippedItemActor)
				{
					//set pickable down
					UE_LOG(LogTemp, Log, TEXT("Set Pickable Down"));
					check(TempPickable);
					check(EquippedItemActor);
					check(EquippedItem);
					UE_LOG(LogTemp, Log, TEXT("TempPickabel: %s. Current Equipped Item Actor: %s, EquippedItem: %s"), *TempPickable->GetName(), *EquippedItemActor->GetName(), *EquippedItem->GetName());
					
					/*if (EquippedItemActor)
					{
						EquippedItemActor->DestroyAllWidgets();
					}
					else if (ASearchable* TempSearchable = Cast<ASearchable>(CurrentActor))
					{
						TempSearchable->DestroyAllWidgets();
					}*/
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
				UKismetSystemLibrary::PrintString(this, TEXT("This is a SEARCHABLE"), true, true, FColor::Cyan, 5.0f);
				ASearchable* TempSearchable = Cast<ASearchable>(CurrentActor);
				FString ItemFound = FString(TEXT("None"));
				//if you are not already holding a pickable
				if (TempSearchable && FPC)
				{
					TempSearchable->DestroyAllWidgets();
					TempSearchable->SearchItem(EquippedItem, ItemEquipped);
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
			else if (CurrentActor->ActorHasTag("Note"))
			{
				//DESTROY ALL CURRENT WIDGETS
				if (APickable* TempPickable = Cast<APickable>(CurrentActor))
				{
					TempPickable->DestroyAllWidgets();
					FPC->DestroyDisplayWidget();
					UE_LOG(LogTemp, Warning, TEXT("THIS CURRENT ACTOR IS: %s"), *CurrentActor->GetName());
					FPC->ReadNoteWidget(TempPickable); /////////////////////////////////////
				}
				else if (ASearchable* TempSearchable = Cast<ASearchable>(CurrentActor))
				{
					TempSearchable->DestroyAllWidgets();
				}
				//UE_LOG(LogTemp, Warning, TEXT("THIS CURRENT ACTOR IS: %s"), *CurrentActor->GetName());
				//FPC->ReadNoteWidget("SpawnRoom"); /////////////////////////////////////
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
		/*else
		{
			bGamePaused = false;
			FPC->UnPauseGame();
			UE_LOG(LogTemp, Warning, TEXT("GAME UNPAUSING, bGamePaused: %b"), bGamePaused);
		}*/
	}
}




// Called every frame
void AFirstPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonController* FPC = Cast<AFirstPersonController>(APC); //get player controller
	FPC->CheckExistingQidgets();

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


void AFirstPersonPlayer::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AFirstPersonPlayer::DropEquippedItem(APickable* ToPickUp)
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonController* FPC = Cast<AFirstPersonController>(APC); //get player controller
	//FPC->DestroyDisplayWidget();
	if (ItemEquipped && EquippedItem && EquippedItemActor && ToPickUp)
	{

		UE_LOG(LogTemp, Log, TEXT("Can Set Down Item"));
		// Get temp pickable transform
		FVector SpawnLocation = ToPickUp->GetActorLocation();
		FRotator SpawnRotation = ToPickUp->GetActorRotation();
		UE_LOG(LogTemp, Log, TEXT("TempPickable Location: %s. TempPickable Rotation: %s"), *SpawnLocation.ToString(), *SpawnRotation.ToString());

		//Call pickup function (on TempPickable)
		ItemToSetDown = EquippedItemActor;
		
		ToPickUp->Destroy();


		// Drop what we're currently holding
		TSubclassOf<APickable> ClassToSpawn = EquippedItemActor->GetClass();
		UE_LOG(LogTemp, Log, TEXT("ClassToSpawn: %s"), *EquippedItemActor->GetName());
		if (ClassToSpawn)
		{
			UE_LOG(LogTemp, Log, TEXT("Class to spawn PASSED"));
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			APickable* DroppedItem = GetWorld()->SpawnActor<APickable>(ClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			if (DroppedItem)
			{
				UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(DroppedItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				DroppedItem->Tags.Add("PickUp");
				DroppedItem->TransferActorTags(EquippedItemActorTags, DroppedItem);
				//remove from inventory
				//get FPC 
				APlayerController* APlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				AFirstPersonController* FPersonController = Cast<AFirstPersonController>(APlayerController); //get player controller
				FPersonController->CheckInventory(EquippedItemActor, false);

				
				if (MeshComp && EquippedItem)
				{
					UStaticMesh* DroppedMesh = EquippedItem->GetStaticMesh();
					MeshComp->SetStaticMesh(DroppedMesh);
				}

			}
			DrawDebugSphere(GetWorld(), SpawnLocation, 10.0f, 12, FColor::Red, false, 5.0f);
			UE_LOG(LogTemp, Log, TEXT("Dropped item: %s"), *DroppedItem->GetName());
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

void AFirstPersonPlayer::EquipItem(APickable* CurrentPickable, AFirstPersonController* AFPC)
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonController* FPC = Cast<AFirstPersonController>(APC); //get player controller
	AFPC->DestroyDisplayWidget();

	if (PickUpSoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSoundEffect, GetActorLocation());
	}

	//get mesh of aactor and set it to mesh of equipped item
	EquippedItemActor = CurrentPickable;
	EquippedItemActorTags = CurrentPickable->GetPickableTags(CurrentPickable);
	CurrentPickable->PickUp();
	FString Message = FString::Printf(TEXT("Pickable is: %s"), *CurrentPickable->GetName());
	UKismetSystemLibrary::PrintString(this, Message, true, true, FColor::Red, 5.0f);
	ItemEquipped = true;

	UStaticMeshComponent* PickableMeshComp = Cast<UStaticMeshComponent>(CurrentPickable->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	UStaticMesh* PickableMesh = PickableMeshComp->GetStaticMesh();
	EquippedItem->SetStaticMesh(PickableMesh);
	UE_LOG(LogTemp, Display, TEXT("EquippedItem: %s"), *EquippedItem->GetStaticMesh()->GetName());
	AFPC->CheckInventory(CurrentPickable, true);
}





