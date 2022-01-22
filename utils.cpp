#include "Cheat.h"
typedef void(*RandomSeed_t)(UINT);
typedef float(*RandomFloat_t)(float, float);
typedef bool(__cdecl* _LineGoesThroughSmoke) (Vector, Vector);
COffsets offsets;
CNetVarManager* U::NetVars = new CNetVarManager;

void U::init() {
	start_csgo_interface();
	SetupOffsets();
	start_hooks();
	Config->Setup();
}

void U::start_csgo_interface() {
	I::Client = U::CaptureInterface<IBaseClientDll>("client.dll", "VClient018");
	I::Engine = U::CaptureInterface<IEngineClient>("engine.dll", "VEngineClient014");
	I::ClientEntList = U::CaptureInterface<IClientEntityList>("client.dll", "VClientEntityList003");
	I::ClientMode = **(IClientModeShared***)((*(DWORD**)I::Client)[10] + 0x5);
	I::Prediction = U::CaptureInterface<IPrediction>(("client.dll"), ("VClientPrediction001"));
	I::MoveHelper = **(IMoveHelper***)(U::FindPattern(("client.dll"), ("8B 0D ? ? ? ? 8B 46 08 68")) + 0x2);
	I::GameMovement = U::CaptureInterface<IGameMovement>(("client.dll"), ("GameMovement001"));

	I::Surface = U::CaptureInterface<ISurface>(("vguimatsurface.dll"), ("VGUI_Surface031"));
	I::VPanel = U::CaptureInterface<IVPanel>(("vgui2.dll"), ("VGUI_Panel009"));
	I::EngineTrace = U::CaptureInterface<IEngineTrace>("engine.dll", ("EngineTraceClient004"));
	I::InputSystem = U::CaptureInterface<IInputSystem>(("inputsystem.dll"), ("InputSystemVersion001"));
	I::PhysicsSurfaceProps = U::CaptureInterface<IPhysicsSurfaceProps>(("vphysics.dll"), ("VPhysicsSurfaceProps001"));
	I::MDLCache = U::CaptureInterface<IMDLCache>(("datacache.dll"), ("MDLCache004"));
	I::ConVar = U::CaptureInterface<IConVar>(("vstdlib.dll"), ("VEngineCvar007"));

	I::Globals = **(IGlobalVarsBase***)((U::FindPattern("client.dll","A1 ? ? ? ? 5E 8B 40 10"))+1);

	I::ModelInfo = U::CaptureInterface<IVModelInfo>("engine.dll", ("VModelInfoClient004"));
	I::DebugOverlay = U::CaptureInterface<IDebugOverlay>("engine.dll", ("VDebugOverlay004"));
}
void U::start_hooks() {
    H::D3D9 = ((DWORD*)offsets.d3d9Device);
	H::ClientMode = ((DWORD*)I::ClientMode);
	H::Surface = ((DWORD*)I::Surface);
	H::VPanel = (DWORD*)I::VPanel;

    H::D3D9 = (DWORD*)H::D3D9[0];
	H::ClientMode = (DWORD*)H::ClientMode[0];
	H::Surface = (DWORD*)H::Surface[0];
	H::VPanel = (DWORD*)H::VPanel[0];

	oPaintTraverse = (PaintTraverseFn)((DWORD*)H::VPanel[41]);
    oReset = (ResetFn)((DWORD*)H::D3D9[16]);
    oEndScene = (EndSceneFn)((DWORD*)H::D3D9[42]);
	oCreateMove = (CreateMoveFn)((DWORD*)H::ClientMode[24]);
	oOverrideView = (OverrideViewFn)((DWORD*)H::ClientMode[18]);
	oGetViewModel = (GetViewModelFOVfn)((DWORD*)H::ClientMode[35]);
	o_LockCursor = (LockCursorFn)(DWORD*)H::Surface[67];

	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oEndScene, Hooks::EndScene);
	DetourAttach(&(PVOID&)oReset, Hooks::Reset);
	DetourAttach(&(PVOID&)oCreateMove, Hooks::CreateMove);
	DetourAttach(&(PVOID&)oOverrideView, Hooks::OverrideView);
	DetourAttach(&(PVOID&)oGetViewModel, Hooks::GetViewModelFOV);
	DetourAttach(&(PVOID&)oPaintTraverse, Hooks::PaintTraverse);
	DetourAttach(&(PVOID&)o_LockCursor, Hooks::LockCursor_h);
	DetourTransactionCommit();
	
}
void U::uninstall_hooks() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)oEndScene, Hooks::EndScene);
	DetourDetach(&(PVOID&)oReset, Hooks::Reset);
	DetourDetach(&(PVOID&)oCreateMove, Hooks::CreateMove);
	DetourDetach(&(PVOID&)oOverrideView, Hooks::OverrideView);
	DetourDetach(&(PVOID&)oGetViewModel, Hooks::GetViewModelFOV);
	DetourDetach(&(PVOID&)oPaintTraverse, Hooks::PaintTraverse);
	DetourDetach(&(PVOID&)o_LockCursor, Hooks::LockCursor_h);
	DetourTransactionCommit();
}

