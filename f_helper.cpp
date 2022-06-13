#include "game/gamecore.h"
#include "game/client/prediction//entities/character.h"
#include "f_helper.h"

FHelper::FHelper(CGameClient *client) :
	m_pClient(client),
	m_pBots(new FBots(client)),
	m_pAimbot(new FAimbot(client)),
	m_pVisuals(new FVisuals(client)) {}

FHelper::~FHelper()
{
	delete m_pBots;
	delete m_pAimbot;
	delete m_pVisuals;
}


bool FHelper::HitScanHook(vec2 initPos, vec2 targetPos, vec2 scanDir, float radius)
{
	vec2 ExDirection = normalize(scanDir);
	vec2 FinishPos = initPos + ExDirection * (Tuning()->m_HookLength - radius * 1.5f);

	vec2 OldPos = initPos + ExDirection * radius * 1.5f;
	vec2 NewPos = OldPos;

	bool DoBreak = false;

	do
	{
		OldPos = NewPos;
		NewPos = OldPos + ExDirection * Tuning()->m_HookFireSpeed;

		if(distance(initPos, NewPos) > Tuning()->m_HookLength)
		{
			NewPos = initPos + normalize(NewPos - initPos) * Tuning()->m_HookLength;
			DoBreak = true;
		}

		int TeleNr = 0;
		const int Hit = Collision()->IntersectLineTeleHook(OldPos, NewPos, &FinishPos, nullptr, &TeleNr);

		if(IntersectCharacter(OldPos, targetPos, FinishPos))
			return true;

		if(Hit)
			return false;

		NewPos.x = round_to_int(NewPos.x);
		NewPos.y = round_to_int(NewPos.y);

		if(OldPos == NewPos)
			break;

		ExDirection.x = round_to_int(ExDirection.x * 256.0f) / 256.0f;
		ExDirection.y = round_to_int(ExDirection.y * 256.0f) / 256.0f;
	} while(!DoBreak);
	return false;
}

bool FHelper::IntersectCharacter(vec2 hookPos, vec2 targetPos, vec2 &newPos)
{
	vec2 ClosestPoint;
	if(closest_point_on_line(hookPos, newPos, targetPos, ClosestPoint))
	{
		if(distance(targetPos, ClosestPoint) < PHYS_SIZE)
		{
			newPos = ClosestPoint;
			return true;
		}
	}
	return false;
}

bool FHelper::PredictHook(vec2 &myPos, vec2 myVel, vec2 &targetPos, vec2 targetVel, float speed)
{
	const vec2 delta = targetPos - myPos;
	const vec2 deltaVel = targetVel - myVel;

	const float hookSpeed = length(deltaVel) * 0.75f + speed;
	const float a = dot(deltaVel, deltaVel) - powf(hookSpeed, 2);
	const float b = 2.f * dot(deltaVel, delta);
	const float c = dot(delta, delta);
	
	const float sol = powf(b, 2) - 4.f * a * c;
	if(sol >= 0.f)
	{
		const float time = (2.f * c / (sqrtf(sol) - b)) + GetPing();
		targetPos += targetVel * time;
		return true;
	}
	return false;
}

void FHelper::PredictHookOut(vec2 &myPos, vec2 &myVel, vec2 &targetPos, vec2 &targetVel)
{
	const bool Grounded = IsGrounded(LOCAL_ID);
	const float MaxSpeed = Grounded ? Tuning()->m_GroundControlSpeed : Tuning()->m_AirControlSpeed;
	const float Accel = Grounded ? Tuning()->m_GroundControlAccel : Tuning()->m_AirControlAccel;
	const float Friction = Grounded ? Tuning()->m_GroundFriction : Tuning()->m_AirFriction;

	if(Controls()->m_InputData[LOCAL].m_Direction < 0)
		myVel.x = SaturatedAdd(-MaxSpeed, MaxSpeed, myVel.x, -Accel);
	if(Controls()->m_InputData[LOCAL].m_Direction > 0)
		myVel.x = SaturatedAdd(-MaxSpeed, MaxSpeed, myVel.x, Accel);
	if(Controls()->m_InputData[LOCAL].m_Direction == 0)
		myVel.x *= Friction;

	const vec2 targetDistance = targetPos - myPos;
	const float hookAccel = Tuning()->m_HookDragAccel * (length(targetDistance) / Tuning()->m_HookLength);
	const float dragSpeed = Tuning()->m_HookDragSpeed;
	if(m_pClient->m_aClients[LOCAL_ID].m_Snapped.m_HookedPlayer != -1)
	{
		if(length(targetDistance) > PHYS_SIZE * 1.5f)
		{
			const vec2 tmp = normalize(targetDistance);
			targetVel.x = SaturatedAdd(-dragSpeed, dragSpeed, targetVel.x, hookAccel * tmp.x * 1.5f);
			targetVel.y = SaturatedAdd(-dragSpeed, dragSpeed, targetVel.y, hookAccel * tmp.y * 1.5f);

			myVel.x = SaturatedAdd(-dragSpeed, dragSpeed, myVel.x, -hookAccel * tmp.x * 0.25f);
			myVel.y = SaturatedAdd(-dragSpeed, dragSpeed, myVel.y, -hookAccel * tmp.y * 0.25f);
		}
	}
	if(length(targetDistance) > 0)
	{
		const vec2 Dir = normalize(myPos - targetPos);

		if(length(targetDistance) < PHYS_SIZE * 1.25f && length(targetDistance) > 0.0f)
		{
			float a = (PHYS_SIZE * 1.45f - length(targetDistance));
			float Velocity = 0.5f;
			if(length(myVel) > 0.0001f)
				Velocity = 1 - (dot(normalize(myVel), Dir) + 1) / 2;

			myVel += Dir * a * (Velocity * 0.75f);
			myVel *= 0.85f;
		}
		if(m_pClient->m_aClients[LOCAL_ID].m_Snapped.m_HookedPlayer == -1 &&
			m_pClient->m_aClients[LOCAL_ID].m_Snapped.m_HookState != HOOK_IDLE)
		{
			vec2 HookVel = normalize(Controls()->m_MousePos[LOCAL] - myPos) * Tuning()->m_HookDragAccel;
			if(HookVel.y > 0)
				HookVel.y *= 0.3f;
			if((HookVel.x < 0 && Controls()->m_InputData[LOCAL].m_Direction < 0) || (HookVel.x > 0 && Controls()->m_InputData[LOCAL].m_Direction > 0))
				HookVel.x *= 0.95f;
			else
				HookVel.x *= 0.75f;

			const vec2 NewVel = myVel + HookVel;
			if(length(NewVel) < Tuning()->m_HookDragSpeed || length(NewVel) < length(myVel))
				myVel = NewVel;
		}
	}
	if(length(myVel) > 6000)
		myVel = normalize(myVel) * 6000;
}

