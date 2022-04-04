#include "GameCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Car.h"

AGameCamera::AGameCamera()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGameCamera::BeginPlay()
{
    Super::BeginPlay();
}

void AGameCamera::SetTarget()
{
    APlayerController *OurPC = UGameplayStatics::GetPlayerController(this, 0);

    if (OurPC)
    {
        OurPC->SetViewTarget(this);
        m_pTarget = OurPC->GetPawn<ACar>();
    }
}

void AGameCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (m_pTarget)
    {
        FVector vNewPos = m_pTarget->GetActorLocation();
        vNewPos.Z += m_fMinDistance + m_fDistanceFromVelocityFactor *
                                          m_pTarget->GetCarMovementComponent()->GetVelocityMagnitude();
        SetActorLocation(vNewPos);
    }
    else
    {
        SetTarget();
    }
}
