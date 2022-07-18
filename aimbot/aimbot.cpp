#include "game/client/prediction/entities/character.h"
#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"


void FAimbot::Aimbot()
{
	if(!fConfig->aimbotCfg.enabled)
		return;

	// Get closest hook point into `m_TargetPos`
	GetClosestHitpoint();
	HookVisible(m_TargetPos);
	if(Controls()->m_aInputData[LOCAL].m_Hook == 1)
		Aim(NormalizeAim(m_TargetPos));
	else
		m_CanAim = true;
}

void FAimbot::HookVisible(vec2 targetPos)
{
	// Constant
	static bool hasHooked = false;

	// Reset input if needed and return
	if(!fConfig->aimbotCfg.hookVisible)
	{
		if(hasHooked)
		{
			Controls()->m_aLastData[LOCAL].m_Hook = 0;
			Controls()->m_aInputData[LOCAL].m_Hook = Controls()->m_aLastData[LOCAL].m_Hook;
			hasHooked = false;
		}
		return;
	}

	// If target is not visible return
	if(!m_TargetVisible)
		return;

	// Handle 'can aim' logic		
	m_CanAim = true;
	if(hasHooked == true)
		m_CanAim = false;

	// Aim and hook
	Aim(NormalizeAim(targetPos));
	Controls()->m_aInputData[LOCAL].m_Hook = 1;

	// Hooked, so update `hasHooked`
	hasHooked = true;
}


// Gets
void FAimbot::GetClosestHitpoint()
{
	// Get closest id in FOV
	m_TargetId = GetClosestId(fConfig->aimbotCfg.fov / (pi * 0.5f));
	// If ID is invalid return
	if(!fHelper->IsValidId(m_TargetId))
	{
		m_TargetPos = vec2(0.f, 0.f);
		m_TargetVel = vec2(0, 0);
		m_TargetVisible = false;
		m_TargetId = -1;
		return;
	}
	m_MyPos = m_pClient->m_PredictedChar.m_Pos;
	m_MyVel = m_pClient->m_PredictedChar.m_Vel;
	m_TargetPos = m_pClient->m_aClients[m_TargetId].m_Predicted.m_Pos;
	m_TargetVel = m_pClient->m_aClients[m_TargetId].m_Predicted.m_Vel;
	m_TargetPos = EdgeScan();
}

int FAimbot::GetClosestId(int fov, float range)
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

		if(!InFov(fov, Position - Pos))
			continue;
		if(ClosestID != -1 && GameWorld()->m_GameTick % 150 != 0)
			return ClosestID;
		static int lastHookedId = m_pClient->m_Snap.m_pLocalCharacter->m_HookedPlayer;
		if(fHelper->IsValidId(lastHookedId) &&
		   length(m_pClient->m_aClients[lastHookedId].m_Predicted.m_Pos - cData.m_Predicted.m_Pos) < Tuning()->m_HookLength + PHYS_SIZE * 0.5f)
			return ClosestID;
		if(ClosestID == -1 && distance(Pos, Position) < Distance)
		{
			ClosestID = i;
			Distance = distance(Pos, Position);
		}
	}
	return ClosestID;
}

float FAimbot::GetPing() const
{
	const auto realPing = static_cast<float>(Client()->GetPredictionTime());
	const float ping = realPing / 100.f;
	return ping;
}


// Helpers
bool FAimbot::PredictHook(vec2 &myPos, vec2 myVel, vec2 &targetPos, vec2 targetVel)
{
	const vec2 delta = targetPos - myPos;
	const vec2 deltaVel = targetVel - myVel;

	const float hookSpeed = length(targetVel) + Tuning()->m_HookFireSpeed;
	const float a = dot(deltaVel, deltaVel) - powf(hookSpeed, 2);
	const float b = 2.f * dot(deltaVel, delta);
	const float c = dot(delta, delta);

	const float sol = powf(b, 2) - 4.f * a * c;
	if(sol > 0.f)
	{
		// qTime is the same as time
		// const float qTime = (-sqrt(sol) - b) / (2 * a);
		const float time = abs(2.f * c / (sqrtf(sol) - b)) + GetPing();
		targetPos += targetVel * time;
		return true;
	}
	return false;
}

