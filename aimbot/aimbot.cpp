#include "game/client/fluffytw/f_helper.h"
#include "aimbot.h"

void FAimbot::Aimbot()
{
	if(!fConfig->aimbotCfg.enabled)
		return;
	RunThread();
	m_TargetVisible = m_TargetPos == vec2(0, 0) ? false : true;
	HookVisible(m_TargetPos);
	if(Controls()->m_InputData[LOCAL].m_Hook == 0)
	{
		m_CanAim = true;
	}
	if(Controls()->m_InputData[LOCAL].m_Hook == 1)
	{
		Aim(NormalizeAim(m_TargetPos));
	}
}

void FAimbot::HookVisible(vec2 targetPos)
{
	static bool hasHooked = false;
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
	if(fConfig->aimbotCfg.hookVisible && m_TargetVisible)
	{
		if(hasHooked == true)
			m_CanAim = false;
		else
			m_CanAim = true;
		Aim(NormalizeAim(targetPos));
		Controls()->m_InputData[LOCAL].m_Hook = 1;
		hasHooked = true;
	}

}


// Gets

void FAimbot::GetClosestHitpoint()
{
	while(true)
	{
		const int closestId = fHelper->GetClosestId(fConfig->aimbotCfg.fov / (pi * 0.5f));
		if(fHelper->IsValidId(closestId))
			m_TargetPos = EdgeScan(closestId);
		else
			m_TargetPos = vec2(0, 0);
		thread_sleep(10);
	}
}

// Aim

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

void FAimbot::Aim(vec2 Pos)
{
	if(!m_CanAim)
		return;
	m_CanAim = false;
	if(!m_TargetVisible)
		return;
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


// Thread
void AimbotProxy(void *pUser)
{
	FAimbot *pSelf = (FAimbot *)pUser;
	pSelf->GetClosestHitpoint();
}

void FAimbot::RunThread()
{
	if(m_pAimbotThread == NULL)
		m_pAimbotThread = thread_init_and_detach(AimbotProxy, this, "Aimbot Thread");
}


// Check

bool FAimbot::InFov(float fov, vec2 dir)
{
	const float angle_difference = pi - abs(abs(angle(dir) - angle(Controls()->m_MousePos[LOCAL])) - pi);
	if(angle_difference > fov * (pi / 180))
		return false;
	return true;
}
