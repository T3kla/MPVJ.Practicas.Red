#include "Game/CarMovementComponent.h"

UCarMovementComponent::UCarMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCarMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

FVector UCarMovementComponent::CalculateAcceleration() const
{
    FVector vAccel;

    if (m_vMovementInput.Y > 0.f)
        vAccel = m_fAccel * m_vMovementInput.Y * GetOwner()->GetActorForwardVector();
    else if (m_vMovementInput.Y < 0.f)
        vAccel = m_fBrakeAccel * m_vMovementInput.Y * GetOwner()->GetActorForwardVector();
    else
        vAccel = -m_fDrag * GetOwner()->GetActorForwardVector();

    // Side
    vAccel += m_vMovementInput.X * m_vVelocity.Size() * m_fRotationFactor * GetOwner()->GetActorRightVector();

    return vAccel;
}

FVector UCarMovementComponent::CalculateAverageVelocity(const FVector &_vAccel, float _fDeltaTime)
{
    FVector vAverageVelocity = m_vVelocity + 0.5f * _vAccel * _fDeltaTime;
    ClampVelocity(vAverageVelocity);

    m_vVelocity += _vAccel * _fDeltaTime;
    ClampVelocity(m_vVelocity);

    return vAverageVelocity;
}

void UCarMovementComponent::ClampVelocity(FVector &_vVelocity_) const
{
    if (_vVelocity_ != FVector::ZeroVector &&
        FVector::DotProduct(_vVelocity_, GetOwner()->GetActorForwardVector()) < 0.f)
    {
        _vVelocity_ = FVector::ZeroVector;
    }
    else if (_vVelocity_.SizeSquared() > m_fMaxVelocity * m_fMaxVelocity)
    {
        _vVelocity_ *= m_fMaxVelocity / _vVelocity_.Size();
    }
}

void UCarMovementComponent::MoveActor(const FVector &_vAverageVelocity, float _fDeltaTime)
{
    // X = Xo + Vm * t
    FTransform mTrans = GetOwner()->GetActorTransform();
    mTrans.AddToTranslation(_vAverageVelocity * _fDeltaTime);
    GetOwner()->SetActorTransform(mTrans);
    if (_vAverageVelocity != FVector::ZeroVector)
    {
        FRotator oRot = FRotationMatrix::MakeFromX(_vAverageVelocity).Rotator();
        GetOwner()->SetActorRotation(oRot);
    }
}

void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector vAccel = CalculateAcceleration();
    FVector vAverageVelocity = CalculateAverageVelocity(vAccel, DeltaTime);

    MoveActor(vAverageVelocity, DeltaTime);
}

float UCarMovementComponent::GetVelocityMagnitude() const
{
    return m_vVelocity.Size();
}
