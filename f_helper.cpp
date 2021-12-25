#include "f_helper.h"

FHelper::FHelper(CGameClient *client) :
	m_pClient(client)
{
	m_pBots = new FBots(m_pClient);
	m_pAimbot = new FAimbot(m_pClient);
	m_pVisuals = new FVisuals(m_pClient);
}

FHelper::~FHelper()
{
	delete m_pBots;
	delete m_pAimbot;
	delete m_pVisuals;
} 


bool FHelper::HitScanHook(vec2 InitPos, vec2 scanDir, float radius)
{
	vec2 ExDirection = normalize(scanDir);
	vec2 FinishPos = InitPos + ExDirection * (Tuning()->m_HookLength - 42.f);

	vec2 OldPos = InitPos + ExDirection * radius * 1.5f;
	vec2 NewPos = OldPos;

	bool DoBreak = false;

	do
	{
		OldPos = NewPos;
		NewPos = OldPos + ExDirection * Tuning()->m_HookFireSpeed;

		if(distance(InitPos, NewPos) > Tuning()->m_HookLength)
		{
			NewPos = InitPos + normalize(NewPos - InitPos) * Tuning()->m_HookLength;
			DoBreak = true;
		}

		int TeleNr = 0;
		const int Hit = Collision()->IntersectLineTeleHook(OldPos, NewPos, &FinishPos, nullptr, &TeleNr);

		if(m_pClient->IntersectCharacter(OldPos, FinishPos, FinishPos, LOCAL_ID) != -1)
			return true;

		if(Hit)
			break;

		NewPos.x = round_to_int(NewPos.x);
		NewPos.y = round_to_int(NewPos.y);

		if(OldPos == NewPos)
			break;

		ExDirection.x = round_to_int(ExDirection.x * 256.0f) / 256.0f;
		ExDirection.y = round_to_int(ExDirection.y * 256.0f) / 256.0f;
	} while(!DoBreak);
	return false;
}


int FHelper::GetClosestId(int fov)
{
	const vec2 Pos = m_pClient->m_PredictedChar.m_Pos;
	float Distance = Tuning()->m_HookLength + PHYS_SIZE;
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

		if(!m_pAimbot->InFov(fov, Position - OwnClientData.m_Predicted.m_Pos))
			continue;
		if(ClosestID == -1 || distance(Pos, Position) < Distance)
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
	const float ping = (realPing / 1000) + 1.f;
	return ping;
}

bool FHelper::PredictHook(vec2 myPos, vec2 myVel, vec2 &targetPos, vec2 targetVel)
{
	const vec2 targetDistance = targetPos - myPos;
	const vec2 velDiff = targetVel - myVel / length(myVel);

	const float time = (length(targetDistance) / Tuning()->m_HookFireSpeed) + fHelper->GetPing();
	const vec2 acc = velDiff / time;

	targetPos.x = 0.5f * acc.x * pow(time, 2) + velDiff.x * time + targetDistance.x;
	targetPos.y = 0.5f * acc.y * pow(time, 2) + velDiff.x * time + targetDistance.y;
	return true;
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
