#include "SDK.h"

const char* IVPanel::GetName(int iIndex)
{
	typedef const char* (__thiscall * OriginalFn)(void*, int);
	return U::GetVFunc<OriginalFn>(this, 36)(this, iIndex);
}

void IVPanel::SetMouseInputEnabled(unsigned int panel, bool state)
{
	return U::GetVFunc<void(__thiscall*) (PVOID, int, bool)>(this, 32) (this, panel, state);
}