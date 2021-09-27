**Fluffy Teeworlds**  
------
Hack base for ddracenetwork.

**Disclaimer**
------
This is not for complete begginers!

**What functions can you use**
------
You can use these hooks:
```cpp
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
```

You can use these functions for making hacks:
```cpp
void BotMove(int direction, int dummy);
void BotJump(int stroke, int dummy);
void BotHook(int stroke, int dummy);
void BotFire(int stroke, int dummy);

void BotAim(vec2 pos, int dummy, int type);
void BotAim(float posx, float posy, int dummy, int type);
void BotSetWeapon(int weapon, int dummy);
void BotDie(int dummy);

vec2 GetClosestPlayer(int ownId, int range, int type);
vec2 NormalizeAim(vec2 Pos, int dummy);

bool TicksPassed(int ticks);
bool IsGrounded(int dummy);
bool IsFrozen(int dummy);
int Jumps(int dummy);
int Jumped(int dummy);
```

**How to make a hack**
-------
First you need to create cpp and h files and reference them in CMakeLists.txt.

CMakeLists.txt file:
```
...
set_src(GAME_CLIENT GLOB_RECURSE src/game/client
  components/hacks/myhack.cpp
  components/hacks/myhack.h
  # cmakelists.txt
...
)
```

The hack.h file:
```cpp
#ifndef GAME_CLIENT_COMPONENTS_HACKS_MYHACK_H
#define GAME_CLIENT_COMPONENTS_HACKS_MYHACK_H

#include "../hackbase/hackbase.h"

class HMyhack : public Hack
{
public:
  HMyhack(CGameClient *pClient) : Hack(pClient){};
  ~HMyhack(){};
  
  void Hack();
  void OnSnapInput(); // virtual function
};

#endif
// myhack.h file
```

The cpp file:
```cpp
#include "myhack.h"

void HMyhack::OnSnapInput()
{
  Hack();
}

void HMyhack::Hack()
{
  The hack...
}
// myhack.cpp file
```

Then you need to reference this in gameclient.cpp
```cpp
#include "components/hacks/myhack.h"
...
void CGameClient::OnConsoleInit()
{
  ...
  m_Hacks.push_back(new HMyhack(this));
}
```

Now you have successfully made your hack.
