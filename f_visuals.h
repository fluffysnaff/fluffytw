#pragma once

#include <game/client/gameclient.h>
#include "game/client/fluffytw/f_config.h"
#include "game/client/fluffytw/f_component.h"

class FVisuals : public FComponent
{
public:
	FVisuals(CGameClient *pClient) :
		FComponent(pClient){}

	void Run(int ClientID, float Angle, vec2 Position);

	// Draw
	void DrawLine(vec2 pos1, vec2 pos2, ColorRGBA color = {1, 1, 1, 1});
	void DrawCircle(vec2 pos, float size, ColorRGBA color = {1, 1, 1, 1});
	void DrawBox(vec2 center, float size, ColorRGBA color = {1, 1, 1, 1});

private:
	// Aimbot Fov
	void DrawFov();
	void DrawFovLine(float offset);
};