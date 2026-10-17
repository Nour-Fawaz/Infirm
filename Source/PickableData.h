

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PickableData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPickableData
{
    GENERATED_BODY()

    UPROPERTY()
    TSubclassOf<class APickable> PickableClass;

    UPROPERTY()
    UStaticMesh* Mesh;

    UPROPERTY()
    TArray<FName> Tags;

    UPROPERTY()
    FString DisplayName;
};
