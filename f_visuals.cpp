#include "f_helper.h"
#include "f_visuals.h"


void FVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!fConfig->espCfg.enabled)
		return;
	Graphics()->TextureClear();
	DrawFov();
}


// Fov

void FVisuals::DrawFov()
{
	if(!fConfig->espCfg.drawFov)
		return;
	DrawFovLine(static_cast<float>(fConfig->aimbotCfg.fov * 0.01));
	DrawFovLine(static_cast<float>(fConfig->aimbotCfg.fov * -0.01));
}

void FVisuals::DrawFovLine(float offset)
{
	const float Angle = angle(Controls()->m_MousePos[g_Config.m_ClDummy]) + offset;
	const vec2 ExDirection = normalize(direction(Angle));

	const vec2 InitPos = m_pClient->m_LocalCharacterPos;
	vec2 FinishPos = InitPos + ExDirection * (Tuning()->m_HookLength);

	int teleNr = 0;
	Collision()->IntersectLineTeleHook(InitPos, FinishPos, &FinishPos, nullptr, &teleNr);

	const ColorRGBA HookCollColor(0.0f, 0.0f, 0.0f, 0.5f);
	DrawLine(InitPos, FinishPos, HookCollColor);
}
