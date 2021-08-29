#ifndef GAME_CLIENT_COMPONENTS_HACKS_AIMBOT_H
#define GAME_CLIENT_COMPONENTS_HACKS_AIMBOT_H

#include "../hackbase/hackbase.h"

class HAimbot : public Hack
{
public:
	HAimbot(CGameClient *pClient) : Hack(pClient){};
	~HAimbot(){};

	void Aimbot();
	void OnSnapInput();
};

#endif