#ifdef _WIN32
#include <windows.h>
#pragma comment (lib, "winmm.lib")
#else
#include <chrono>
#include <thread>
#endif

#include "sleep_util.h"

SleepManager::SleepManager() {
#ifdef _WIN32
	TIMECAPS timecaps;
	timeGetDevCaps(&timecaps, sizeof(timecaps));
	timeBeginPeriod(timecaps.wPeriodMin);
#endif
}

SleepManager::~SleepManager() {
#ifdef _WIN32
	TIMECAPS timecaps;
	timeGetDevCaps(&timecaps, sizeof(timecaps));
	timeEndPeriod(timecaps.wPeriodMin);
#endif
}

void SleepManager::sleep(int ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}
