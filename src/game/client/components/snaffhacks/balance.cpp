#include "balance.h"

/*
	Example:
		Balance bot

	Description:
		This Balance bot will balance on a player in 401 range.
		It has velocity checks and introduces the use of InputStates. (see controls.cpp OnConsoleInit() for more)
*/

HBalance::HBalance(CGameClient *pClient) :
	SnaffHack(pClient)
{
	return;
} // this will execute when you create new instance of HBalance


void HBalance::OnSnapInput()
{
	Balance();
} // this will execute each snap (see controls.cpp SnapInput())
  // OnSnapInput() is useful when working with input.
  // You should execute your hacks here


vec2 HBalance::GetClosestPlayer(int ownId)
{
	vec2 EnemyPos = vec2(0, 0);

	vec2 OwnVel = GameClient()->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction
	vec2 OwnPos = GameClient()->m_PredictedChar.m_Pos + OwnVel;  // + OwnVel; // + vel for velocity prediction
	
	int ID = -1;
	int Range = 401; // 401 is around the hook length

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
		return vec2(GameClient()->m_Snap.m_aCharacters[ID].m_Cur.m_X, GameClient()->m_Snap.m_aCharacters[ID].m_Cur.m_Y);
	}
	return vec2(0, 0); // returns if ID = -1
}

void HBalance::Balance()
{
	if(!GameClient()->m_Snap.m_pLocalCharacter)
		return; // so that we don't balance when we are not connected
	if(!GameClient()->m_Controls.balanceonoff == 1)
		return; // don't execute if we stopped holding (see controls.cpp)
 	vec2 OwnVel = GameClient()->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction
	vec2 OwnPos = GameClient()->m_PredictedChar.m_Pos + OwnVel; // + vel for velocity prediction

	vec2 closestPlayer = GetClosestPlayer(GameClient()->m_LocalIDs[g_Config.m_ClDummy]); 
	// g_Config.m_ClDummy returns 1 if you play with main and 0 if u play with dummy
	// if you always want dummy id for example you would do m_pClient->m_LocalIDs[!g_Config.m_ClDummy]
	if(closestPlayer == vec2(0, 0))
		return; // if id = -1 don't execute
	if(OwnPos.y != closestPlayer.y)
	{
		// see variables.h to see balance precision defined
		if((OwnPos.x - closestPlayer.x) - g_Config.m_ClBalancePrecision > 1)
		{
			BotMove(-1, g_Config.m_ClDummy); // BotMove(direction, dummy), -1 is left 1 is right, (see snaffbase.cpp)
		}
		if((OwnPos.x - closestPlayer.x) + g_Config.m_ClBalancePrecision < 1)
		{
			BotMove(1, g_Config.m_ClDummy);
		}
	}
}
