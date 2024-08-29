#include "aimbot.h"
#include "game/client/fluffytw/f_helper.h"

vec2 FAimbot::EdgeScan()
{
	int hitPointsCount = 0;
	vec2 hitPoints[MAX_HITPOINTS];

	vec2 myPos = m_MyPos;
	vec2 targetPos = m_TargetPos;

	m_TargetVisible = false;

	// Predict hook and return, if it's impossible
	if(!PredictHook(myPos, m_MyVel, targetPos, m_TargetVel))
		return vec2(0, 0);

	// If player is hookable right away, return the position
	if(HitScanHook(myPos, targetPos, targetPos - myPos))
	{
		m_TargetVisible = true;
		return targetPos - myPos;
	}

	// If hitpoint scan is disabled and normal scan failed, return
	if(!fHelper->m_pConfig->aimbotCfg.edge)
		return vec2(0, 0);

	/* Gets the angle we should be able to hook
	 *
	 * a = visibleAngle 
	 *
	 * myPos
	 * |\
	 * | \
	 * |  \
	 * |__a\
	 *      targetPos
	*/
	const float visibleAngle = atan2(targetPos.y - myPos.y, targetPos.x - myPos.x) + pi * 0.5f;
	for(float i = visibleAngle; i < pi + visibleAngle; i += 1.f / fHelper->m_pConfig->aimbotCfg.accuracy)
	{
		// Return if we have enough hitpoints
		if(hitPointsCount >= MAX_HITPOINTS)
			break;

		// Convert desired angle(hitpoint) to Cartesian coordinates
		auto pos = vec2(static_cast<int>(targetPos.x + cosf(i) * PHYS_SIZE),
			static_cast<int>(targetPos.y + sinf(i) * PHYS_SIZE));
		const vec2 dir = pos - myPos;

		// Check if hitpoint is hookable and if it is
		// append it to `hitPoints` and increase `hitPointsCount`
		if(HitScanHook(myPos, targetPos, dir))
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
		m_TargetVisible = true;
		m_TargetPos = hitPoints[middleIndex];
		return hitPoints[middleIndex];
	}
	return vec2(0, 0);
}
