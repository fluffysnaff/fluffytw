#pragma once

// TODO: Better configs

namespace FConfig
{
	struct EspConfig
	{
		bool fovEnabled;
		int aimbotFov;
	};

	struct AimbotConfig
	{
		bool enabled;
		bool silent;
		int fov;
		bool edge;
		bool accuracy;
		bool hookVisible;
		bool hook;
		bool hammer;
		bool grenade;
		bool laser;
		bool shotgun;
	};
}
