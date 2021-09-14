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
	DoThread();
	Aimbot();
}

void HAimbot::Aimbot()
{
	if(!m_pClient->m_Snap.m_pLocalCharacter)
		return; // so that we don't aim when we aren't ingame
	if(!g_Config.m_ClAimbot)
		return; // don't execute if aimbot is of
	if(GetInput(local, INPUT_HOOK) != 1)
		return; // execute only when we hook
	if(AimbotResult == vec2(0, 0))
		return; // don't execute if no player was found
	vec2 ownVel = m_pClient->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction NOTE: do m_Vel / 256.f when checking for intersections
	vec2 ownPos = m_pClient->m_PredictedChar.m_Pos + ownVel; // + vel for velocity prediction
	vec2 ClosestPlayer = NormalizeAim(AimbotResult - ownPos, local);
	if(g_Config.m_ClAimbotSilent)
		BotInput(ClosestPlayer, local, AIM_TYPE_TARGET);
	else
		BotInput(ClosestPlayer, local, AIM_TYPE_MOUSE);
}



void HAimbot::AimbotVoid() // what executes in the thread
{
	if(!m_pClient->m_Snap.m_pLocalCharacter)
		return;
	AimbotResult = GetClosestPlayer(localid, 440, g_Config.m_ClAimbotAngle, CLOSEST_TYPE_AIMBOT);
}

void HAimbot::AimbotThread(void *pUser) // the static void for threads
{
	while(1)
	{
		HAimbot *pThis = (HAimbot *)pUser;
		pThis->AimbotVoid();
		thread_sleep(1);
	}
}

void HAimbot::DoThread() // function for calling the thread
{
	if(!started)
	{
		started = true;
		m_pAimbotThread = thread_init(AimbotThread, this, "AimbotThread");
		thread_detach(m_pAimbotThread);
	}
}
