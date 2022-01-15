#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

vec2 FAimbot::HitpointScan(int id, bool edge, float accuracy)
{
	const vec2 myPos = m_pClient->m_PredictedChar.m_Pos;
	const vec2 myVel = m_pClient->m_PredictedChar.m_Vel;
	const vec2 targetVel = m_pClient->m_aClients[id].m_Predicted.m_Vel;
	const vec2 targetPos = m_pClient->m_aClients[id].m_Predicted.m_Pos;
 
	int hitPointsCount = 0;
	vec2 hitPoints[MAX_HITPOINTS];

	vec2 temp = targetPos;
	if(!fHelper->PredictHook(myPos, myVel, temp, targetVel))
		return vec2(0, 0);
	if(fHelper->HitScanHook(myPos, targetPos, temp, PHYS_SIZE))
		return temp; // first, scan for normal direction only
	if(!edge)
		return vec2(0, 0); // edge scan is disabled, normal scan failed

	// accuracy / 50 should be better, but it's not threaded yet
	const float visibleAngle = atan2(targetPos.y - myPos.y, targetPos.x - myPos.x) + pi / 2;
	for(float i = visibleAngle; i < pi + visibleAngle; i += accuracy / 25)
	{
		if(hitPointsCount >= MAX_HITPOINTS)
			break;
		const float x = targetPos.x + (cosf(i) * PHYS_SIZE);
		const float y = targetPos.y + (sinf(i) * PHYS_SIZE);
		vec2 pos = vec2(static_cast<int>(x), static_cast<int>(y));

		if(!fHelper->PredictHook(myPos, myVel, pos, targetVel))
			return vec2(0, 0);
		if(fHelper->HitScanHook(myPos, targetPos, pos, PHYS_SIZE))
		{
			hitPoints[hitPointsCount] = pos;
			hitPointsCount++;
		}
	}
	if(hitPointsCount > 0)
	{
		const int middleIndex = (hitPointsCount - 1) / 2;
		return hitPoints[middleIndex];
	}
	return vec2(0, 0);
}