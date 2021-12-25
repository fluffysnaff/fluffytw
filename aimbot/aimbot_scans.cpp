#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

vec2 FAimbot::HitpointScan(int id)
{
	const vec2 myPos = m_pClient->m_PredictedChar.m_Pos;
	const vec2 myVel = m_pClient->m_PredictedChar.m_Vel;
	vec2 targetVel = m_pClient->m_aClients[id].m_Predicted.m_Vel;
	vec2 targetPos = m_pClient->m_aClients[id].m_Predicted.m_Pos;

	fHelper->PredictHook(myPos, myVel, targetPos, targetVel);
	if(fHelper->HitScanHook(myPos, m_pClient->m_aClients[id].m_Predicted.m_Pos + normalize(targetPos), targetPos, PHYS_SIZE))
	{
		return targetPos;
	}
	return vec2(0, 0);
}
