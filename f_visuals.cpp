#include "f_helper.h"
#include "f_visuals.h"


void FVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!fHelper->m_pConfig->espCfg.enabled)
		return;
	Graphics()->TextureClear();
	DrawFov();
}


// Fov
void FVisuals::DrawFov()
{
	if(!fHelper->m_pConfig->espCfg.drawFov)
		return;
	DrawFovLine(fHelper->m_pConfig->aimbotCfg.fov * 0.01f);
	DrawFovLine(fHelper->m_pConfig->aimbotCfg.fov * -0.01f);
}

void FVisuals::DrawFovLine(float offset)
{
	const float Angle = angle(Controls()->m_aMousePos[g_Config.m_ClDummy]) + offset;
	const vec2 ExDirection = normalize(direction(Angle));

	const vec2 InitPos = m_pClient->m_LocalCharacterPos;
	vec2 FinishPos = InitPos + ExDirection * (Tuning()->m_HookLength);

	int teleNr = 0;
	Collision()->IntersectLineTeleHook(InitPos, FinishPos, &FinishPos, nullptr, &teleNr);

	const ColorRGBA HookCollColor(0.0f, 0.0f, 0.0f, 0.5f);
	DrawLine(InitPos, FinishPos, HookCollColor);
}
