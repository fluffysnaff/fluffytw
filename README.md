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
#include "fluffytw/f_helper.h"
std::unique_ptr<FHelper> fHelper;
```

Now add this code to `void CGameClient::OnConsoleInit()`:
```cpp
void CGameClient::OnConsoleInit()
{
	// ...
	FHelper = std::make_unique<FHelper>(this);
}
```

## Usage
1. Visuals  
	Inside `players.cpp` include:
	```cpp
	#include "game/client/fluffytw/f_helper.h"
	```
	
	Then in `RenderPlayer()` add:
	```cpp
	// ...
	FConfig::EspConfig espCfg = { 0 }; // replace 0 with your g_Config variables
	fHelper->m_pVisuals->Run(ClientID, Angle, Position, espCfg);
	// ...
	```

2. Helper  
	Make this public:
	```cpp
		// inside collision.h
		class CTile *m_pTiles;
		int m_Width;
		int m_Height;
		class CLayers *m_pLayers;
	```

3. Bots  
	Inside `controls.cpp` include:
	```cpp
	#include "game/client/fluffytw/f_helper.h"
	```
	Then in `SnapInput();` add:
	```cpp
	// ...
	if(!m_InputDirectionLeft[g_Config.m_ClDummy] && m_InputDirectionRight[g_Config.m_ClDummy])
		m_InputData[g_Config.m_ClDummy].m_Direction = 1;

	FConfig::AimbotConfig cfg = { 0, 0, 0, 0}; // replace 0s with your g_Config variables
	fHelper->m_pBots->Run(cfg);
	// ...
	```
