#include "hackbase.h"

void Hack::BotInput(int stroke, int Dummy, int flag)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	if(Dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[Dummy];
	else if(Dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
	else
	{
		dbg_msg("botinput", "Error: Local id not found!");
		return;
	}

	if(flag == INPUT_FIRE)
	{
		if(stroke >= 2)
			m_Input->m_Fire += stroke;
		else
			m_Input->m_Fire = stroke;
	}
	else if(flag == INPUT_HOOK)
		m_Input->m_Hook = stroke;
	else if(flag == INPUT_JUMP)
		m_Input->m_Jump = stroke;
	else if(flag == INPUT_DIRECTION)
		m_Input->m_Direction = stroke;
	else if(flag == INPUT_WEAPON)
		m_Input->m_WantedWeapon = stroke;
	else if(flag == INPUT_DIE)
		m_pClient->SendKill(Dummy);
}

void Hack::BotInput(vec2 stroke, int Dummy, int flag)
{
	if(!m_pClient)
		return;
	CNetObj_PlayerInput *m_Input;
	vec2 *m_MousePos;
	if(Dummy == g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_Controls.m_InputData[Dummy];
		m_MousePos = &m_pClient->m_Controls.m_MousePos[Dummy];
	}
	else if(Dummy == !g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_DummyInput;
		m_MousePos = &m_pClient->m_Controls.m_MousePos[Dummy];
	}
	else
	{
		dbg_msg("botinput", "Error: Local id not found!");
		return;
	}

	if(flag == AIM_TYPE_TARGET)
	{
		if(m_pClient->m_Snap.m_aCharacters[localid].m_Cur.m_HookState != HOOK_RETRACTED)
		{
			m_Input->m_TargetX = static_cast<int>(stroke.x);
			m_Input->m_TargetY = static_cast<int>(stroke.y);
		}
	}
	else if(flag == AIM_TYPE_MOUSE)
	{
		if(m_pClient->m_Snap.m_aCharacters[localid].m_Cur.m_HookState != HOOK_RETRACTED)
		{
			m_Input->m_TargetX = static_cast<int>(stroke.x);
			m_Input->m_TargetY = static_cast<int>(stroke.y);
			m_MousePos->x = m_Input->m_TargetX;
			m_MousePos->y = m_Input->m_TargetY;
		}
	}
}

vec2 Hack::GetClosestPlayer(int ownId, int range, int angle, int type)
{
	if(!m_pClient->m_Snap.m_pLocalCharacter)
		return vec2(0, 0);

	vec2 EnemyPos = vec2(0, 0);
	vec2 OwnPos = m_pClient->m_PredictedChar.m_Pos;

	int ID = -1;
	int Range = range;
	int Angle = angle;

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(i == ownId)
			continue;
		EnemyPos = vec2(m_pClient->m_Snap.m_aCharacters[i].m_Cur.m_X, m_pClient->m_Snap.m_aCharacters[i].m_Cur.m_Y);
		int Dist = distance(EnemyPos, OwnPos);
		if(!m_pClient->m_Snap.m_aCharacters[i].m_Active)
			continue;
		EnemyPos = vec2(m_pClient->m_Snap.m_aCharacters[i].m_Cur.m_X, m_pClient->m_Snap.m_aCharacters[i].m_Cur.m_Y);
		if(Dist < Range)
		{
			if(lastID == -1 || lastID != ID)
			{
				ID = i;
				Range = Dist;
			}
		}
	}
	lastID = ID;
	if(lastID != -1)
	{
		Fluffhelper &m_pFluffhelper = Fluffhelper::getInstance();
		m_pFluffhelper.ActiveID = lastID;
		if(type == CLOSEST_TYPE_NORMAL)
			return vec2(m_pClient->m_Snap.m_aCharacters[lastID].m_Cur.m_X + m_pClient->m_Snap.m_aCharacters[lastID].m_Cur.m_VelX / 256.f,
				m_pClient->m_Snap.m_aCharacters[lastID].m_Cur.m_Y + m_pClient->m_Snap.m_aCharacters[lastID].m_Cur.m_VelY / 256.f);
		else if(type == CLOSEST_TYPE_AIMBOT)
		{
			double mouse_angle = GetAngle(m_pClient->m_Controls.m_MousePos[g_Config.m_ClDummy]);
			double aim_at_angle = GetAngle((m_pClient->m_aClients[lastID].m_Predicted.m_Pos) - OwnPos);
			double angle_difference = pi - abs(abs(aim_at_angle - mouse_angle) - pi);

			if(angle_difference > Angle * (pi / 180))
			{
				return vec2(0, 0);
			}

			Angle = angle_difference * (180 / pi);

			vec2 scan = LegitScan(lastID);

			if(scan != vec2(0, 0))
				return scan;
			else
				return vec2(0, 0);
		}
		return vec2(0, 0);
	}
	return vec2(0, 0);
}

