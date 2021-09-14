#ifndef GAME_CLIENT_COMPONENTS_HACKS_BALANCE_H
#define GAME_CLIENT_COMPONENTS_HACKS_BALANCE_H

#include "../hackbase/hackbase.h"

class HBalance : public Hack
{
public:
	HBalance(CGameClient *pClient) : Hack(pClient){};

	void Balance();
	void OnSnapInput();
};

#endif