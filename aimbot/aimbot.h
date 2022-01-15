#pragma once

#include <game/client/gameclient.h>
#include "game/client/fluffytw/f_component.h"

class FAimbot : public FComponent
{
public:
	FAimbot(CGameClient *pClient) :
		FComponent(pClient){}

	void Aimbot(FConfig::AimbotConfig *cfg);
	void HookVisible(bool cfgVar, bool cfgSilent, vec2 targetPos);

	// Checks
	bool InFov(int fov, vec2 dir);

	// Gets
	vec2 GetClosestHitpoint(FConfig::AimbotConfig *cfg);

	// Scans
	vec2 HitpointScan(int id, bool edge, float accuracy);

	// Aim stuff
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos, int silent);

	// GLOBALS
	bool m_CanAim = true;
	bool m_TargetVisible = false;
};

#define MAX_HITPOINTS 32