vec2 Hack::NormalizeAim(vec2 Pos, int Dummy)
{
	if(!m_pClient)
		return vec2(0, 0);
	float CameraMaxDistance = 200.0f;
	float FollowFactor = (g_Config.m_ClDyncam ? g_Config.m_ClDyncamFollowFactor : g_Config.m_ClMouseFollowfactor) / 100.0f;
	float DeadZone = g_Config.m_ClDyncam ? g_Config.m_ClDyncamDeadzone : g_Config.m_ClMouseDeadzone;
	float MaxDistance = g_Config.m_ClDyncam ? g_Config.m_ClDyncamMaxDistance : g_Config.m_ClMouseMaxDistance;
	float MouseMax = minimum((FollowFactor != 0 ? CameraMaxDistance / FollowFactor + DeadZone : MaxDistance), MaxDistance);

	vec2 Direction = Pos;
	float MDistance = length(Direction);
	Direction = normalize_pre_length(Direction, MDistance) * MouseMax;
	Direction = vec2(static_cast<int>(Direction.x), static_cast<int>(Direction.y));

	return Direction;
}

int Hack::GetInput(int Dummy, int flag)
{
	if(!m_pClient)
		return -1;
	CNetObj_PlayerInput *m_Input;
	if(Dummy == g_Config.m_ClDummy)
		m_Input = &m_pClient->m_Controls.m_InputData[Dummy];
	else if(Dummy == !g_Config.m_ClDummy)
		m_Input = &m_pClient->m_DummyInput;
	else
	{
		dbg_msg("getinput", "Error: Local id not found!");
		return -1;
	}

	if(flag == INPUT_FIRE)
		return m_Input->m_Fire;
	else if(flag == INPUT_HOOK)
		return m_Input->m_Hook;
	else if(flag == INPUT_JUMP)
		return m_Input->m_Jump;
	else if(flag == INPUT_DIRECTION)
		return m_Input->m_Direction;
	else if(flag == INPUT_WEAPON)
		return m_Input->m_WantedWeapon;
}
vec2 Hack::GetInput(int Dummy, int flag, int flag2)
{
	if(!m_pClient)
		return vec2(0, 0);
	CNetObj_PlayerInput *m_Input;
	vec2 m_MousePos;
	if(Dummy == g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_Controls.m_InputData[Dummy];
		m_MousePos = m_pClient->m_Controls.m_MousePos[Dummy];
	}
	else if(Dummy == !g_Config.m_ClDummy)
	{
		m_Input = &m_pClient->m_DummyInput;
		m_MousePos = m_pClient->m_Controls.m_MousePos[Dummy];
	}
	else
	{
		dbg_msg("getinput", "Error: Local id not found!");
		return vec2(0, 0);
	}

	if(flag2 == AIM_TYPE_MOUSE)
	{
		if(flag == INPUT_AIM)
			return vec2(m_Input->m_TargetX, m_Input->m_TargetY);
	}
	else if(flag2 == AIM_TYPE_TARGET)
	{
		if(flag == INPUT_AIM)
			return m_MousePos;
	}
}

bool Hack::TicksPassed(int ticks)
{
	if(!m_pClient)
		return -1;
	return m_pClient->m_GameWorld.m_GameTick % ticks == 0;
}

