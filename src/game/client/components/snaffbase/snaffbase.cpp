#include "snaffbase.h"

void SnaffHack::BotMove(int direction, int dummy)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &GameClient()->m_DummyInput;
	else
	{
		dbg_msg("botmove", "Error: Local id not found!");
		return;
	}
	m_Input->m_Direction = direction;
}

void SnaffHack::BotJump(int stroke, int dummy)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &GameClient()->m_DummyInput;
	else
	{
		dbg_msg("botjump", "Error: Local id not found!");
		return;
	}
	m_Input->m_Jump = stroke;
}

void SnaffHack::BotHook(int stroke, int dummy)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &GameClient()->m_DummyInput;
	else
	{
		dbg_msg("bothook", "Error: Local id not found!");
		return;
	}
	m_Input->m_Hook = stroke;
}

void SnaffHack::BotFire(int stroke, int dummy)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &GameClient()->m_DummyInput;
	else
	{
		dbg_msg("botfire", "Error: Local id not found!");
		return;
	}
	m_Input->m_Fire = stroke;
}

void SnaffHack::BotAim(vec2 pos, int dummy, int type)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	vec2 *m_MousePos;
	if(dummy == g_Config.m_ClDummy)
	{
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
		m_MousePos = &GameClient()->m_Controls.m_MousePos[dummy];
	}
	else if(dummy == !g_Config.m_ClDummy)
	{
		m_Input = &GameClient()->m_DummyInput;
		m_MousePos = &GameClient()->m_Controls.m_MousePos[dummy];
	}
	else
	{
		dbg_msg("botaim", "Error: Local id not found!");
		return;
	}

	if(type == TYPE_TARGET)
	{
		m_Input->m_TargetX = static_cast<int>(pos.x);
		m_Input->m_TargetY = static_cast<int>(pos.y);
	}
	else if(type == TYPE_MOUSE)
	{
		m_Input->m_TargetX = static_cast<int>(pos.x);
		m_Input->m_TargetY = static_cast<int>(pos.y);
		m_MousePos->x = m_Input->m_TargetX;
		m_MousePos->y = m_Input->m_TargetY;
	}
}

void SnaffHack::BotAim(float posx, float posy, int dummy, int type)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	vec2 *m_MousePos;
	if(dummy == g_Config.m_ClDummy)
	{
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
		m_MousePos = &GameClient()->m_Controls.m_MousePos[dummy];
	}
	else if(dummy == !g_Config.m_ClDummy)
	{
		m_Input = &GameClient()->m_DummyInput;
		m_MousePos = &GameClient()->m_Controls.m_MousePos[dummy];
	}
	else
	{
		dbg_msg("botaim", "Error: Local id not found!");
		return;
	}

	if(type == TYPE_TARGET)
	{
		m_Input->m_TargetX = static_cast<int>(posx);
		m_Input->m_TargetX = static_cast<int>(posy);
	}
	else if(type == TYPE_MOUSE)
	{
		m_Input->m_TargetX = static_cast<int>(posx);
		m_Input->m_TargetX = static_cast<int>(posy);
		m_MousePos->x = m_Input->m_TargetX;
		m_MousePos->y = m_Input->m_TargetY;
	}
}

void SnaffHack::BotSetWeapon(int weapon, int dummy)
{
	if(!GameClient())
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &GameClient()->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &GameClient()->m_DummyInput;
	else
	{
		dbg_msg("botsetweapon", "Error: Local id not found!");
		return;
	}

	static bool has_switched = false;
	if(has_switched)
	{
		has_switched = false;
		return;
	}
	if(m_Input->m_WantedWeapon == weapon)
	{
		return;
	}
	m_Input->m_WantedWeapon = weapon;
	has_switched = true;
}

void SnaffHack::BotDie(int dummy)
{
	if(!GameClient())
		return;
	if(dummy != g_Config.m_ClDummy || !g_Config.m_ClDummy)
		return;
	GameClient()->SendKill(dummy);
}


bool SnaffHack::TicksPassed(int ticks) 
{ 
	if(!GameClient())
		return -1;
	return GameClient()->m_GameWorld.m_GameTick % ticks == 0; 
}

bool SnaffHack::IsGrounded(int dummy)
{
	if(!GameClient())
		return -1;
	float m_ProximityRadius = 28;
	// m_aClients[dummyid].m_Predicted.m_Pos
	if(GameClient()->Collision()->CheckPoint(GameClient()->m_aClients[dummy].m_Predicted.m_Pos.x + m_ProximityRadius / 2, GameClient()->m_aClients[dummy].m_Predicted.m_Pos.y + m_ProximityRadius / 2 + 5))
		return true;
	if(GameClient()->Collision()->CheckPoint(GameClient()->m_aClients[dummy].m_Predicted.m_Pos.x - m_ProximityRadius / 2, GameClient()->m_aClients[dummy].m_Predicted.m_Pos.y + m_ProximityRadius / 2 + 5))
		return true;
	int MoveRestrictionsBelow = GameClient()->Collision()->GetMoveRestrictions(GameClient()->m_aClients[dummy].m_Predicted.m_Pos + vec2(0, m_ProximityRadius / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
	{
		return true;
	}

	return false;
}

bool SnaffHack::IsFrozen(int dummy)
{
	if(!GameClient())
		return -1;
	if(GameClient()->m_aClients[dummy].m_Snapped.m_Armor < 10)
		return true;
	return false;
}

int SnaffHack::Jumps(int dummy)
{
	if(!GameClient())
		return -1;
	return GameClient()->m_GameWorld.m_Core.m_apCharacters[dummy]->m_Jumps; 
}
int SnaffHack::Jumped(int dummy) 
{ 
	if(!GameClient())
		return -1;
	return GameClient()->m_GameWorld.m_Core.m_apCharacters[dummy]->m_Jumped;
}
