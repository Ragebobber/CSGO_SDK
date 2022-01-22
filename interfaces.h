#pragma once
#include "IBaseClientDLL.h"
#include "IClientEntList.h"
#include "IClientModeShared.h"
#include "IEngineClient.h"
#include "ISurface.h"
#include "IVPanel.h"
#include "IEngineTrace.h"
#include "IGlobalVarsBase.h"
#include "IVModelInfo.h"
#include "IInputSystem.h"
#include "IDebugOverlay.h"
#include "IPhysicsSurfaceProps.h"
#include "IGameMovement.h"
#include "IMoveHelper.h"
#include "IPrediction.h"
#include "IMDLCache.h"
#include "IConvar.h"
namespace I // Interfaces
{
	extern IBaseClientDll* Client;
	extern IClientEntityList* ClientEntList;
	extern IEngineClient* Engine;
	extern IClientModeShared* ClientMode;
	extern ISurface* Surface;
	extern IVPanel* VPanel;
	extern IEngineTrace* EngineTrace;;
	extern IGlobalVarsBase* Globals;
	extern IVModelInfo* ModelInfo;
	extern IInputSystem* InputSystem;
	extern IDebugOverlay* DebugOverlay;
	extern IPhysicsSurfaceProps* PhysicsSurfaceProps;
	extern IMoveHelper* MoveHelper;
	extern IPrediction* Prediction;
	extern IGameMovement* GameMovement;
	extern IMDLCache* MDLCache;
	extern IConVar* ConVar;
}