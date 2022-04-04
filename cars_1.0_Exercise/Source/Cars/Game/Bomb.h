#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bomb.generated.h"

class UNetComponent;
class UBoxComponent;

UCLASS()
class CARS_API ABomb : public APawn
{
    GENERATED_BODY()

  public:
    ABomb();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

  protected:
    virtual void BeginPlay() override;

  protected:
    // Mesh
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent *m_pMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent *m_pBox;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult &SweepResult);
};
