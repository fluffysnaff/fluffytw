#ifndef GAME_CLIENT_COMPONENTS_VISUALS_H
#define GAME_CLIENT_COMPONENTS_VISUALS_H

#include <game/client/component.h>
#include <game/mapitems.h>

#define MAX_COMPONENT_LEN 16
#define MAX_COMPONENTS_ENABLED 8

class CVisuals : public CComponent
{
public:
	// base
	struct CUiComponent
	{
		char m_aName[MAX_COMPONENT_LEN];
		char m_aNoteShort[16];
		char m_aNoteLong[2048];
	};
	CUiComponent m_aEnabledComponents[MAX_COMPONENTS_ENABLED];

	void MapScreenToGroup(float CenterX, float CenterY, CMapItemGroup *pGroup, float Zoom);
	void RenderEnabledComponents();
	void EnableComponent(const char *pComponent, const char *pNoteShort = 0, const char *pNoteLong = 0);
	void DisableComponent(const char *pComponent);
	bool SetComponentNoteShort(const char *pComponent, const char *pNoteShort = 0);
	bool SetComponentNoteLong(const char *pComponent, const char *pNoteLong = 0);
	void UpdateComponents();

	float ScreenX0, ScreenY0, ScreenX1, ScreenY1;
	void CanvasSetup(float ScreenX0, float ScreenY0, float ScreenX1, float ScreenY1);
	void CanvasEndSetup(float ScreenX0, float ScreenY0, float ScreenX1, float ScreenY1);

	void DrawCircle(float x, float y, float size);
	void DrawCircle(vec2 pos, float size);

	void DrawLine(vec2 point1, vec2 point2, float r, float g, float b); // only use in players.cpp
	void DrawBox(vec2 center, int size, float r, float g, float b); // only use in players.cpp

	void SetColor(float r, float g, float b, float a);

	// visuals
	virtual void OnRender();
};

#endif
