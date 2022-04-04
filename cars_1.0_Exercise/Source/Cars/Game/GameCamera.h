#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameCamera.generated.h"

class ACar;

UCLASS()
class CARS_API AGameCamera : public ACameraActor
{
    GENERATED_BODY()

  public:
    AGameCamera();

    virtual void Tick(float DeltaTime) override;

  protected:
    virtual void BeginPlay() override;

    void SetTarget();

  private:
    UPROPERTY(EditAnywhere)
    float m_fMinDistance = 200.f;

    UPROPERTY(EditAnywhere)
    float m_fDistanceFromVelocityFactor = 0.6f;

    ACar *m_pTarget;
};