void U::start_ofsets() {
#pragma region DT_BasePlayer
	offsets.m_flFallVelocity = U::NetVars->GetOffset("DT_BasePlayer", "m_flFallVelocity");
	offsets.m_viewPunchAngle = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_viewPunchAngle"));
	offsets.m_aimPunchAngle = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_aimPunchAngle"));
	offsets.m_vecViewOffset = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_vecViewOffset[0]"));
	offsets.m_flFriction = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_flFriction"));
	offsets.m_nTickBase = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_nTickBase"));
	offsets.m_vecVelocity = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_vecVelocity[0]"));
	offsets.deadflag = U::NetVars->GetOffset(("DT_BasePlayer"), ("deadflag"));
	offsets.m_iHealth = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_iHealth"));
	offsets.m_lifeState = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_lifeState"));
	offsets.m_fFlags = 0x104;
	offsets.m_hObserverTarget = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_hObserverTarget"));
	offsets.m_ViewModel = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_hViewModel[0]"));
	offsets.m_szLastPlaceName = U::NetVars->GetOffset(("DT_BasePlayer"), ("m_szLastPlaceName"));
#pragma endregion

#pragma region DT_CSPlayer
	offsets.m_ArmorValue = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_ArmorValue"));
	offsets.m_bHasHelmet = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_bHasHelmet"));
	offsets.m_iClass = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_iClass"));
	offsets.m_bDormant = 0xED;
	offsets.m_bGunGameImmunity = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_bGunGameImmunity"));
	offsets.m_iCompetitiveRanking = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_iCompetitiveRanking"));
	offsets.m_iTeamNum = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_iTeamNum"));
	offsets.m_nSurvivalTeam = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_nSurvivalTeam"));
	offsets.m_angEyeAngles = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_angEyeAngles"));
	offsets.m_flFlashDuration = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_flFlashDuration"));
	offsets.m_bHasDefuser = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_bHasDefuser"));
	offsets.m_bIsScoped = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_bIsScoped"));
	offsets.m_angRotation = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_angEyeAngles[0]"));
	offsets.m_flLowerBodyYawTarget = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_flLowerBodyYawTarget"));
	offsets.m_iShotsFired = U::NetVars->GetOffset(("DT_CSPlayer"), ("m_iShotsFired"));
#pragma endregion

#pragma region DT_BaseCombatCharacter	
	offsets.m_hMyWeapons = U::NetVars->GetOffset(("DT_BaseCombatCharacter"), ("m_hMyWeapons"));
	offsets.m_hMyWearables = U::NetVars->GetOffset(("DT_BaseCombatCharacter"), ("m_hMyWearables"));
	offsets.m_hActiveWeapon = U::NetVars->GetOffset(("DT_BaseCombatCharacter"), ("m_hActiveWeapon"));
	offsets.m_flNextAttack = U::NetVars->GetOffset("DT_BaseCombatCharacter", "m_flNextAttack");

#pragma endregion

#pragma region DT_BaseAnimating
	offsets.m_nForceBone = U::NetVars->GetOffset(("DT_BaseAnimating"), ("m_nForceBone"));
	offsets.m_nSequence = U::NetVars->GetOffset(("DT_BaseAnimating"), ("m_nSequence"));
	offsets.m_mBoneMatrix = offsets.m_nForceBone + 0x1C;
	offsets.m_nHitboxSet = U::NetVars->GetOffset(("DT_BaseAnimating"), ("m_nHitboxSet"));
	offsets.m_flPoseParameter = U::NetVars->GetOffset(("DT_BaseAnimating"), ("m_flPoseParameter"));
	offsets.m_bClientSideAnimation = U::NetVars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
#pragma endregion

#pragma region DT_BaseEntity	
	offsets.m_flSimulationTime = U::NetVars->GetOffset(("DT_BaseEntity"), ("m_flSimulationTime"));
	offsets.m_vecOrigin = U::NetVars->GetOffset(("DT_BaseEntity"), ("m_vecOrigin"));
	offsets.m_vecAbsOrigin = U::NetVars->GetOffset(("DT_BaseEntity"), ("m_vecAbsOrigin"));
	offsets.m_Collision = U::NetVars->GetOffset(("DT_BaseEntity"), ("m_Collision"));
	offsets.m_CollisionGroup = U::NetVars->GetOffset(("DT_BaseEntity"), ("m_CollisionGroup"));
	offsets.m_hOwnerEntity = U::NetVars->GetOffset(("DT_BaseEntity"), ("m_hOwnerEntity"));
	offsets.m_fEffects = U::NetVars->GetOffset("DT_BaseEntity", "m_fEffects");
	offsets.m_vecAbsVelocity = U::NetVars->GetOffset("DT_BaseEntity", "m_vecAbsVelocity");
#pragma endregion
	
	
#pragma region DT_PlantedC4	
	offsets.m_bBombDefused = U::NetVars->GetOffset(("DT_PlantedC4"), ("m_bBombDefused"));
	offsets.m_flC4Blow = U::NetVars->GetOffset(("DT_PlantedC4"), ("m_flC4Blow"));
#pragma endregion
	
#pragma region DT_BaseViewModel
	offsets.m_nModelIndex = U::NetVars->GetOffset(("DT_BaseViewModel"), ("m_nModelIndex"));
	offsets.m_hOwner = U::NetVars->GetOffset(("DT_BaseViewModel"), ("m_hOwner"));
	offsets.m_hWeapon = U::NetVars->GetOffset(("DT_BaseViewModel"), ("m_hWeapon"));
	offsets.m_nViewModelIndex = U::NetVars->GetOffset(("DT_BaseViewModel"), ("m_nViewModelIndex"));

#pragma endregion

#pragma region DT_WeaponCSBase
	offsets.m_fAccuracyPenalty = U::NetVars->GetOffset(("DT_WeaponCSBase"), ("m_fAccuracyPenalty"));
	offsets.m_iWeaponID = U::NetVars->GetOffset(("DT_WeaponCSBase"), ("m_fAccuracyPenalty")) + 0x30;
	offsets.m_flPostponeFireReadyTime = U::NetVars->GetOffset(("DT_WeaponCSBase"), ("m_flPostponeFireReadyTime"));
	offsets.m_bReloadVisuallyComplete = U::NetVars->GetOffset(("DT_WeaponCSBase"), ("m_bReloadVisuallyComplete"));
#pragma endregion
	
#pragma region DT_BaseCombatWeapon	
	offsets.m_flNextPrimaryAttack = U::NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_flNextPrimaryAttack"));
	offsets.m_iWorldModelIndex = U::NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_iWorldModelIndex"));
	offsets.m_iClip1 = U::NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_iClip1"));
	offsets.m_iClip2 = U::NetVars->GetOffset(("DT_BaseCombatWeapon"), ("m_iPrimaryReserveAmmoCount"));
	offsets.m_flNextSecondaryAttack = U::NetVars->GetOffset("DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
#pragma endregion
	
#pragma region DT_BaseAttributableItem	
	offsets.m_nFallbackPaintKit = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_nFallbackPaintKit"));
	offsets.m_nFallbackSeed = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_nFallbackSeed"));
	offsets.m_flFallbackWear = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_flFallbackWear"));
	offsets.m_nFallbackStatTrak = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_nFallbackStatTrak"));
	offsets.m_Item = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_Item"));
	offsets.m_iItemIDHigh = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iItemIDHigh"));
	offsets.m_iItemIDLow = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iItemIDLow"));
	offsets.m_iAccountID = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iAccountID"));
	offsets.m_iEntityQuality = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iEntityQuality"));
	offsets.m_OriginalOwnerXuidLow = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_OriginalOwnerXuidLow"));
	offsets.m_OriginalOwnerXuidHigh = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_OriginalOwnerXuidHigh"));
	offsets.m_iItemDefinitionIndex = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iItemDefinitionIndex"));
	offsets.m_iEntityLevel = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_iEntityLevel"));
	offsets.m_AttributeManager = U::NetVars->GetOffset(("DT_BaseAttributableItem"), ("m_AttributeManager"));
	
#pragma endregion

#pragma region DT_WeaponCSBaseGun
	offsets.m_zoomLevel = U::NetVars->GetOffset(("DT_WeaponCSBaseGun"), ("m_zoomLevel"));
#pragma endregion

#pragma region DT_BaseCSGrenade
	offsets.m_bPinPulled = U::NetVars->GetOffset("CBaseCSGrenade", "m_bPinPulled");
	offsets.m_fThrowTime = U::NetVars->GetOffset("CBaseCSGrenade", "m_fThrowTime");
	offsets.m_flThrowStrength = U::NetVars->GetOffset("CBaseCSGrenade", "m_flThrowStrength");
#pragma endregion
	
#pragma region DT_BaseCSGrenadeProjectile
	offsets.m_nExplodeEffectTickBegin = U::NetVars->GetOffset("CBaseCSGrenadeProjectile", "m_nExplodeEffectTickBegin");
#pragma endregion	

	offsets.m_nMoveType = 0x25C;

    offsets.d3d9Device = **(DWORD**)(U::FindPattern(("shaderapidx9.dll"), ("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 0x1);
	offsets.SetAbsOrigin = U::FindPattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
	offsets.invalidateBoneCache = U::FindPattern(("client.dll"), "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	offsets.LineGoesThroughSmoke = U::FindPattern("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	offsets.m_PhysicsRunThink = U::FindPattern("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87");
	offsets.m_oPostThinkVPhysics = U::FindPattern("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB");
	offsets.m_oSimulatePlayerSimulatedEntities = U::FindPattern("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74");
	offsets.m_iPredictionRandomSeed = U::FindPattern("client.dll", ("8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04")) + 0x2;
	offsets.m_pSetPredictionEntity= U::FindPattern("client.dll", ("89 35 ? ? ? ? F3 0F 10 48 20")) + 0x2;
}
void U::SetupOffsets() {
	U::NetVars->Initialize();
	start_ofsets();
}
// learn_more
DWORD U::FindPattern(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

void U::RandomSeed(UINT seed) {
	static RandomSeed_t m_RandomSeed;
	if (!m_RandomSeed)
		m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");

	m_RandomSeed(seed);
}

float U::RandomFloat(const float min, const float max)
{
	static RandomFloat_t m_RandomFloat;
	if (!m_RandomFloat)
		m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");

	return m_RandomFloat(min, max);
}

bool U::LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos)
{
	static _LineGoesThroughSmoke LineGoesThroughSmokeFn = 0;

	if (!LineGoesThroughSmokeFn)
		LineGoesThroughSmokeFn = (_LineGoesThroughSmoke)offsets.LineGoesThroughSmoke;

	if (LineGoesThroughSmokeFn)
		return LineGoesThroughSmokeFn(vStartPos, vEndPos);
	return false;
}

