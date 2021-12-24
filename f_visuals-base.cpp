#include "f_visuals.h"

void FVisuals::DrawLine(vec2 pos1, vec2 pos2, ColorRGBA color)
{
	Graphics()->LinesBegin();
	const IGraphics::CLineItem LineItem(pos1.x, pos1.y, pos2.x, pos2.y);
	Graphics()->SetColor(color);
	Graphics()->LinesDraw(&LineItem, 1);
	Graphics()->LinesEnd();
}

void FVisuals::DrawCircle(vec2 pos, float size, ColorRGBA color)
{
	const float step = 0.1f;
	const float firstX = cos(-step) * size + pos.x;
	const float firstY = sin(-step) * size + pos.y;

	vec2 prevPos = vec2(firstX, firstY);
	vec2 newPos = prevPos;

	for (float i = 0; i < 2 * pi; i += step)
	{
		const float x = cos(i) * size + pos.x;
		const float y = sin(i) * size + pos.y;

		prevPos = newPos;
		newPos = vec2(x, y);

		DrawLine(prevPos, newPos, color);
	}
}

void FVisuals::DrawBox(vec2 center, float size, ColorRGBA color)
{
	Graphics()->LinesBegin();
	const IGraphics::CLineItem array[4] =
	{
		IGraphics::CLineItem(center.x - size, center.y - size, center.x + size, center.y - size),
		IGraphics::CLineItem(center.x + size, center.y - size, center.x + size, center.y + size),
		IGraphics::CLineItem(center.x + size, center.y + size, center.x - size, center.y + size),
		IGraphics::CLineItem(center.x - size, center.y + size, center.x - size, center.y - size)
	};
	Graphics()->SetColor(color);
	Graphics()->LinesDraw(array, 4);
	Graphics()->LinesEnd();
}
