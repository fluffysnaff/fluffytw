#include "aimbot.h"
#include "game/client/fluffytw/f_helper.h"

vec2 FAimbot::EdgeScan(int id)
{
	// Constants
	vec2 myPos = m_pClient->m_PredictedChar.m_Pos;
	vec2 myVel = m_pClient->m_PredictedChar.m_Vel;
	vec2 targetVel = m_pClient->m_aClients[id].m_Predicted.m_Vel;
	vec2 targetPos = m_pClient->m_aClients[id].m_Predicted.m_Pos;

	int hitPointsCount = 0;
	vec2 hitPoints[MAX_HITPOINTS];

    // Predict hook and return, if it's impossible
	if(!fHelper->PredictHook(myPos, myVel, targetPos, targetVel, Tuning()->m_HookFireSpeed))
		return vec2(0, 0);

    // If player is hookable right away, return the position
	if(fHelper->HitScanHook(myPos, targetPos, targetPos - myPos, PHYS_SIZE))
		return targetPos - myPos;

	// If hitpoint scan is disabled and normal scan failed, return
	if(!fConfig->aimbotCfg.edge)
		return vec2(0, 0);

	/* Gets the angle range we should be able to hook
	 *
	 * a = the range
	 * myPos
	 * |\
	 * | \
	 * |  \
	 * |__a\
	 *      targetPos
	*/
	const float visibleAngle = atan2(targetPos.y - myPos.y, targetPos.x - myPos.x) + pi / 2;
	const float mouseAngle = angle(Controls()->m_MousePos[LOCAL]);
	for(float i = mouseAngle + visibleAngle; i < fConfig->aimbotCfg.fov + mouseAngle + visibleAngle; i += 1 / fConfig->aimbotCfg.accuracy)
	{
		// Return if we have enough hitpoints
		if(hitPointsCount >= MAX_HITPOINTS)
			break;

		// Convert desired angle(hitpoint) to Cartesian coordinates
		vec2 pos = vec2(static_cast<int>(targetPos.x + (cosf(i) * PHYS_SIZE)), 
			static_cast<int>(targetPos.y + (sinf(i) * PHYS_SIZE)));
		const vec2 dir = pos - myPos;

		// Check if hitpoint is hookable and if it is
		// append it to `hitPoints` and increase `hitPointsCount`
		if(fHelper->HitScanHook(myPos, targetPos, dir, PHYS_SIZE))
		{
			hitPoints[hitPointsCount] = dir;
			hitPointsCount++;
		}
	}

	// If hitpoints were found
	// return the best(i.e. middle) hitpoint position
	if(hitPointsCount > 0)
	{
		// Calculate the middle index of `hitPoints` array
		const int middleIndex = (hitPointsCount - 1) / 2;
		return hitPoints[middleIndex];
	}
	return vec2(0, 0);
}