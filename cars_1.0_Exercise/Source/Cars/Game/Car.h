#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CarMovementComponent.h"
#include "Car.generated.h"

class UNetComponent;

UCLASS()
class CARS_API ACar : public APawn
{
    GENERATED_BODY()

  public:
    ACar();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

    UCarMovementComponent *GetCarMovementComponent()
    {
        return m_pCarMovement;
    }
    UNetComponent *GetNetComponent()
    {
        return m_pNet;
    }

  protected:
    virtual void BeginPlay() override;

    void Move(float AxisValue);
    void Turn(float AxisValue);
    void PlaceBomb();

  protected:
    // Mesh
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent *m_pMesh;

    UPROPERTY(EditAnywhere)
    UCarMovementComponent *m_pCarMovement;

    UPROPERTY(EditAnywhere)
    UNetComponent *m_pNet;

    // Input variables
    FVector2D m_vMovementInput = FVector2D::ZeroVector;
};
