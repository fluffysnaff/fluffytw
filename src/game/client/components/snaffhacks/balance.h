#ifndef GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFHACKS_BALANCE_H
#define GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFHACKS_BALANCE_H

#include "../snaffbase/snaffbase.h"

class HBalance : public SnaffHack
{
public:
	HBalance(CGameClient *pClient);
	~HBalance(){};

	vec2 GetClosestPlayer(int ownId);
	void Balance();

	void OnSnapInput();
};

#endif