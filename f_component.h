 #pragma once

#include <game/client/gameclient.h>


#define LOCAL g_Config.m_ClDummy
#define LOCAL_ID m_pClient->m_LocalIDs[LOCAL]

#define DUMMY !g_Config.m_ClDummy
#define DUMMY_ID m_pClient->m_LocalIDs[DUMMY]

#define PHYS_SIZE 28.f


class FComponent
{
public:
	FComponent(CGameClient *pClient) :
		m_pClient(pClient){}
	virtual ~FComponent() = default;

protected:
	CGameClient *m_pClient;

	CGameClient *GameClient() const { return m_pClient; }
	CControls *Controls() const { return &m_pClient->m_Controls; }
	IConsole *Console() const { return m_pClient->Console(); }
	IInput *Input() const { return m_pClient->Input(); }
	CCollision *Collision() const { return m_pClient->Collision(); }
	IClient *Client() const { return m_pClient->Client(); }
	IGraphics *Graphics() const { return m_pClient->Graphics(); }
	CGameWorld *GameWorld() const { return &m_pClient->m_GameWorld; }
	CTuningParams *Tuning() const { return m_pClient->m_Tuning; }

};
