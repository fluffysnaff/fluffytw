#ifndef GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFHACKS_AIMBOT_H
#define GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFHACKS_AIMBOT_H

#include "../snaffbase/snaffbase.h"

class HAimbot : public SnaffHack
{
public:
	HAimbot(CGameClient *pClient);
	~HAimbot(){};

	vec2 GetClosestPlayer(int ownId);
	void Aimbot();

	void OnSnapInput();
};

#endif