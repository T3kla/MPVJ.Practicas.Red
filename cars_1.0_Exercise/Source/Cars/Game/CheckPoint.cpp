
#include "CheckPoint.h"
#include "DrawDebugHelpers.h"
#include "FinishLine.h"

ACheckPoint::ACheckPoint()
{
    OnActorBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
}

void ACheckPoint::BeginPlay()
{
    Super::BeginPlay();

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true,
                 FLT_MAX, 0, 5);
}

void ACheckPoint::OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor)
{
    if (OtherActor && (OtherActor != this) && m_pFinishLine)
        m_pFinishLine->AddPassedCheckPoint(this);
}
