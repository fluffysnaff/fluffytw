#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

// TODO: Proper weapon aimbot

void FAimbot::Aimbot(FConfig::AimbotConfig cfg)
{
	if(!cfg.enabled)
		return;
	m_StartTick = Client()->GameTick(LOCAL_ID);
	const vec2 EnemyPos = GetClosestHitpoint(cfg.fov / (pi * 0.5f));
	if(EnemyPos == vec2(0, 0))
		return;
	if(Controls()->m_InputData[LOCAL].m_Hook == 1 || cfg.hammer && Controls()->m_InputData[LOCAL].m_Fire & 1)
		Aim(NormalizeAim(EnemyPos), cfg.silent);
	else
		m_CanAim = true; // reset bool
}


// Gets

vec2 FAimbot::GetClosestHitpoint(int fov)
{
	const int closestId = fHelper->GetClosestId(fov);
	if(fHelper->IsValidId(closestId))
	{
		const vec2 hitpoint = HitpointScan(closestId);
		return hitpoint;
	}
	return vec2(0, 0);
}


// Aim Stuff

vec2 FAimbot::NormalizeAim(vec2 Pos)
{
	const float CameraMaxDistance = 200;
	const float FollowFactor = (g_Config.m_ClDyncam ? g_Config.m_ClDyncamFollowFactor : g_Config.m_ClMouseFollowfactor) / 100.0f;
	const float DeadZone = g_Config.m_ClDyncam ? g_Config.m_ClDyncamDeadzone : g_Config.m_ClMouseDeadzone;
	const float MaxDistance = g_Config.m_ClMouseMaxDistance;
	const float MouseMax = minimum((FollowFactor != 0.f ? CameraMaxDistance / FollowFactor + DeadZone : MaxDistance), MaxDistance);
	const float MDistance = length(Pos);
	Pos = normalize_pre_length(Pos, MDistance) * MouseMax;
	Pos = vec2(static_cast<int>(Pos.x), static_cast<int>(Pos.y));
	return Pos;
}

void FAimbot::Aim(vec2 Pos, int silent)
{
	if(!m_CanAim)
		return;
	Controls()->m_InputData[LOCAL].m_TargetX = static_cast<int>(Pos.x);
	Controls()->m_InputData[LOCAL].m_TargetY = static_cast<int>(Pos.y);
	if(!silent)
	{
		Controls()->m_MousePos[LOCAL].x = Controls()->m_InputData[LOCAL].m_TargetX;
		Controls()->m_MousePos[LOCAL].y = Controls()->m_InputData[LOCAL].m_TargetY;
	}
	if(Client()->GameTick(LOCAL_ID) - m_StartTick < SERVER_TICK_SPEED * 2)
		m_CanAim = false;
}


// Check

bool FAimbot::InFov(int fov, vec2 dir)
{
	const double angle_difference = pi - abs(abs(angle(dir) - angle(Controls()->m_MousePos[LOCAL])) - pi);
	if(angle_difference > fov * (pi / 180))
	{
		return false;
	}
	return true;
}
