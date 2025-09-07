# fluffytw

<p align="center">
  <img src="./images/logo.png" alt="Fluffytw Logo">
</p>

fluffytw is a base for Teeworlds hacking.

<div align="center">
  <!-- Primary Badges -->
  <a href="https://github.com/fluffysnaff/fluffytw"><img src="https://img.shields.io/badge/For-Teeworlds-98d243?style=for-the-badge" alt="For Teeworlds"></a>
  <a href="https://github.com/fluffysnaff/fluffytw"><img src="https://img.shields.io/badge/Type-Hacking%20Base-blueviolet?style=for-the-badge" alt="Type: Hacking Base"></a>
  <a href="https://github.com/fluffysnaff/fluffytw/commits/master"><img src="https://img.shields.io/github/last-commit/fluffysnaff/fluffytw?style=for-the-badge" alt="Last Commit"></a>
  <!-- Secondary / Tech Badges -->
  <br/>
  <img src="https://img.shields.io/badge/C++-00599C?style=flat-square&logo=c%2B%2B&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Compatible%20with-DDNet-lightgrey?style=flat-square" alt="Compatible with DDNet">
  <!-- Special Call-to-Action Badge -->
  <a href="https://krxteam.com/krx-client#pricing"><img src="https://img.shields.io/badge/Premium%20Version-KRX%20Client-gold?style=flat-square&logo=rocket" alt="KRX Client"></a>
</div>

---

## From Open-Source to Ultimate Power

Liking `fluffytw`? If you want to skip the setup and access the most powerful features, like advanced replay bots, ad-free experience, and priority support—all in a ready-to-go package, **KRX Client** is for you.

It's the ultimate, pre-compiled version of our work, built for players who want maximum performance with zero hassle.

➡️ **[Check out KRX Client's Premium Features](https://krxteam.com/krx-client#pricing)**

---

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
#include "fluffytw/f_helper.h"
std::unique_ptr<FHelper> fHelper;
```

Now add this code to `void CGameClient::OnConsoleInit()`:
```cpp
void CGameClient::OnConsoleInit()
{
	// ...
	fHelper = std::make_unique<FHelper>(this);
	// ...
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
	fHelper->m_pConfig_->espCfg.enabled = g_Config.m_ClEspEnable; // This is an example
	fHelper->m_pVisuals->Run(ClientID, Angle, Position); // Make sure to call this
	```

2. Helper  
	Add this in `CColision` class:
	```cpp
	// inside collision.h
	public:
	    // ...
	    CTile *GetTiles() const { return m_pTiles; }
	```

3. Bots  
	Inside `controls.cpp` include:
	```cpp
	#include "game/client/fluffytw/f_helper.h"
	```
	Then in `SnapInput();` add:
	```cpp
	// ...
	fHelper->m_pConfig->aimbotCfg.enabled = false; // Set what variables you need using fConfig
	fHelper->m_pBots->Run();
	// ...
	```
	It should be placed after:
	```cpp
	if(!m_aInputDirectionLeft[g_Config.m_ClDummy] && m_aInputDirectionRight[g_Config.m_ClDummy])
		m_aInputData[g_Config.m_ClDummy].m_Direction = 1;
	```   

  
## Possible problems
1. If something isn't working, make sure that configs are setup right and everything is executing.  
2. Aimbot isn't working - Debug it. For example set `m_CanAim` always to true and see if it works.  

## Showcase
1. [Hitpoint scan](https://streamable.com/s81xls)   
2. [Hook prediction](https://streamable.com/j51ilg)  & [Without hook prediction](https://streamable.com/4zegsy)

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=fluffysnaff/fluffytw&type=Date)](https://star-history.com/#fluffysnaff/fluffytw&Date)