bool Hack::IsGrounded(int ownId)
{
	if(!m_pClient)
		return -1;
	float m_ProximityRadius = 28;
	if(m_pClient->Collision()->CheckPoint(m_pClient->m_aClients[ownId].m_Predicted.m_Pos.x + m_ProximityRadius / 2, m_pClient->m_aClients[ownId].m_Predicted.m_Pos.y + m_ProximityRadius / 2 + 5))
		return true;
	if(m_pClient->Collision()->CheckPoint(m_pClient->m_aClients[ownId].m_Predicted.m_Pos.x - m_ProximityRadius / 2, m_pClient->m_aClients[ownId].m_Predicted.m_Pos.y + m_ProximityRadius / 2 + 5))
		return true;
	int MoveRestrictionsBelow = m_pClient->Collision()->GetMoveRestrictions(m_pClient->m_aClients[ownId].m_Predicted.m_Pos + vec2(0, m_ProximityRadius / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
	{
		return true;
	}

	return false;
}

bool Hack::IsFrozen(int ownId)
{
	if(!m_pClient)
		return -1;
	if(m_pClient->m_aClients[ownId].m_Snapped.m_Armor < 10)
		return true;
	return false;
}

int Hack::Jumps(int ownId)
{
	if(!m_pClient)
		return -1;
	return m_pClient->m_GameWorld.m_Core.m_apCharacters[ownId]->m_Jumps;
}

int Hack::Jumped(int ownId)
{
	if(!m_pClient)
		return -1;
	return m_pClient->m_GameWorld.m_Core.m_apCharacters[ownId]->m_Jumped;
}

// prediction
void Hack::Predict(CNetObj_Character *pCharacter, int t)
{
	CWorldCore tempworld;
	CCharacterCore tempcore;
	mem_zero(&tempcore, sizeof(tempcore));
	tempcore.Init(&tempworld, m_pClient->Collision());
	tempcore.Read(pCharacter);

	t = (t > MAX_PREDICTION_TICKS) ? MAX_PREDICTION_TICKS : t;

	for(int i = 0; i < t; i++)
	{
		tempcore.Tick(false);
		tempcore.Move();
		tempcore.Quantize();
		m_PredPositionArray[i] = tempcore.m_Pos;
	}

	tempcore.Write(pCharacter);
}

vec2 Hack::LegitScan(int ID)
{
	if(ID != -1)
	{
		CNetObj_Character CurChar = m_pClient->m_Snap.m_aCharacters[ID].m_Cur;
		Predict(&CurChar, MAX_PREDICTION_TICKS);
		for(int i = 0; i < MAX_PREDICTION_TICKS; i++)
		{
			// edge scan stuff
			vec2 localvel = m_pClient->m_PredictedChar.m_Vel / 256.0f;
			vec2 localpos = m_pClient->m_PredictedChar.m_Pos + localvel;
			vec2 EnemyPosChar = vec2(static_cast<int>(m_PredPositionArray[i].x), static_cast<int>(m_PredPositionArray[i].y));

			// IntersectLineTeleHook variables
			int TeleNr = 0;
			vec2 FinishPosChar = vec2(0, 0);

			float hookdragaccel = 3.8f;
			vec2 HookPosChar = (m_pClient->m_PredictedChar.m_Pos + localvel) + (EnemyPosChar - (m_pClient->m_PredictedChar.m_Pos + localvel)) * 28.0f * 1.5f;
			vec2 HookVelChar = (normalize(HookPosChar - (m_pClient->m_PredictedChar.m_Pos + localvel)) * hookdragaccel) / 256.0f;

			vec2 EnemyVelocityChar = vec2(CurChar.m_VelX, CurChar.m_VelY) / 256.0f;
			EnemyPosChar += (HookVelChar - EnemyVelocityChar) + (EnemyVelocityChar - localvel) + EnemyVelocityChar;
			localpos += normalize(EnemyPosChar - localpos) * 28.f;

			int HitChar = !m_pClient->Collision()->IntersectLineTeleHook(localpos, EnemyPosChar, &FinishPosChar, 0, &TeleNr);
			int HitCharacter = !m_pClient->IntersectCharacter(localpos, FinishPosChar, FinishPosChar, localid); 
			// check for charater intersection too
			if(HitChar && HitCharacter != -1)
				return FinishPosChar;
			else
				continue;
		}
		return vec2(0, 0);
	}
	return vec2(0, 0);
}
