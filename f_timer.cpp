#include "f_timer.h"

void FTimer::InitTimer(int configVar)
{
	if(!configVar || hasInit)
		return;
	hasInit = true;
	startTime = std::chrono::high_resolution_clock::now();
}

void FTimer::ResetTimer()
{
	hasInit = false;
	startTime = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 2; i++)
	{
		currentTime[i] = -1;
		currentTime[i + 3] = 1;
	}
}

void FTimer::ShowTimer()
{
	STextRenderColor TColor = { 1.f, 1.f, 1.f, 1.f };
	STextRenderColor TOutColor = { 0.f, 0.f, 0.f, 0.3f};

	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "%d", currentTime[1]);
	const float width = m_pClient->TextRender()->TextWidth(0, 5.f, aBuf, -1, -1.0f);

	const float m_Width = 300.0f * Graphics()->ScreenAspect();
	constexpr float m_Height = 300.0f;
	Graphics()->MapScreen(0.0f, 0.0f, m_Width, m_Height);

	CTextCursor Cursor;
	m_pClient->TextRender()->SetCursor(&Cursor, m_Width - m_Width / 2.f - width, 10.f, 10.f, TEXTFLAG_RENDER);
	Cursor.m_LineWidth = -1;
	static int textContainer = -1;
	if(textContainer == -1)
		textContainer = m_pClient->TextRender()->CreateTextContainer(&Cursor, "0");
	m_pClient->TextRender()->RecreateTextContainerSoft(&Cursor, textContainer, aBuf);
	m_pClient->TextRender()->RenderTextContainer(textContainer, &TColor, &TOutColor);
}

bool FTimer::TimedShutdown(int minutes)
{
	const auto nowTime = std::chrono::high_resolution_clock::now();
	const auto timeDiff = nowTime - startTime;
	currentTime[0] = std::chrono::duration_cast<std::chrono::hours>(timeDiff).count();
	currentTime[1] = std::chrono::duration_cast<std::chrono::minutes>(timeDiff).count();
	currentTime[2] = ceil(std::chrono::duration_cast<std::chrono::seconds>(timeDiff).count());
	if(std::chrono::duration_cast<std::chrono::minutes>(timeDiff).count() >= minutes)
	{
		Client()->Quit();
		return true;
	}
	return false;
}