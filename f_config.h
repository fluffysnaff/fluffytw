#pragma once

class FConfig
{
public:
	struct AimbotCfg
	{
		bool enabled = false;
		float fov = 0.f;
		bool silent = false;
		bool hookVisible = false;
		bool edge = false;
		int scanType = 0;
		float accuracy = 0.f;
	} aimbotCfg;

	struct EspCfg
	{
		bool enabled = false;
		bool drawFov = false;
	} espCfg;
};

extern std::unique_ptr<FConfig> fConfig;
