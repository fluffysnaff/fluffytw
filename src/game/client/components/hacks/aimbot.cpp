#include "aimbot.h"

/*
	Example:
		Basic Aimbot

	Description:
		This Aimbot will aim at a players position in 360 angle.
		It has a few velocity checks and works only when pressing
		the hook button.
*/

void HAimbot::OnSnapInput()
{
	Aimbot();
}

void HAimbot::Aimbot()
{
	if(!m_pClient->m_Snap.m_pLocalCharacter)
		return; // so that we don't aim when we aren't ingame
	if(!g_Config.m_ClAimbot)
		return; // don't execute if aimbot is off
	if(!m_pClient->m_Controls.aimbotonoff == 1)
		return;
	vec2 ownVel = m_pClient->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction
	vec2 ownPos = m_pClient->m_PredictedChar.m_Pos + ownVel; // + vel for velocity prediction
	vec2 closestPlayer = GetClosestPlayer(m_pClient->m_LocalIDs[g_Config.m_ClDummy], 500, CLOSEST_TYPE_AIMBOT);
	if(closestPlayer == vec2(0, 0))
		return; // if id = -1 don't execute
	closestPlayer = NormalizeAim(closestPlayer - ownPos, g_Config.m_ClDummy);
	if(g_Config.m_ClAimbotSilent)
		BotAim(closestPlayer, g_Config.m_ClDummy, AIM_TYPE_TARGET);
	else
		BotAim(closestPlayer, g_Config.m_ClDummy, AIM_TYPE_MOUSE);
}
