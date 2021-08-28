**Fluffy Teeworlds**  
------
This client uses a custom base class that you can use to make hacks. 

**What functions can you use**
------
You can use these hooks for executing your hacks:
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
  components/snaffhacks/hack.cpp
  components/snaffhacks/hack.h
  # cmakelists.txt
...
)
```

The hack.h file:
```cpp
#ifndef GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFHACKS_HACK_H
#define GAME_CLIENT_COMPONENTS_SNAFFBOTV2_SNAFFHACKS_HACK_H

#include "../snaffbase/snaffbase.h"

class HHack : public SnaffHack
{
public:
  HHack(CGameClient *pClient);
  ~HHack(){};
  
  void Hack();
  void OnSnapInput(); // virtual function
};

#endif
// hack.h file
```

The cpp file:
```cpp
#include "hack.h"

HHack::HHack(CGameClient *pClient) : SnaffHack(pClient)
{
  return;
}

void HHack::OnSnapInput()
{
  Hack();
}

void HHack::Hack()
{
  The hack...
}
// hack.cpp file
```

Then you need to reference this in gameclient.cpp
```cpp
#include "components/snaffhacks/hack.h"
...
void CGameClient::OnConsoleInit()
{
  ...
  m_Hacks.push_back(new HHack(this));
}
```

Now you have successfully made your hack.
