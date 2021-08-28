#ifndef GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFBASE_H
#define GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFBASE_H

#include "game/client/gameclient.h"

class CGameClient;

class SnaffHack
{
public:
	SnaffHack(CGameClient *pClient)
	{
		m_pClient = pClient;
	}
	virtual ~SnaffHack(){};

	virtual void OnInit(){};
	virtual void OnRender(){};
	virtual void OnSnapInput(){};
	virtual void OnConnected(){};
	virtual void OnUpdate(){};
	virtual void OnDummyDisconnect(){};
	virtual void OnRelease(){};
	virtual void OnConsoleInit(){};
	virtual void OnStateChange(int NewState, int OldState){};
	virtual void OnMessage(int MsgId, CUnpacker *pUnpacker, bool IsDummy = 0){};
	virtual void InvalidateSnapshot(){};
	virtual void OnNewSnapshot(){};
	virtual void OnPredict(){};
	virtual void OnActivateEditor(){};
	virtual void OnDummySwap(){};
	virtual void OnShutdown(){};
	virtual void OnEnterGame(){};
	virtual void OnRconType(bool UsernameReq){};
	virtual void OnRconLine(const char *pLine){};
	virtual void OnGameOver(){};
	virtual void OnStartGame(){};
	virtual void OnFlagGrab(int TeamID){};

private:
	CGameClient *m_pClient;

protected:
	CGameClient *GameClient() const { return m_pClient; }

	#define TYPE_TARGET 0
	#define TYPE_MOUSE 1
	// base input
	void BotMove(int direction, int dummy);
	void BotJump(int stroke, int dummy);
	void BotHook(int stroke, int dummy);
	void BotFire(int stroke, int dummy);

	void BotAim(vec2 pos, int dummy, int type);
	void BotAim(float posx, float posy, int dummy, int type);

	void BotSetWeapon(int weapon, int dummy);
	void BotDie(int dummy);

	// base get/checks
	bool TicksPassed(int ticks);
	bool IsGrounded(int dummy);
	bool IsFrozen(int dummy);
	int Jumps(int dummy);
	int Jumped(int dummy);

};

#endif