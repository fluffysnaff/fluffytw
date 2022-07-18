#pragma once

#include "game/client/fluffytw/f_component.h"
#include <game/client/gameclient.h>

class FAimbot : public FComponent
{
public:
	FAimbot(CGameClient *pClient) :
		FComponent(pClient) {}

	void Aimbot();
	void HookVisible(vec2 targetPos);

	// Gets
	void GetClosestHitpoint();
	int GetClosestId(int fov = 360, float range = 395.f);
	[[nodiscard]] float GetPing() const;

	// Helpers
	bool PredictHook(vec2 &myPos, vec2 myVel, vec2 &targetPos, vec2 targetVel);
	bool HitScanHook(vec2 InitPos, vec2 targetPos, vec2 scanDir);
	bool IntersectCharacter(vec2 hookPos, vec2 targetPos, vec2 &newPos);

	// Scans
	vec2 EdgeScan();

	// Aim
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos);

	// Check
	bool InFov(float fov, vec2 dir);

	// Globals
	bool m_CanAim = true;
	bool m_TargetVisible = false;
	int m_TargetId = 0;

	vec2 m_MyPos = vec2(0, 0);
	vec2 m_MyVel = vec2(0, 0);
	vec2 m_TargetPos = vec2(0, 0);
	vec2 m_TargetVel = vec2(0, 0);
};
