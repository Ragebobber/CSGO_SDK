#include "SDK.h"

HWND	G::Window;
bool	G::PressedKeys[256] = {};
bool	G::d3dinit;
CBaseEntity* G::LocalPlayer;
ImDrawList* G::draw_list;
CUserCmd* G::UserCmd;
bool	G::SendPacket = true;
int		G::endIdx;

bool   G::has_offset = false;
DWORD  G::offset;


DWORD* H::D3D9;
DWORD* H::ClientMode;
DWORD* H::Surface;
DWORD* H::VPanel;

CVisuals* E::Visuals = new CVisuals();
CTriggerbot* E::Triggerbot = new CTriggerbot();
CLegitBot* E::LegitBot = new CLegitBot();
CAutoWall* E::AutoWall = new CAutoWall();
CPrediction* E::Prediction = new CPrediction();
CMisc* E::Misc = new CMisc();
CAntiAim* E::AntiAim = new CAntiAim();
CRageBot* E::RageBot = new CRageBot();