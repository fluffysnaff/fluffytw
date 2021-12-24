#pragma once

#include <game/client/gameclient.h>
#include "f_config.h"
#include "f_component.h"

class FBots : public FComponent
{
public:
	FBots(CGameClient *pClient) :
		FComponent(pClient){}
	void Run(FConfig::AimbotConfig aimbotConfig);
};
