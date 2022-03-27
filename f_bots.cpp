#include "f_helper.h"
#include "f_bots.h"

void FBots::Run()
{
	if(!fHelper->IsLocalActive())
		return;
	fHelper->m_pAimbot->Aimbot();
}
