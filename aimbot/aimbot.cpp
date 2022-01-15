#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

void FAimbot::Aimbot(FConfig::AimbotConfig *cfg)
{
	if(!cfg->enabled)
		return;

	const vec2 EnemyPos = GetClosestHitpoint(cfg);
	m_TargetVisible = EnemyPos == vec2(0, 0) ? false : true;
	HookVisible(cfg->hookVisible, cfg->silent, EnemyPos);
	if(Controls()->m_InputData[LOCAL].m_Hook == 0)
	{
		m_CanAim = true;
	}
	if(m_TargetVisible && Controls()->m_InputData[LOCAL].m_Hook == 1)
	{
		Aim(NormalizeAim(EnemyPos), cfg->silent);
	}
}

void FAimbot::HookVisible(bool cfgVar, bool cfgSilent, vec2 targetPos)
{
	static bool hasHooked = false;
	if(!cfgVar)
	{
		if(hasHooked)
		{
			Controls()->m_LastData[LOCAL].m_Hook = 0;
			Controls()->m_InputData[LOCAL].m_Hook = Controls()->m_LastData[LOCAL].m_Hook;
			hasHooked = false;
		}
		return;
	}
	if(cfgVar && m_TargetVisible)
	{
		if(hasHooked == true)
			m_CanAim = false;
		else
			m_CanAim = true;
		Aim(NormalizeAim(targetPos), cfgSilent);
		Controls()->m_InputData[LOCAL].m_Hook = 1;
		hasHooked = true;
	}

}


// Gets

vec2 FAimbot::GetClosestHitpoint(FConfig::AimbotConfig *cfg)
{
	if(cfg == nullptr)
		return vec2(0, 0);
	const int closestId = fHelper->GetClosestId(cfg->fov / (pi * 0.5f));
	if(fHelper->IsValidId(closestId))
	{
		if(cfg->hook)
			return HitpointScan(closestId, cfg->edge, cfg->accuracy);
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
	m_CanAim = false;
	Controls()->m_InputData[LOCAL].m_TargetX = static_cast<int>(Pos.x);
	Controls()->m_InputData[LOCAL].m_TargetY = static_cast<int>(Pos.y);
	if(!silent)
	{
		Controls()->m_MousePos[LOCAL].x = Controls()->m_InputData[LOCAL].m_TargetX;
		Controls()->m_MousePos[LOCAL].y = Controls()->m_InputData[LOCAL].m_TargetY;
	}
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
