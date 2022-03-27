#pragma once

#include <game/client/gameclient.h>
#include "game/client/fluffytw/f_component.h"

class FAimbot : public FComponent
{
public:
	FAimbot(CGameClient *pClient) :
		FComponent(pClient){}

	void Aimbot();
	void HookVisible(vec2 targetPos);

	// Checks
	bool InFov(float fov, vec2 dir);

	// Gets
	void GetClosestHitpoint();

	// Scans
	vec2 EdgeScan(int id);

	// Aim 
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos);

	// Thread
	void RunThread();
	void *m_pAimbotThread = NULL;

	// Globals
	bool m_CanAim = true;
	bool m_TargetVisible = false;
	int m_TargetId = 0;
	vec2 m_TargetPos = vec2(0, 0);
};
