#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetComponent.generated.h"

namespace Net
{
class CManager;
}
class CGameBuffer;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CARS_API UNetComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    UNetComponent();

  protected:
    virtual void BeginPlay() override;

  public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    void SetID(unsigned int _uID);
    void SetInput(const FVector2D &_vInput);

    void SerializeData();
    void DeserializeData(CGameBuffer *pData);

    void WantToPlaceBomb(FVector vBomb);
    void WantToDestroyBomb(AActor *pBomb);

    void PlaceBomb(CGameBuffer *pData);
    void DestroyBomb(CGameBuffer *pData);

  private:
    Net::CManager *m_pManager;

    TArray<AActor *> Bombs;

    unsigned int m_uID;
    FVector2D m_vMovementInput;
};
