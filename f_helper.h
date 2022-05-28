#pragma once

#include "f_bots.h"
#include "f_visuals.h"
#include "aimbot/aimbot.h"

class FHelper
{
protected:
	[[nodiscard]] CGameClient*   GameClient() const { return m_pClient; }
	[[nodiscard]] CControls*     Controls()	  const { return &m_pClient->m_Controls; }
	[[nodiscard]] IConsole*		 Console()    const { return m_pClient->Console(); }
	[[nodiscard]] IInput*		 Input()      const { return m_pClient->Input(); }
	[[nodiscard]] CCollision*	 Collision()  const { return m_pClient->Collision(); }
	[[nodiscard]] IClient*       Client()     const { return m_pClient->Client(); }
	[[nodiscard]] IGraphics*     Graphics()   const { return m_pClient->Graphics(); }
	[[nodiscard]] CGameWorld*	 GameWorld()  const { return &m_pClient->m_GameWorld; }
	[[nodiscard]] CTuningParams* Tuning()     const { return m_pClient->m_Tuning; }

public:
	FHelper(CGameClient *client);
	~FHelper();

	// CGameClient pointer
	CGameClient* m_pClient = nullptr;

	// fluffytw pointers
	FBots*		 m_pBots = nullptr;
	FAimbot*     m_pAimbot = nullptr;
	FVisuals*    m_pVisuals = nullptr;

	bool HitScanHook(vec2 InitPos, vec2 targetPos, vec2 scanDir, float radius);
	bool IntersectCharacter(vec2 hookPos, vec2 targetPos, vec2 &newPos);

	bool PredictHook(vec2 &myPos, vec2 myVel, vec2 &targetPos, vec2 targetVel, float speed);
	void PredictHookOut(vec2 &myPos, vec2 &myVel, vec2 &targetPos, vec2 &targetVel);
	void TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray);

	int GetCustomTile(float x, float y) const;
	int GetClosestId(int fov = 360, float range = 408.f); // hook_length + phys_size = 408.f
	float GetPing() const;

	bool IsLocalActive();
	bool IsValidId(int id);
	bool IsGrounded(int id, vec2 pos = vec2(0, 0));
};

extern std::unique_ptr<FHelper> fHelper;