bool FAimbot::HitScanHook(vec2 initPos, vec2 targetPos, vec2 scanDir)
{
	vec2 exDirection = normalize(scanDir);
	vec2 finishPos = initPos + exDirection * (Tuning()->m_HookLength - PHYS_SIZE * 1.5f);

	vec2 oldPos = initPos + exDirection * PHYS_SIZE * 1.5f;
	vec2 newPos = oldPos;

	bool DoBreak = false;

	do
	{
		oldPos = newPos;
		newPos = oldPos + exDirection * Tuning()->m_HookFireSpeed;

		if(distance(initPos, newPos) > Tuning()->m_HookLength)
		{
			newPos = initPos + normalize(newPos - initPos) * Tuning()->m_HookLength;
			DoBreak = true;
		}

		int TeleNr = 0;
		const int Hit = Collision()->IntersectLineTeleHook(oldPos, newPos, &finishPos, nullptr, &TeleNr);

		if(IntersectCharacter(oldPos, targetPos, finishPos))
			return true;

		if(Hit)
			return false;

		newPos.x = round_to_int(newPos.x);
		newPos.y = round_to_int(newPos.y);

		if(oldPos == newPos)
			break;

		exDirection.x = round_to_int(exDirection.x * 256.0f) / 256.0f;
		exDirection.y = round_to_int(exDirection.y * 256.0f) / 256.0f;
	} while(!DoBreak);
	return false;
}

bool FAimbot::IntersectCharacter(vec2 hookPos, vec2 targetPos, vec2 &newPos)
{
	vec2 closestPoint;
	if(closest_point_on_line(hookPos, newPos, targetPos, closestPoint))
	{
		if(distance(targetPos, closestPoint) < PHYS_SIZE - 1.f)
		{
			newPos = closestPoint;
			return true;
		}
	}
	return false;
}


// Aim
vec2 FAimbot::NormalizeAim(vec2 Pos)
{
	constexpr float CameraMaxDistance = 200.f;
	const float FollowFactor = (g_Config.m_ClDyncam ? g_Config.m_ClDyncamFollowFactor : g_Config.m_ClMouseFollowfactor) / 100.f;
	const float DeadZone = g_Config.m_ClDyncam ? g_Config.m_ClDyncamDeadzone : g_Config.m_ClMouseDeadzone;
	const float MaxDistance = g_Config.m_ClMouseMaxDistance;
	const float MouseMax = minimum((FollowFactor != 0.f ? CameraMaxDistance / FollowFactor + DeadZone : MaxDistance), MaxDistance);
	const float MDistance = length(Pos);
	Pos = normalize_pre_length(Pos, MDistance) * MouseMax;
	Pos = vec2(static_cast<int>(Pos.x), static_cast<int>(Pos.y));
	return Pos;
}

void FAimbot::Aim(vec2 Pos)
{
	if(!m_CanAim)
		return;

	// Aimbot will aim -> update `m_CanAim`
	m_CanAim = false;

	if(!m_TargetVisible)
		return;

	// Aim using desired way
	if(!fConfig->aimbotCfg.silent)
	{
		Controls()->m_aMousePos[LOCAL] = Pos;
		Controls()->m_aInputData[LOCAL].m_TargetX = static_cast<int>(Controls()->m_aMousePos[LOCAL].x);
		Controls()->m_aInputData[LOCAL].m_TargetY = static_cast<int>(Controls()->m_aMousePos[LOCAL].y);
	}
	else
	{
		Controls()->m_aInputData[LOCAL].m_TargetX = static_cast<int>(Pos.x);
		Controls()->m_aInputData[LOCAL].m_TargetY = static_cast<int>(Pos.y);
	}
}


// Check
bool FAimbot::InFov(float fov, vec2 dir)
{
	const float angle_difference = pi - abs(abs(angle(dir) - angle(Controls()->m_aMousePos[LOCAL])) - pi);
	if(angle_difference > fov * (pi / 180))
		return false;
	return true;
}
