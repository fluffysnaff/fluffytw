#include "game/gamecore.h"
#include "game/client/prediction/entities/character.h"
#include "game/collision.h"
#include "f_helper.h"

#include "game/mapitems.h"

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

void FHelper::TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray)
{
	CWorldCore tmpWorld;
	auto tmpCore = CCharacterCore();
	auto tmpTeams = CTeamsCore();
	tmpCore.Init(&tmpWorld, Collision(), &tmpTeams);
	tmpCore.Read(pCharacter);

	for(int i = 0; i < t; i++)
	{
		tmpCore.Tick(false);
		tmpCore.Move();
		m_pPosArray[i] = tmpCore.m_Pos + tmpCore.m_Vel;
	}
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
		if(ClosestID != -1 && GameWorld()->m_GameTick % 150 != 0)
			return ClosestID;
		static int lastHookedId = m_pClient->m_Snap.m_pLocalCharacter->m_HookedPlayer;
		if(IsValidId(lastHookedId) && length(m_pClient->m_aClients[lastHookedId].m_Predicted.m_Pos - cData.m_Predicted.m_Pos) < Tuning()->m_HookLength + PHYS_SIZE * 0.5f)
			return ClosestID;
		if(ClosestID == -1 && distance(Pos, Position) < Distance)
		{
			ClosestID = i;
			Distance = distance(Pos, Position);
		}
	}
	return ClosestID;
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
