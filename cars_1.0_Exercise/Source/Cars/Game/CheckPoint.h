#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CheckPoint.generated.h"

class AFinishLine;

UCLASS()
class CARS_API ACheckPoint : public ATriggerBox
{
  GENERATED_BODY()

protected:
  virtual void BeginPlay() override;

public:
  ACheckPoint();

  inline void SetFinishLine(AFinishLine* _pFinishLine) { m_pFinishLine = _pFinishLine; }

public:
  UFUNCTION()
  void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

private:
  AFinishLine * m_pFinishLine;
  
};