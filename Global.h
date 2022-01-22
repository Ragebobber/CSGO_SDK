#pragma once
#include "Features.h"

namespace G
{
	extern HWND	Window;
	extern bool	PressedKeys[256];
	extern bool d3dinit;
	extern CBaseEntity* LocalPlayer;
	extern ImDrawList* draw_list;	
	extern bool has_offset;
	extern DWORD offset;
	extern CUserCmd* UserCmd;
	extern bool SendPacket;
	extern int endIdx;
}
namespace H {
	extern DWORD*	D3D9;
	extern DWORD* ClientMode;
	extern DWORD* Surface;
	extern DWORD* VPanel;
}

namespace E
{
	extern CVisuals* Visuals;
	extern CTriggerbot* Triggerbot;
	extern CLegitBot* LegitBot;
	extern CAutoWall* AutoWall;
	extern CPrediction* Prediction;
	extern CMisc* Misc;
	extern CAntiAim* AntiAim;
	extern CRageBot* RageBot;
}