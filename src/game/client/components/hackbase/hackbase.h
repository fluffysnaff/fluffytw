#ifndef GAME_CLIENT_COMPONENTS_HACKBASE_H
#define GAME_CLIENT_COMPONENTS_HACKBASE_H

#include "game/client/gameclient.h"

class CGameClient;

class Hack
{
public:
	Hack(CGameClient *pClient)
	{
		m_pClient = pClient;
	}
	virtual ~Hack(){};

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

protected:
	CGameClient *GameClient() const { return m_pClient; }
	CGameClient *m_pClient;

	#define AIM_TYPE_TARGET 0
	#define AIM_TYPE_MOUSE 1

	#define CLOSEST_TYPE_NORMAL 0
	#define CLOSEST_TYPE_AIMBOT 1

	// input
	void BotMove(int direction, int dummy);
	void BotJump(int stroke, int dummy);
	void BotHook(int stroke, int dummy);
	void BotFire(int stroke, int dummy);

	void BotAim(vec2 pos, int dummy, int type);
	void BotAim(float posx, float posy, int dummy, int type);

	void BotSetWeapon(int weapon, int dummy);
	void BotDie(int dummy);

	// gets
	vec2 GetClosestPlayer(int ownId, int range, int type);
	vec2 NormalizeAim(vec2 Pos, int dummy);

	bool TicksPassed(int ticks);
	bool IsGrounded(int dummy);
	bool IsFrozen(int dummy);
	int Jumps(int dummy);
	int Jumped(int dummy);
};

#endif