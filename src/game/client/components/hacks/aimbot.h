#ifndef GAME_CLIENT_COMPONENTS_HACKS_AIMBOT_H
#define GAME_CLIENT_COMPONENTS_HACKS_AIMBOT_H

#include "../hackbase/hackbase.h"

class HAimbot : public Hack
{
public:
	HAimbot(CGameClient *pClient) : Hack(pClient){};
	void Aimbot();
	void OnSnapInput();

private:
	void AimbotVoid();
	void DoThread();
	static void AimbotThread(void *pUser);

	void *m_pAimbotThread = nullptr;
	bool started = false;

	vec2 AimbotResult = vec2(0, 0);
};

#endif