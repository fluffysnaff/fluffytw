#include "f_helper.h"
#include "f_bots.h"

void FBots::Run(FConfig::AimbotConfig aimbotConfig)
{
	if(!fHelper->IsLocalActive())
		return;
	fHelper->m_pAimbot->Aimbot(&aimbotConfig);
}
