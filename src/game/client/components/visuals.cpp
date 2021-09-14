#include <game/client/gameclient.h>

#include "visuals.h"

void CVisuals::OnRender()
{
	// HUD
	UpdateComponents();
	RenderEnabledComponents();
	//if(Layers()->GameGroup())
	//{
	//	CanvasSetup(ScreenX0, ScreenX1, ScreenY0, ScreenY1);
		// u can use DrawCircle here for example
	//	CanvasEndSetup(ScreenX0, ScreenX1, ScreenY0, ScreenY1);
	//}
}

// base
void CVisuals::RenderEnabledComponents()
{
	if(m_pClient->m_Menus.IsActive())
		return;
	if(m_pClient->m_Voting.IsVoting())
		return;
	int offset = 0;
	for(auto &m_aEnabledComponent : m_aEnabledComponents)
	{
		if(m_aEnabledComponent.m_aName[0] == '\0')
			continue;
		float TwName = TextRender()->TextWidth(0, 10.0f, m_aEnabledComponent.m_aName, -1, -1);
		float TwNoteShort = 2.f;
		if(m_aEnabledComponent.m_aNoteShort[0])
			TwNoteShort += TextRender()->TextWidth(0, 10.0f, m_aEnabledComponent.m_aNoteShort, -1, -1);
		Graphics()->BlendNormal();
		Graphics()->TextureClear();
		Graphics()->QuadsBegin();
		Graphics()->SetColor(0, 0, 0, 0.5f);
		RenderTools()->DrawRoundRect(4.0f, 60.f + offset * 15, TwName + TwNoteShort, 14.0f, 3.0f);
		Graphics()->QuadsEnd();
		TextRender()->Text(0, 5.0f, 60.f + offset * 15, 10.0f, m_aEnabledComponent.m_aName, -1);
		TextRender()->Text(0, 5.0f + TwName + 2.f, 60.f + offset * 15, 10.0f, m_aEnabledComponent.m_aNoteShort, -1);
		TextRender()->Text(0, 5.0f, 60.f + offset * 15 + 10, 4.0f, m_aEnabledComponent.m_aNoteLong, -1);
		offset++;
	}
}

void CVisuals::EnableComponent(const char *pComponent, const char *pNoteShort, const char *pNoteLong)
{
	// update
	for(auto &Component : m_aEnabledComponents)
	{
		if(str_comp(Component.m_aName, pComponent))
			continue;
		str_copy(Component.m_aName, pComponent, sizeof(Component.m_aName));
		if(pNoteShort)
			str_copy(Component.m_aNoteShort, pNoteShort, sizeof(Component.m_aNoteShort));
		if(pNoteLong)
			str_copy(Component.m_aNoteLong, pNoteLong, sizeof(Component.m_aNoteLong));
		return;
	}
	// insert
	for(auto &Component : m_aEnabledComponents)
	{
		if(Component.m_aName[0] != '\0')
			continue;
		str_copy(Component.m_aName, pComponent, sizeof(Component.m_aName));
		Component.m_aNoteShort[0] = '\0';
		Component.m_aNoteLong[0] = '\0';
		if(pNoteShort)
			str_copy(Component.m_aNoteShort, pNoteShort, sizeof(Component.m_aNoteShort));
		if(pNoteLong)
			str_copy(Component.m_aNoteLong, pNoteLong, sizeof(Component.m_aNoteLong));
		return;
	}
}

void CVisuals::DisableComponent(const char *pComponent)
{
	// update
	for(auto &Component : m_aEnabledComponents)
	{
		if(str_comp(Component.m_aName, pComponent))
			continue;
		Component.m_aName[0] = '\0';
		return;
	}
}

bool CVisuals::SetComponentNoteShort(const char *pComponent, const char *pNote)
{
	if(!pNote)
		return false;
	for(auto &Component : m_aEnabledComponents)
	{
		if(str_comp(Component.m_aName, pComponent))
			continue;
		str_copy(Component.m_aNoteShort, pNote, sizeof(Component.m_aNoteShort));
		return true;
	}
	return false;
}

bool CVisuals::SetComponentNoteLong(const char *pComponent, const char *pNote)
{
	if(!pNote)
		return false;
	for(auto &Component : m_aEnabledComponents)
	{
		if(str_comp(Component.m_aName, pComponent))
			continue;
		str_copy(Component.m_aNoteLong, pNote, sizeof(Component.m_aNoteLong));
		return true;
	}
	return false;
}

void CVisuals::UpdateComponents()
{
	if(g_Config.m_ClAimbot)
		EnableComponent("aimbot");
	else
		DisableComponent("aimbot");
	if(m_pClient->m_Controls.balanceonoff == 1)
		EnableComponent("balance");
	else
		DisableComponent("balance");
}

void CVisuals::MapScreenToGroup(float CenterX, float CenterY, CMapItemGroup *pGroup, float Zoom)
{
	float Points[4];
	RenderTools()->MapscreenToWorld(CenterX, CenterY, pGroup->m_ParallaxX, pGroup->m_ParallaxY,
		pGroup->m_OffsetX, pGroup->m_OffsetY, Graphics()->ScreenAspect(), Zoom, Points);
	Graphics()->MapScreen(Points[0], Points[1], Points[2], Points[3]);
}

void CVisuals::CanvasSetup(float ScreenX0, float ScreenY0, float ScreenX1, float ScreenY1)
{
	Graphics()->GetScreen(&ScreenX0, &ScreenY0, &ScreenX1, &ScreenY1);
	MapScreenToGroup(m_pClient->m_Camera.m_Center.x, m_pClient->m_Camera.m_Center.y, Layers()->GameGroup(), m_pClient->m_Camera.m_Zoom);
	Graphics()->BlendNormal();
	Graphics()->TextureClear();
	Graphics()->QuadsBegin();
}

void CVisuals::CanvasEndSetup(float ScreenX0, float ScreenY0, float ScreenX1, float ScreenY1)
{
	Graphics()->QuadsEnd();
	Graphics()->MapScreen(ScreenX0, ScreenY0, ScreenX1, ScreenY1);
}

// drawing
void CVisuals::DrawCircle(float x, float y, float size)
{
	RenderTools()->DrawCircle(x, y, size, 64);
}
void CVisuals::DrawCircle(vec2 pos, float size)
{
	RenderTools()->DrawCircle(pos.x, pos.y, size, 64);
}

void CVisuals::DrawLine(vec2 point1, vec2 point2, float r, float g, float b)
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	Graphics()->SetColor(r, g, b, 1.f);
	IGraphics::CLineItem LineItem(point1.x, point1.y, point2.x, point2.y);
	Graphics()->LinesDraw(&LineItem, 1);
	Graphics()->LinesEnd();
}

void CVisuals::DrawBox(vec2 center, int size, float r, float g, float b)
{
	vec2 x, y, a, e;
	x = vec2(center.x - size, center.y - size);
	y = vec2(center.x + size, center.y - size);
	a = vec2(center.x + size, center.y + size);
	e = vec2(center.x - size, center.y + size);

	// from PwnClient
	DrawLine(x, y, r, g, b);
	DrawLine(y, a, r, g, b);
	DrawLine(a, e, r, g, b);
	DrawLine(e, x, r, g, b);
}

void CVisuals::SetColor(float r, float g, float b, float a)
{
	Graphics()->SetColor(r, g, b, a);
}
