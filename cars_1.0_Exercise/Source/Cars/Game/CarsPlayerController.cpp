#include "CarsPlayerController.h"

ACarsPlayerController::ACarsPlayerController(const class FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    bAutoManageActiveCameraTarget = false;
}

void ACarsPlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetInputMode(FInputModeGameAndUI());
}
