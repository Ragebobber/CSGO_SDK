#pragma once
#include "Cheat.h"

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))
#define MakePtr (cast, ptr, addValue) ( cast )( (DWORD)( ptr ) + ( DWORD )( addValue ) )

namespace U {
	/*Void*/
	extern void init();
	extern void start_csgo_interface();
	extern void start_hooks();
	extern void uninstall_hooks();
	extern void start_ofsets();
	extern void SetupOffsets();
	extern DWORD FindPattern(std::string moduleName, std::string pattern);
	extern CNetVarManager* NetVars;
	/*perm*/
	template <typename T>
	T GetVFunc(void* vTable, int iIndex) {
		return (*(T**)vTable)[iIndex];
	}
	template <typename T>
	T* CaptureInterface(std::string strModule, std::string strInterface)
	{
		typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(strModule.c_str()), ("CreateInterface"));

		return CreateInterface(strInterface.c_str(), 0);
	}

	extern float RandomFloat(const float min, const float max);
	extern bool LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
	extern void RandomSeed(UINT seed);
}