void FHelper::TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray)
{
	CWorldCore tempworld;
	CCharacterCore tempcore;
	mem_zero(&tempcore, sizeof(tempcore));
	tempcore.Init(&tempworld, m_pClient->Collision());
	tempcore.ReadCharacter(pCharacter);

	for(int i = 0; i < t; i++)
	{
		tempcore.Tick(false);
		tempcore.Move();
		tempcore.Quantize();
		m_pPosArray[i] = tempcore.m_Pos;
	}
	tempcore.Write(pCharacter);
}


int FHelper::GetClosestId(int fov, float range)
{
	const vec2 Pos = m_pClient->m_PredictedChar.m_Pos;
	float Distance = range;
	int ClosestID = -1;

	const CGameClient::CClientData OwnClientData = m_pClient->m_aClients[LOCAL_ID];

	auto *player = dynamic_cast<CCharacter *>(m_pClient->m_GameWorld.FindFirst(m_pClient->m_GameWorld.ENTTYPE_CHARACTER));
	for(; player; player = dynamic_cast<CCharacter *>(player->TypeNext()))
	{
		int i = player->ID();
		if(i == LOCAL_ID || !player)
			continue;

		const CGameClient::CClientData cData = m_pClient->m_aClients[i];
		if(!cData.m_Active)
			continue;
		vec2 Position = m_pClient->m_aClients[i].m_Predicted.m_Pos;

		const bool IsOneSolo = cData.m_Solo || OwnClientData.m_Solo;
		const bool IsOneSpec = cData.m_Spec || OwnClientData.m_Spec;

		if(IsOneSpec || IsOneSolo)
			continue;

		if(!m_pClient->m_Teams.SameTeam(i, LOCAL_ID) || OwnClientData.m_NoHookHit)
			continue;

		if(!m_pAimbot->InFov(fov, Position - Pos))
			continue;
		if(ClosestID != -1 && GameWorld()->m_GameTick % 100 != 0)
			return ClosestID;
		if(ClosestID == -1 && distance(Pos, Position) < Distance)
		{
			ClosestID = i;
			Distance = distance(Pos, Position);
		}
	}
	return ClosestID;
}

float FHelper::GetPing() const
{
	const auto realPing = static_cast<float>(Client()->GetPredictionTime());
	const float ping = (realPing / 100.f);
	return ping;
}

int FHelper::GetCustomTile(float x, float y) const
{
	if(!m_pClient->Collision()->m_pTiles)
		return 0;

	int Nx = clamp(static_cast<int>(x / 32), 0, m_pClient->Collision()->m_Width - 1);
	int Ny = clamp(static_cast<int>(y / 32), 0, m_pClient->Collision()->m_Height - 1);
	int pos = Ny * m_pClient->Collision()->m_Width + Nx;

	return m_pClient->Collision()->m_pTiles[pos].m_Index;
}


bool FHelper::IsGrounded(int id, vec2 pos)
{
	if(!IsValidId(id) && pos == vec2(0, 0))
		return false;
	if(pos == vec2(0, 0))
		pos = m_pClient->m_aClients[id].m_Predicted.m_Pos;
	if(m_pClient->Collision()->CheckPoint(pos.x + PHYS_SIZE / 2, pos.y + PHYS_SIZE / 2 + 5))
		return true;
	if(m_pClient->Collision()->CheckPoint(pos.x - PHYS_SIZE / 2, pos.y + PHYS_SIZE / 2 + 5))
		return true;
	int MoveRestrictionsBelow = m_pClient->Collision()->GetMoveRestrictions(pos + vec2(0, PHYS_SIZE / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
		return true;
	return false;
}

bool FHelper::IsValidId(int id)
{
	if(id > -1 && id <= MAX_CLIENTS)
		return true;
	return false;
}

bool FHelper::IsLocalActive()
{
	if(!(Controls()->m_InputData[g_Config.m_ClDummy].m_PlayerFlags & PLAYERFLAG_PLAYING) || Client()->State() != IClient::STATE_ONLINE)
		return false;
	return true;
}
