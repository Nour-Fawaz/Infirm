
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFIRM_API UDisplayWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RetText;

public:

	void SetText(int textVal);
	
	
};
