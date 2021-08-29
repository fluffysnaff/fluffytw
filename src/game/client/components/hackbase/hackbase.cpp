#include "hackbase.h"

void Hack::BotMove(int direction, int dummy)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
	else
	{
		dbg_msg("botmove", "Error: Local id not found!");
		return;
	}
	m_Input->m_Direction = direction;
}

void Hack::BotJump(int stroke, int dummy)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
	else
	{
		dbg_msg("botjump", "Error: Local id not found!");
		return;
	}
	m_Input->m_Jump = stroke;
}

void Hack::BotHook(int stroke, int dummy)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
	else
	{
		dbg_msg("bothook", "Error: Local id not found!");
		return;
	}
	m_Input->m_Hook = stroke;
}

void Hack::BotFire(int stroke, int dummy)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
	else
	{
		dbg_msg("botfire", "Error: Local id not found!");
		return;
	}
	m_Input->m_Fire = stroke;
}

void Hack::BotAim(vec2 pos, int dummy, int type)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	vec2 *m_MousePos;
	if(dummy == g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
		m_MousePos = &m_pClient->m_Controls.m_MousePos[dummy];
	}
	else if(dummy == !g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_DummyInput;
		m_MousePos = &m_pClient->m_Controls.m_MousePos[dummy];
	}
	else
	{
		dbg_msg("botaim", "Error: Local id not found!");
		return;
	}

	if(type == AIM_TYPE_TARGET)
	{
		m_Input->m_TargetX = static_cast<int>(pos.x);
		m_Input->m_TargetY = static_cast<int>(pos.y);
	}
	else if(type == AIM_TYPE_MOUSE)
	{
		m_Input->m_TargetX = static_cast<int>(pos.x);
		m_Input->m_TargetY = static_cast<int>(pos.y);
		m_MousePos->x = m_Input->m_TargetX;
		m_MousePos->y = m_Input->m_TargetY;
	}
}

void Hack::BotAim(float posx, float posy, int dummy, int type)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	vec2 *m_MousePos;
	if(dummy == g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
		m_MousePos = &m_pClient->m_Controls.m_MousePos[dummy];
	}
	else if(dummy == !g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_DummyInput;
		m_MousePos = &m_pClient->m_Controls.m_MousePos[dummy];
	}
	else
	{
		dbg_msg("botaim", "Error: Local id not found!");
		return;
	}

	if(type == AIM_TYPE_TARGET)
	{
		m_Input->m_TargetX = static_cast<int>(posx);
		m_Input->m_TargetX = static_cast<int>(posy);
	}
	else if(type == AIM_TYPE_MOUSE)
	{
		m_Input->m_TargetX = static_cast<int>(posx);
		m_Input->m_TargetX = static_cast<int>(posy);
		m_MousePos->x = m_Input->m_TargetX;
		m_MousePos->y = m_Input->m_TargetY;
	}
}

void Hack::BotSetWeapon(int weapon, int dummy)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	if(dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[dummy];
	else if(dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
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

void Hack::BotDie(int dummy)
{
	if(!m_pClient)
		return;
	if(dummy != g_Config.m_ClDummy || !g_Config.m_ClDummy)
		return;
	m_pClient->SendKill(dummy);
}


vec2 Hack::GetClosestPlayer(int ownId, int range, int type)
{
	if(!m_pClient)
		return vec2(0, 0);
	vec2 EnemyPos = vec2(0, 0);

	vec2 OwnVel = m_pClient->m_PredictedChar.m_Vel;
	vec2 OwnPos = m_pClient->m_PredictedChar.m_Pos + OwnVel;

	int ID = -1;
	int Range = 401;

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(i == ownId)
			continue;
		EnemyPos = vec2(m_pClient->m_Snap.m_aCharacters[i].m_Cur.m_X, m_pClient->m_Snap.m_aCharacters[i].m_Cur.m_Y);
		int Dist = distance(EnemyPos, OwnPos);
		if(!m_pClient->m_Snap.m_aCharacters[i].m_Active)
			continue;
		if(Dist < Range)
		{
			ID = i;
			Range = Dist;
		}
	}
	if(ID != -1)
	{
		if(type == CLOSEST_TYPE_NORMAL)
			return vec2(m_pClient->m_Snap.m_aCharacters[ID].m_Cur.m_X, m_pClient->m_Snap.m_aCharacters[ID].m_Cur.m_Y);
		else if(type == CLOSEST_TYPE_AIMBOT)
		{
			EnemyPos = vec2(m_pClient->m_Snap.m_aCharacters[ID].m_Cur.m_X, m_pClient->m_Snap.m_aCharacters[ID].m_Cur.m_Y);
			vec2 FinishPos;
			int TeleNr;
			if(!m_pClient->Collision()->IntersectLineTeleHook(OwnPos, EnemyPos, &FinishPos, 0x0, &TeleNr))
				return FinishPos;
		}
	}
	return vec2(0, 0);
}

vec2 Hack::NormalizeAim(vec2 Pos, int dummy)
{
	if(!m_pClient)
		return vec2(0, 0);
	return normalize(Pos) * length(m_pClient->m_Controls.m_MousePos[dummy]);
}


bool Hack::TicksPassed(int ticks) 
{ 
	if(!m_pClient)
		return -1;
	return m_pClient->m_GameWorld.m_GameTick % ticks == 0; 
}

bool Hack::IsGrounded(int dummy)
{
	if(!m_pClient)
		return -1;
	float m_ProximityRadius = 28;
	// m_aClients[dummyid].m_Predicted.m_Pos
	if(m_pClient->Collision()->CheckPoint(m_pClient->m_aClients[dummy].m_Predicted.m_Pos.x + m_ProximityRadius / 2, m_pClient->m_aClients[dummy].m_Predicted.m_Pos.y + m_ProximityRadius / 2 + 5))
		return true;
	if(m_pClient->Collision()->CheckPoint(m_pClient->m_aClients[dummy].m_Predicted.m_Pos.x - m_ProximityRadius / 2, m_pClient->m_aClients[dummy].m_Predicted.m_Pos.y + m_ProximityRadius / 2 + 5))
		return true;
	int MoveRestrictionsBelow = m_pClient->Collision()->GetMoveRestrictions(m_pClient->m_aClients[dummy].m_Predicted.m_Pos + vec2(0, m_ProximityRadius / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
	{
		return true;
	}

	return false;
}

bool Hack::IsFrozen(int dummy)
{
	if(!m_pClient)
		return -1;
	if(m_pClient->m_aClients[dummy].m_Snapped.m_Armor < 10)
		return true;
	return false;
}

int Hack::Jumps(int dummy)
{
	if(!m_pClient)
		return -1;
	return m_pClient->m_GameWorld.m_Core.m_apCharacters[dummy]->m_Jumps; 
}

int Hack::Jumped(int dummy) 
{ 
	if(!m_pClient)
		return -1;
	return m_pClient->m_GameWorld.m_Core.m_apCharacters[dummy]->m_Jumped;
}
