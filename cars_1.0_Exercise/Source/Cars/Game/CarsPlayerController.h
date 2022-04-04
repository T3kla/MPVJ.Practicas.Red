#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarsPlayerController.generated.h"

UCLASS()
class CARS_API ACarsPlayerController : public APlayerController
{
    GENERATED_BODY()

    ACarsPlayerController(const class FObjectInitializer &ObjectInitializer);

  protected:
    virtual void BeginPlay() override;
};
