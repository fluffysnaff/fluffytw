# fluffytw
fluffytw is a base for Teeworlds hacking.

## Installation
Before you start, make sure to clone the DDNet repository.
```bash
# do this inside DDNet main directory
cd src/game/client
git submodule add https://github.com/fluffysnaff/fluffytw.git
```

Add this to `CMakeLists.txt`: 
```cmake
add_subdirectory(src/game/client/fluffytw)
```

It should be placed before:
```cmake
set(CLIENT_SRC ${ENGINE_CLIENT} ${PLATFORM_CLIENT} ${GAME_CLIENT} ${GAME_EDITOR} ${GAME_GENERATED_CLIENT})
```

## Initialization
Inside `gameclient.cpp` add this:
```cpp
#include <memory.h>
#include "fluffytw/f_config.h"
#include "fluffytw/f_helper.h"
std::unique_ptr<FHelper> fHelper;
std::unique_ptr<FConfig> fConfig;
```

Now add this code to `void CGameClient::OnConsoleInit()`:
```cpp
void CGameClient::OnConsoleInit()
{
	// ...
	fHelper = std::make_unique<FHelper>(this);
	fConfig = std::make_unique<FConfig>();
	// ...
}
```

## Usage
1. Visuals  
	Inside `players.cpp` include:
	```cpp
	#include "game/client/fluffytw/f_config.h"
	#include "game/client/fluffytw/f_helper.h"
	```
	
	Then in `RenderPlayer()` add:
	```cpp
	// ...
	fConfig->EspCfg.enabled = g_Config.m_ClEspEnable; // This is an example
	fHelper->m_pVisuals->Run(ClientID, Angle, Position); // Call this at the end
	```

2. Helper  
	Make this public:
	```cpp
		// inside collision.h
		// from: 
		class CTile *m_pTiles;
		int m_Width;
		int m_Height;
		class CLayers *m_pLayers;
		
	// to: 
	public:
		class CTile *m_pTiles;
		int m_Width;
		int m_Height;
		class CLayers *m_pLayers;
	```

3. Bots  
	Inside `controls.cpp` include:
	```cpp
	#include "game/client/fluffytw/f_helper.h"
	#include "game/client/fluffytw/f_config.h"
	```
	Then in `SnapInput();` add:
	```cpp
	// ...
	if(!m_InputDirectionLeft[g_Config.m_ClDummy] && m_InputDirectionRight[g_Config.m_ClDummy])
		m_InputData[g_Config.m_ClDummy].m_Direction = 1;
	fConfig->aimbotCfg.enabled = false; // Set what variables you need using fConfig
	fHelper->m_pBots->Run();
	// ...
	```
  
## Possible problems
1. If something isn't working, make sure that configs are setup right and everything is executing.  
2. Aimbot isn't working - Debug it. For example set `m_CanAim` always to true and see if it works.  
3. If there are problems with the execution of your build, it's most likely the aimbot thread.
  
## Showcase
[Hook prediction](https://streamable.com/j51ilg)  

Aimbot:  
1. [Hitpoint scan](https://streamable.com/s81xls)  
2. [Bruteforce scan](https://streamable.com/rq31xk)  
