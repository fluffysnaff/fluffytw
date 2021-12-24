#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

vec2 FAimbot::HitpointScan(int id)
{
	const vec2 myPos = m_pClient->m_PredictedChar.m_Pos;
	const vec2 targetVel = m_pClient->m_aClients[id].m_Predicted.m_Vel;
	vec2 targetPos = m_pClient->m_aClients[id].m_Predicted.m_Pos;

	const vec2 scanDir = targetPos - myPos;

	if(fHelper->HitScanHook(myPos, scanDir, PHYS_SIZE))
	{
		fHelper->PredictHook(myPos, targetPos, targetVel);
		return targetPos - myPos;
	}
	return vec2(0, 0);
}
