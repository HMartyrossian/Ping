// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"

#include <stdio.h>

#include "UnityPingAPIImpl.h"

void ProcessAttach() {
}

void ThreadAttach() {
}

void ThreadDetach() {
}

void ProcessDetach() {
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			ProcessAttach();
			break;
		case DLL_THREAD_ATTACH:
			ThreadAttach();
			break;
		case DLL_THREAD_DETACH:
			ThreadDetach();
			break;
		case DLL_PROCESS_DETACH:
			ProcessDetach();
			break;
		default:
			break;
	}

	return TRUE;
}

extern "C" {
	__declspec(dllexport) bool PingImpl(const char* str) {
		return CUnityPingAPIImpl::Ping(str);
	}
}

extern "C" {
	__declspec(dllexport) bool SuccessImpl(const char* str) {
		return CUnityPingAPIImpl::Success(str);
	}
}

extern "C" {
	__declspec(dllexport) long long TimeImpl(const char* str) {
		return CUnityPingAPIImpl::Time(str);
	}
}
