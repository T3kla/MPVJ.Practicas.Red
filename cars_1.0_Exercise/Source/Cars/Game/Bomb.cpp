#include "Bomb.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameNet/NetComponent.h"

ABomb::ABomb()
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
        static ConstructorHelpers::FObjectFinder<UMaterial> CarMaterial(
            TEXT("Material'/Game/Textures/Bomb'"));
        m_pMesh->SetMaterial(0, CarMaterial.Object);
    }

    SetActorScale3D(FVector(0.2f, 0.1f, 0.05f));
    SetActorRotation(FRotator(0.f, 270.f, 0.f));
}

void ABomb::BeginPlay()
{
    Super::BeginPlay();
}

void ABomb::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABomb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
