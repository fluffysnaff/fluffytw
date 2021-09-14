#ifndef GAME_CLIENT_COMPONENTS_HACKBASE_H
#define GAME_CLIENT_COMPONENTS_HACKBASE_H

#include "game/client/gameclient.h"

#define MAX_PREDICTION_TICKS 28

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

	// faster access to local ids
	#define local g_Config.m_ClDummy
	#define localid m_pClient->m_LocalIDs[local] // for checking with IDs
	
	#define dummy !g_Config.m_ClDummy
	#define dummyid m_pClient->m_LocalIDs[dummy]
	
	// aim flags
	#define AIM_TYPE_TARGET 0
	#define AIM_TYPE_MOUSE 1
	
	// getclosestplayer flags
	#define CLOSEST_TYPE_NORMAL 2
	#define CLOSEST_TYPE_AIMBOT 3
	
	// input flags
	#define INPUT_FIRE 4
	#define INPUT_HOOK 5
	#define INPUT_JUMP 6
	#define INPUT_DIRECTION 7
	#define INPUT_WEAPON 8
	#define INPUT_AIM 9
	#define INPUT_DIE 10

	// input
	void BotInput(int stroke, int Dummy, int flag);
	void BotInput(vec2 stroke, int Dummy, int flag);

	// gets
	vec2 GetClosestPlayer(int ownId, int range, int angle, int type);
	vec2 NormalizeAim(vec2 Pos, int Dummy);

	int GetInput(int Dummy, int flag);
	vec2 GetInput(int Dummy, int flag, int flag2);

	// prediction
	void Predict(CNetObj_Character *pCharacter, int t);
	vec2 m_PredPositionArray[MAX_PREDICTION_TICKS];

	bool TicksPassed(int ticks);
	bool IsGrounded(int ownId);
	bool IsFrozen(int ownId);
	int Jumps(int ownId);
	int Jumped(int ownId);

private:
	int lastID = -1;

	vec2 LegitScan(int ID);
};

#endif