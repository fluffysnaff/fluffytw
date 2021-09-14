#include "balance.h"

/*
	Example:
		Balance bot

	Description:
		This Balance bot will balance on a player in range of 401.
		It has velocity checks and introduces the use of InputStates. (see controls.cpp OnConsoleInit() for more)
*/

void HBalance::OnSnapInput()
{
	Balance();
} // this will execute each snap (see controls.cpp SnapInput())
  // OnSnapInput() is useful when working with input.
  // You should execute your hacks here



void HBalance::Balance()
{
	if(!m_pClient->m_Snap.m_pLocalCharacter)
		return; // so that we don't balance when we are not connected
	if(!m_pClient->m_Controls.balanceonoff == 1)
		return; // don't execute if we stopped holding (see controls.cpp)
 	vec2 OwnVel = m_pClient->m_PredictedChar.m_Vel; // we use predicted char for more precise prediction
	vec2 OwnPos = m_pClient->m_PredictedChar.m_Pos + OwnVel; // + vel for velocity prediction
	vec2 closestPlayer = GetClosestPlayer(localid, 401, 360, CLOSEST_TYPE_NORMAL); 
	// g_Config.m_ClDummy returns 1 if you play with main and 0 if u play with dummy
	// if you always want dummy id for example you would do m_pClient->m_LocalIDs[!g_Config.m_ClDummy]
	if(closestPlayer == vec2(0, 0))
		return; // if id = -1 don't execute
	if(OwnPos.y != closestPlayer.y)
	{
		// see variables.h to see balance precision defined
		if((OwnPos.x - closestPlayer.x) - g_Config.m_ClBalancePrecision > 1)
		{
			BotInput(-1, local, INPUT_DIRECTION);
		}
		if((OwnPos.x - closestPlayer.x) + g_Config.m_ClBalancePrecision < 1)
		{
			BotInput(1, local, INPUT_DIRECTION);
		}
	}
}
