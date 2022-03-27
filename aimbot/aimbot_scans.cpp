#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

vec2 FAimbot::EdgeScan(int id)
{
	vec2 myPos = m_pClient->m_PredictedChar.m_Pos;
	vec2 myVel = m_pClient->m_PredictedChar.m_Vel;
	vec2 targetVel = m_pClient->m_aClients[id].m_Predicted.m_Vel;
	vec2 targetPos = m_pClient->m_aClients[id].m_Predicted.m_Pos;

	int hitPointsCount = 0;
	vec2 hitPoints[MAX_HITPOINTS];

	if(!fHelper->PredictHook(myPos, myVel, targetPos, targetVel, Tuning()->m_HookFireSpeed))
		return vec2(0, 0);
	if(fHelper->HitScanHook(myPos, targetPos, targetPos - myPos, PHYS_SIZE))
		return targetPos - myPos; // first, scan for normal direction only
	if(!fConfig->aimbotCfg.edge)
		return vec2(0, 0); // edge scan is disabled, normal scan failed

	switch(fConfig->aimbotCfg.scanType)
	{
	case 0:
	{
		const float visibleAngle = atan2(targetPos.y - myPos.y, targetPos.x - myPos.x) + pi / 2;
		for(float i = visibleAngle; i < pi + visibleAngle; i += fConfig->aimbotCfg.accuracy / 10.f)
		{
			if(hitPointsCount >= MAX_HITPOINTS)
				break;
			const float x = targetPos.x + (cosf(i) * PHYS_SIZE);
			const float y = targetPos.y + (sinf(i) * PHYS_SIZE);
			vec2 pos = vec2(static_cast<int>(x), static_cast<int>(y));
			const vec2 dir = pos - myPos;
			if(fHelper->HitScanHook(myPos, targetPos, dir, PHYS_SIZE))
			{
				hitPoints[hitPointsCount] = dir;
				hitPointsCount++;
			}
		}
		if(hitPointsCount > 0)
		{
			const int middleIndex = (hitPointsCount - 1) / 2;
			return hitPoints[middleIndex];
		}
		break;
	}
	case 1:
	{
		for(float i = 0; i < fConfig->aimbotCfg.fov * (180.f / pi); i += fConfig->aimbotCfg.accuracy / 10.f)
		{
			if(hitPointsCount >= MAX_HITPOINTS)
				break;
			const vec2 dir = (normalize(direction(i)) * PHYS_SIZE + targetPos) - myPos;
			if(fHelper->HitScanHook(myPos, targetPos, dir, PHYS_SIZE))
			{
				hitPoints[hitPointsCount] = dir;
				hitPointsCount++;
			}
		}
		if(hitPointsCount > 0)
		{
			const int middleIndex = (hitPointsCount - 1) / 2;
			return hitPoints[middleIndex];
		}
		break;
	}
	default:
		break;
	}
	return vec2(0, 0);
}