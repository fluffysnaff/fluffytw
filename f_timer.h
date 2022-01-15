#pragma once

#include <chrono>

#include "f_component.h"

class FTimer : FComponent
{
public:
	FTimer(CGameClient *pClient) :
		FComponent(pClient) {}

	int currentTime[6];

	void InitTimer(int configVar);
	void ResetTimer();
	void ShowTimer();
	bool TimedShutdown(int minutes);

private:
	std::chrono::high_resolution_clock::time_point startTime;
	bool hasInit = false;
};
