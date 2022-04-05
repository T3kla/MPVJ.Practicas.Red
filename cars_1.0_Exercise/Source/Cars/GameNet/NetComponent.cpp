#include "GameNet/NetComponent.h"
#include "Net/Manager.h"
#include "DrawDebugHelpers.h"
#include "GameNet/GameBuffer.h"
#include "GameNet/BombBuffer.h"
#include "Game/CarMovementComponent.h"
#include "Game/Car.h"

UNetComponent::UNetComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    m_pManager = Net::CManager::getSingletonPtr();
}

void UNetComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                  FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (m_pManager->getID() == Net::ID::SERVER)
    {
        DrawDebugString(GetWorld(), FVector::ZeroVector, *FString("Real"), GetOwner(), FColor::Blue, 0.02f);
        SerializeData();
    }
    else if (m_pManager->getID() == m_uID)
    {
        DrawDebugString(GetWorld(), FVector::ZeroVector, *FString("Owner"), GetOwner(), FColor::Green, 0.02f);
        SerializeData();
    }
    else
    {
        DrawDebugString(GetWorld(), FVector::ZeroVector, *FString("Other"), GetOwner(), FColor::Red, 0.02f);
    }
}

void UNetComponent::SetID(unsigned int _uID)
{
    m_uID = _uID;
}

void UNetComponent::SetInput(const FVector2D &_vInput)
{
    m_vMovementInput = _vInput;
}

// Tell Server to place bomb
void UNetComponent::WantToPlaceBomb(FVector vBomb)
{
    CBombBuffer oData;

    oData.write(Net::NetMessageType::BOMB_PLACE);
    oData.write(m_uID);
    oData.write(vBomb);

    m_pManager->send(&oData, true);
}

// Tell Server to destroy bomb
void UNetComponent::WantToDestroyBomb(FVector vBomb)
{
    CBombBuffer oData;

    oData.write(Net::NetMessageType::BOMB_EXPLODE);
    oData.write(m_uID);
    oData.write(vBomb);

    m_pManager->send(&oData, true);
}

void UNetComponent::SerializeData()
{
    CGameBuffer oData;
    Net::NetMessageType eMType = Net::NetMessageType::ENTITY_MSG;
    oData.write(eMType);
    oData.write(m_uID);

    if (m_pManager->getID() == Net::ID::SERVER)
    {
        FTransform vTrans = GetOwner()->GetActorTransform();
        oData.write(vTrans);
        m_pManager->send(&oData, false);
    }
    else
    {
        oData.write(m_vMovementInput);
        m_pManager->send(&oData, false);
    }
}

void UNetComponent::DeserializeData(CGameBuffer *pData)
{
    ACar *pCar = GetOwner<ACar>();
    if (m_pManager->getID() == Net::ID::SERVER)
    {
        FVector2D vInput;
        pData->read(vInput);
        pCar->GetCarMovementComponent()->SetInput(vInput);
    }
    else
    {
        FTransform vTrans;
        pData->read(vTrans);
        pCar->SetActorTransform(vTrans);
    }
}
