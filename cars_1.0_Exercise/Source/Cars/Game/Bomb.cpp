#include "Bomb.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameNet/NetComponent.h"
#include "Car.h"

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

    SetActorScale3D(FVector(0.2f, 0.2, 0.2));
    SetActorRotation(FRotator(0.f, 0.f, 0.f));

    m_pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCppCmp"));
    m_pBox->SetupAttachment(RootComponent);
}

void ABomb::BeginPlay()
{
    Super::BeginPlay();

    m_pBox->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnOverlapBegin);
}

void ABomb::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABomb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABomb::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                           UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult &SweepResult)
{
    ACar *Car = Cast<ACar>(OtherActor);

    if (Car)
        Car->CollidedWithBomb(this);
}