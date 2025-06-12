// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Components/BoxComponent.h"
#include "Misc/App.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonController.h"
#include "Pickable.h"

// Sets default values
AGrabber::AGrabber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//component initialization
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	BoxComp->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void AGrabber::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AGrabber::OverlapBegin); //bind FUNCTION that is called to THIS object when overlapping other stuff
	
	//get FPP and FPC
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FPC = Cast<AFirstPersonController>(APC);
	if (APC)
	{
		FPP = Cast<AFirstPersonPlayer>(APC->GetPawn());

	}

	//socketrs
	ItemSpawnSockets = { "ItemSocketSpawn1", "ItemSocketSpawn2", "ItemSocketSpawn3", "ItemSocketSpawn4" };

}

// Called every frame
void AGrabber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabber::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AFirstPersonPlayer* FirstPersonPlayer = Cast<AFirstPersonPlayer>(OtherActor);
		UE_LOG(LogTemp, Display, TEXT("Casted OtherActor in Grabber"))

			if (FirstPersonPlayer && FPC)
			{
				TArray<FName> FPPTags = FirstPersonPlayer->GetEquippedItemActorTags();

				for (FName FPPTag : FPPTags)
				{
					for (FName TagsWanted : ItemsToGrab)
					{
						UE_LOG(LogTemp, Display, TEXT("FPPTag: %s and TagWanted: %s"), *FPPTag.ToString(), *TagsWanted.ToString());

						if (FPPTag == TagsWanted && FPC->InInventory(FPPTag))
						{
							// Cache pickable actor BEFORE removing anything
							APickable* PickableToSpawn = FPP->GetEquippedItemActor();

							FirstPersonPlayer->RemoveEquippedItem();
							FPC->RemoveFromInventory(FPPTag);

							if (PickableToSpawn && CurrentSocketIndex < ItemSpawnSockets.Num())
							{
								UE_LOG(LogTemp, Display, TEXT("EquippedItemActor name: %s"), *PickableToSpawn->GetName());

								UStaticMeshComponent* SourceMeshComp = Cast<UStaticMeshComponent>(PickableToSpawn->GetComponentByClass(UStaticMeshComponent::StaticClass()));
								if (!SourceMeshComp) return;

								UStaticMesh* MeshToSpawn = SourceMeshComp->GetStaticMesh();
								if (!MeshToSpawn) return;

								FName SocketName = ItemSpawnSockets[CurrentSocketIndex++];

								if (!StaticMesh->DoesSocketExist(SocketName))
								{
									UE_LOG(LogTemp, Error, TEXT("Socket %s does not exist!"), *SocketName.ToString());
									return;
								}

								FVector SpawnLoc = StaticMesh->GetSocketLocation(SocketName);
								FRotator SpawnRot = StaticMesh->GetSocketRotation(SocketName);

								// Debug sphere
								//DrawDebugSphere(GetWorld(), SpawnLoc, 10.f, 12, FColor::Red, false, 5.0f);

								UStaticMeshComponent* SpawnedMesh = NewObject<UStaticMeshComponent>(this);
								if (SpawnedMesh)
								{
									SpawnedMesh->RegisterComponent();
									SpawnedMesh->SetStaticMesh(MeshToSpawn);
									SpawnedMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

									// Safety settings
									SpawnedMesh->SetWorldLocation(SpawnLoc);
									SpawnedMesh->SetWorldRotation(SpawnRot);
									SpawnedMesh->SetVisibility(true);
									SpawnedMesh->SetHiddenInGame(false);
									SpawnedMesh->SetRelativeScale3D(FVector(1.0f));
									SpawnedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
									SpawnedMesh->SetSimulatePhysics(false);

									// Store reference to prevent garbage collection
									SpawnedMeshes.Add(SpawnedMesh);

									SocketsOccupied++;
									FPP->RemoveEquippedItemActor();
								}

								
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("No more available sockets to spawn items!"));
							}
						}

						if (SocketsOccupied == 4)
						{
							UE_LOG(LogTemp, Warning, TEXT("GAME ENDED"));
							UGameplayStatics::OpenLevel(this, FName("EndGame"));
						}
					}
				}
			}
	}
}

