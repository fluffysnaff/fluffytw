#include "aimbot.h"
#include "game/client/fluffytw/f_helper.h"

void FAimbot::Aimbot()
{
	if(!fConfig->aimbotCfg.enabled)
		return;

	// Get closest hook point into `m_TargetPos`
	GetClosestHitpoint();

	// Validate hook point
	m_TargetVisible = m_TargetPos == vec2(0, 0) ? false : true;

	// Hook visible if it's enabled
	HookVisible(m_TargetPos);

	// Aim at the target
	if(Controls()->m_InputData[LOCAL].m_Hook == 1)
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
			Controls()->m_LastData[LOCAL].m_Hook = 0;
			Controls()->m_InputData[LOCAL].m_Hook = Controls()->m_LastData[LOCAL].m_Hook;
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
	Controls()->m_InputData[LOCAL].m_Hook = 1;

	// Hooked, so update `hasHooked`
	hasHooked = true;
}

// Gets

void FAimbot::GetClosestHitpoint()
{
	// Get closest id in FOV
	const int closestId = fHelper->GetClosestId(fConfig->aimbotCfg.fov / (pi * 0.5f));

	// If ID is invalid return
	if(!fHelper->IsValidId(closestId))
	{
		m_TargetPos = vec2(0.f, 0.f);
		return;
	}
	// ID is valid, edge scan
	m_TargetPos = EdgeScan(closestId);
}

// Aim
vec2 FAimbot::NormalizeAim(vec2 Pos)
{
    // This function returns a position that has the direction of hitpoint
	// and is clamped at `g_Config.m_ClMouseMaxDistance`
	const float CameraMaxDistance = 200.f;
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
    // Return if we already aimed or target is not visible
	if(!m_CanAim || !m_TargetVisible)
		return;

	// Aimbot will aim -> update `m_CanAim`
	m_CanAim = false;

	// Aim using desired way
	if(!fConfig->aimbotCfg.silent)
	{
		Controls()->m_MousePos[LOCAL] = Pos;
		Controls()->m_InputData[LOCAL].m_TargetX = static_cast<int>(Controls()->m_MousePos[LOCAL].x);
		Controls()->m_InputData[LOCAL].m_TargetY = static_cast<int>(Controls()->m_MousePos[LOCAL].y);
	}
	else
	{
		Controls()->m_InputData[LOCAL].m_TargetX = static_cast<int>(Pos.x);
		Controls()->m_InputData[LOCAL].m_TargetY = static_cast<int>(Pos.y);
	}
}

// Check
bool FAimbot::InFov(float fov, vec2 dir)
{
	const float angle_difference = pi - abs(abs(angle(dir) - angle(Controls()->m_MousePos[LOCAL])) - pi);
	if(angle_difference > fov * (pi / 180))
		return false;
	return true;
}
