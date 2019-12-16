#pragma once

#include <mutex>
#include <unordered_map>

#include "UnityPing.h"

class CUnityPingAPIImpl
{
public:
	static bool Ping(const char* destination);
	static bool Success(const char* destination);
	static long long Time(const char* destination);

private:
	static std::mutex destinationToPingMutex;
	static std::unordered_map<std::string, CUnityPing*> destinationToPing;
};
