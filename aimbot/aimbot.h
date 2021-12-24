#pragma once

#include <game/client/gameclient.h>
#include "game/client/fluffytw/f_component.h"

class FAimbot : public FComponent
{
public:
	FAimbot(CGameClient *pClient) :
		FComponent(pClient){}
	void Aimbot(FConfig::AimbotConfig cfg);

	// Gets
	vec2 GetClosestHitpoint(int fov);
	vec2 HitpointScan(int id);

	// Aim stuff
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos, int silent);

	// Checks
	bool InFov(int fov, vec2 dir);

public:
	// GLOBALS
	bool m_CanAim = true;
	int m_StartTick = 0;
};
