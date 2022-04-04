#include "Car.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameNet/NetComponent.h"

ACar::ACar()
{
    PrimaryActorTick.bCanEverTick = true;

    UBoxComponent *BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;

    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    m_pMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Engine/BasicShapes/Cube"));

    if (BoxVisualAsset.Succeeded())
    {
        m_pMesh->SetStaticMesh(BoxVisualAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> CarMaterial(TEXT("Material'/Game/Textures/Car'"));
        m_pMesh->SetMaterial(0, CarMaterial.Object);
    }

    SetActorScale3D(FVector(0.2f, 0.1f, 0.05f));
    SetActorRotation(FRotator(0.f, 270.f, 0.f));

    AutoPossessAI = EAutoPossessAI::Disabled;

    m_pCarMovement = CreateDefaultSubobject<UCarMovementComponent>(TEXT("CarMovement"));
    m_pNet = CreateDefaultSubobject<UNetComponent>(TEXT("Net"));
}

void ACar::BeginPlay()
{
    Super::BeginPlay();
    m_vMovementInput.Set(0.f, 0.f);
}

void ACar::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Move", this, &ACar::Move);
    PlayerInputComponent->BindAxis("Turn", this, &ACar::Turn);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACar::WantToPlaceBomb);
}

UCarMovementComponent *ACar::GetCarMovementComponent()
{
    return m_pCarMovement;
}

UNetComponent *ACar::GetNetComponent()
{
    return m_pNet;
}

void ACar::Move(float AxisValue)
{
    m_vMovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ACar::Turn(float AxisValue)
{
    m_vMovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ACar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    m_pNet->SetInput(m_vMovementInput);
}

// Tell server to place bomb
void ACar::WantToPlaceBomb()
{
    auto pos = GetActorLocation() - GetActorForwardVector() * 100.f;
    m_pNet->WantToPlaceBomb(pos);
}

// Tell server to remove bomb
void ACar::CollidedWithBomb(AActor *pBomb)
{
    m_pNet->WantToDestroyBomb(pBomb);
}