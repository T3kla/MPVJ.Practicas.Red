#pragma once

#include "Net/Manager.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include <map>

class UCarsGameInstance;
class ACar;

class CGameNetMgr : public Net::CManager::IObserver
{
  public:
    CGameNetMgr();
    CGameNetMgr(UCarsGameInstance *_pOwner);
    virtual ~CGameNetMgr();

    virtual void dataPacketReceived(Net::CPacket *packet) override;
    virtual void connectionPacketReceived(Net::CPacket *packet) override;
    virtual void disconnectionPacketReceived(Net::CPacket *packet) override;

  private:
    bool AmIServer();

    void CreateCar(unsigned int _uClient, FVector _vPos);

    void PlaceBomb(unsigned int _uClient, FVector vBomb);
    void DestroyBomb(unsigned int _uClient, FVector vBomb);

    TArray<AActor *> Bombs;

  private:
    Net::CManager *m_pManager = nullptr;
    UCarsGameInstance *m_pCarsGameInstance;
    unsigned int m_uMapLoadedNotifications = 0u;
    std::map<unsigned int, ACar *> m_tPlayers;
};
