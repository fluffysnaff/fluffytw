#include "aimbot.h"

/*
	Example:
		Basic Aimbot

	Description:
		This Aimbot will aim at a players position in 360 angle.
		It has a few velocity checks and works only when pressing
		the hook button.
*/

HAimbot::HAimbot(CGameClient *pClient) : SnaffHack(pClient)
{
	return;
} // this will execute when you create new instance of HAimbot (ie. in gameclient.cpp


void HAimbot::OnSnapInput()
{
	Aimbot();
}

vec2 HAimbot::GetClosestPlayer(int ownId)
{
	vec2 EnemyPos = vec2(0, 0);

	vec2 OwnVel = GameClient()->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction
	vec2 OwnPos = GameClient()->m_PredictedChar.m_Pos + OwnVel; // + OwnVel; // + vel for velocity prediction

	int ID = -1;
	int Range = 500;

	for(int i = 0; i < MAX_CLIENTS; i++) // we loop trough all clients 0 - 63
	{
		if(i == ownId)
			continue; // try another id if the id is ownid;
		EnemyPos = vec2(GameClient()->m_Snap.m_aCharacters[i].m_Cur.m_X, GameClient()->m_Snap.m_aCharacters[i].m_Cur.m_Y);
		// you can also do m_pClient->m_aClients->m_Predicted.m_Pos, but character pos is more precise
		int Dist = distance(EnemyPos, OwnPos);
		if(!GameClient()->m_Snap.m_aCharacters[i].m_Active)
			continue; // check if the id is ingame
		if(Dist < Range)
		{
			ID = i;
			Range = Dist;
		}
	}
	if(ID != -1)
	{
		EnemyPos = vec2(GameClient()->m_Snap.m_aCharacters[ID].m_Cur.m_X, GameClient()->m_Snap.m_aCharacters[ID].m_Cur.m_Y);
		vec2 FinishPos; // required by intersectlinetelehook
		int TeleNr; // required by intersectlinetelehook
		// we are going to check if we can hook the player and if we can we return the position
		// if you want to do collision with weapons u can use IntersectLine; (see collision.cpp for more)
		if(!GameClient()->Collision()->IntersectLineTeleHook(OwnPos, EnemyPos, &FinishPos, 0x0, &TeleNr))
			return FinishPos;
		return vec2(0, 0);
	}
	return vec2(0, 0); // returns if ID = -1
}

void HAimbot::Aimbot()
{
	if(!GameClient()->m_Snap.m_pLocalCharacter)
		return; // so that we don't aim when we aren't ingame
	if(!g_Config.m_ClAimbot)
		return; // don't execute if aimbot is off
	if(!GameClient()->m_Controls.aimbotonoff == 1)
		return;
	vec2 OwnVel = GameClient()->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction
	vec2 OwnPos = GameClient()->m_PredictedChar.m_Pos + OwnVel; // + vel for velocity prediction

	vec2 closestPlayer = GetClosestPlayer(GameClient()->m_LocalIDs[g_Config.m_ClDummy]);
	if(closestPlayer == vec2(0, 0))
		return; // if id = -1 don't execute
	if(g_Config.m_ClAimbotSilent)
		BotAim(closestPlayer - OwnPos, g_Config.m_ClDummy, TYPE_TARGET);
	else
		BotAim(closestPlayer - OwnPos, g_Config.m_ClDummy, TYPE_MOUSE);
}
