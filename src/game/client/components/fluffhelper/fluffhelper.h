// Singleton Class

#ifndef GAME_CLIENT_COMPONENTS_FLUFFBOT_FLUFFHELPER_H
#define GAME_CLIENT_COMPONENTS_FLUFFBOT_FLUFFHELPER_H

#include "game/client/gameclient.h"

class Fluffhelper
{
public:
	static Fluffhelper &getInstance()
	{
		static Fluffhelper instance;
		return instance;
	}

private:
	Fluffhelper(){};

public:
	Fluffhelper(Fluffhelper const &) = delete;
	void operator=(Fluffhelper const &) = delete;

	// CGameClient pointers / get
	CGameClient *m_pClient;
	CGameClient *GameClient() const { return m_pClient; }

	int ActiveID = -1;

};

#endif