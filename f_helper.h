#pragma once

#include <memory>

#include "f_bots.h"
#include "f_visuals.h"
#include "aimbot/aimbot.h"

class FHelper
{
protected:
	[[nodiscard]] CGameClient*	  GameClient()	  const { return m_pClient; }
	[[nodiscard]] CControls*	  Controls()	  const { return &m_pClient->m_Controls; }
	[[nodiscard]] IConsole*		  Console()		  const { return m_pClient->Console(); }
	[[nodiscard]] IInput*		  Input()		  const { return m_pClient->Input(); }
	[[nodiscard]] CCollision*	  Collision()	  const { return m_pClient->Collision(); }
	[[nodiscard]] IClient*		  Client()		  const { return m_pClient->Client(); }
	[[nodiscard]] IGraphics*	  Graphics()	  const { return m_pClient->Graphics(); }
	[[nodiscard]] CGameWorld*	  GameWorld()	  const { return &m_pClient->m_GameWorld; }
	[[nodiscard]] CGameWorld*     PredGameWorld() const { return &m_pClient->m_PredictedWorld; }
	[[nodiscard]] CTuningParams*  Tuning()		  const { return m_pClient->m_aTuning; }
	[[nodiscard]] IServerBrowser* ServerBrowser() const { return m_pClient->ServerBrowser(); }

public:
	FHelper(CGameClient *client) noexcept;

	// Main pointer(needs to be raw)
	CGameClient* m_pClient = nullptr;

	// Pointers
	std::unique_ptr<FBots>    m_pBots;
	std::unique_ptr<FAimbot>  m_pAimbot;
	std::unique_ptr<FVisuals> m_pVisuals;

	void  TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray);
	int   GetCustomTile(float x, float y) const;
	int   GetClosestId(int fov = 360, float range = 394.f); // hook_length + phys_size * 0.5f = 390.f

	bool  IsValidId(int id);
	bool  IsGrounded(int id, vec2 pos = vec2(0, 0));
};

extern std::unique_ptr<FHelper> fHelper;
