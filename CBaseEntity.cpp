#include "SDK.h"

IClientNetworkable* CBaseEntity::GetIClientNetworkable() {
	return (IClientNetworkable*)((DWORD)this + 0x8);
}
IClientUnknown* CBaseEntity::GetIClientUnknown() {
	return (IClientUnknown*)((DWORD)this + 0x0);
}
IClientRenderable* CBaseEntity::GetIClientRenderable() {
	return (IClientRenderable*)((DWORD)this + 0x4);
}

int	CBaseEntity::GetHealth()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_iHealth);
}

bool CBaseEntity::HasImmunity() {
	static auto m_bGunGameImmunity = offsets.m_bGunGameImmunity;
	return *(bool*)((DWORD)this + m_bGunGameImmunity);
}

bool CBaseEntity::IsScoped() {
	return *(bool*)((DWORD)this + offsets.m_bIsScoped);
}

int	CBaseEntity::GetArmor()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_ArmorValue);
}

bool CBaseEntity::GetDormant()
{
	if (!I::Engine->is_connected())
		return true;

	return *(bool*)((DWORD)this + offsets.m_bDormant);
}

HANDLE CBaseEntity::GetObserverTargetHandle()
{
	return *(HANDLE*)((DWORD)this + offsets.m_hObserverTarget);
}

ICollideable* CBaseEntity::GetCollideable()
{
	return (ICollideable*)((DWORD)this + offsets.m_Collision);
}

Vector CBaseEntity::GetOrigin()
{
	return *(Vector*)((DWORD)this + offsets.m_vecOrigin);
}

matrix3x4_t& CBaseEntity::coordinate_frame()
{
	const static auto m_CollisionGroup = offsets.m_CollisionGroup;
	auto m_rgflCoordinateFrame = m_CollisionGroup - 0x30;
	return *(matrix3x4_t*)((DWORD)this + m_rgflCoordinateFrame);
}

bool CBaseEntity::GetAlive()
{
	if (!I::Engine->is_connected())
		return false;
	if (this->GetHealth() <= 0)
		return false;
	if (this->GetLifeState() == false)
		return false;

	return true;
}
bool CBaseEntity::GetLifeState()
{
	if (!I::Engine->is_connected())
		return false;

	return (bool)(*(int*)((DWORD)this + offsets.m_lifeState) == 0);
}

player_info_t CBaseEntity::GetPlayerInfo()
{
	player_info_t pinfo;
	I::Engine->GetPlayerInfo(this->GetIClientNetworkable()->get_index(), &pinfo);
	return pinfo;
}
std::string	CBaseEntity::GetName()
{
	return this->GetPlayerInfo().name;
}
int& CBaseEntity::GetTakeDamage() {
	return *(int*)((DWORD)this + 0x280);
}

int CBaseEntity::GetTeam()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_iTeamNum);
}

bool CBaseEntity::IsEnemy()
{
	return (this->GetTeam() == G::LocalPlayer->GetTeam() || this->GetTeam() == 0) ? false : true;
}

bool CBaseEntity::isVisible()
{
	Vector vSrcOrigin = G::LocalPlayer->GetEyePosition();
	if (vSrcOrigin.IsZero())
		return false;
	BYTE bHitBoxCheckVisible[6] = {
			HITBOX_HEAD,
			HITBOX_CHEST,
			HITBOX_RIGHT_FOOT,
			HITBOX_LEFT_FOOT,
			HITBOX_RIGHT_HAND,
			HITBOX_LEFT_HAND,
	};
	CTraceFilter filter;
	filter.pSkip = G::LocalPlayer;
	for (int nHit = 0; nHit < 6; nHit++)
	{
		Vector vHitBox = this->GetHitboxPosition(bHitBoxCheckVisible[nHit]);

		if (vHitBox.IsZero())
			continue;

		trace_t tr;
		Ray_t ray;

		ray.Init(vSrcOrigin, vHitBox);

		I::EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		if (tr.m_pEnt == this && !tr.allsolid)
			return true;
	}

	return false;

}


bool CBaseEntity::SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	// Store Things
	auto Curtime = I::Globals->cur_time;
	auto FrameTime = I::Globals->frame_time;
	auto AbsoluteFrameTime = I::Globals->absolute_frame_start_time;
	auto NextSimulationTick = I::Globals->frame_count / I::Globals->interval_per_tick + 1;
	auto Frames = I::Globals->frame_count;
	auto Ticks = I::Globals->frame_count;

	// Change Things
	I::Globals->cur_time = this->GetSimulationTime();
	I::Globals->frame_time = I::Globals->interval_per_tick;
	I::Globals->absolute_frame_start_time = I::Globals->interval_per_tick;
	I::Globals->frame_count = (int)NextSimulationTick;
	I::Globals->tick_count = (int)NextSimulationTick;

	auto m_fBackupEffects = *(int*)((DWORD)this + offsets.m_fEffects);
	//m_unkBoneMask 
	static auto backup_value = *(int*)((DWORD)this + 0x274);

	*(int*)((DWORD)this + 0x274) = 0;

	*(int*)((DWORD)this + offsets.m_fEffects) |= 8;

	// Setup Bones
	this->InvalidateBoneCache();

	bool result = this->GetIClientRenderable()->setup_bones(pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	*(int*)((DWORD)this + offsets.m_fEffects) = m_fBackupEffects;

	*(int*)((DWORD)this + 0x274) = backup_value;

	I::Globals->cur_time = Curtime;
	I::Globals->frame_time = FrameTime;
	I::Globals->absolute_frame_start_time = AbsoluteFrameTime;
	I::Globals->frame_count = Frames;
	I::Globals->tick_count = Ticks;

	return result;
}
studiohdr_t* CBaseEntity::GetStudioModel()
{
	const model_t* model = nullptr;

	model = this->GetIClientRenderable()->get_model();

	if (!model)
		return nullptr;

	studiohdr_t* pStudioModel = I::ModelInfo->GetStudioModel(model);

	if (!pStudioModel)
		return nullptr;

	return pStudioModel;
}
mstudiohitboxset_t* CBaseEntity::GetHitBoxSet()
{
	studiohdr_t* pStudioModel = nullptr;
	mstudiohitboxset_t* pHitboxSet = nullptr;

	pStudioModel = this->GetStudioModel();

	if (!pStudioModel)
		return pHitboxSet;

	pHitboxSet = pStudioModel->get_hitbox_set(0);

	if (!pHitboxSet)
		return nullptr;

	return pHitboxSet;
}

mstudiobbox_t* CBaseEntity::GetHitBox(int nHitbox)
{
	if (nHitbox < 0 || nHitbox >= HITBOX_MAX)
		return nullptr;

	mstudiohitboxset_t* pHitboxSet = nullptr;
	mstudiobbox_t* pHitboxBox = nullptr;

	pHitboxSet = this->GetHitBoxSet();

	if (!pHitboxSet)
		return pHitboxBox;

	pHitboxBox = pHitboxSet->get_hitbox(nHitbox);

	if (!pHitboxBox)
		return nullptr;

	return pHitboxBox;
}

Vector CBaseEntity::GetHitboxPosition(int nHitbox)
{
	matrix3x4_t MatrixArray[MAXSTUDIOBONES];
	Vector vRet, vMin, vMax;

	vRet = Vector(0, 0, 0);

	mstudiobbox_t* pHitboxBox = this->GetHitBox(nHitbox);

	if (!pHitboxBox)
		return vRet;

	if (!this->SetupBones(MatrixArray, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Globals->cur_time))
		return vRet;

	if (!pHitboxBox->bone)
		return vRet;

	math::vector_transform(pHitboxBox->bbmin, MatrixArray[pHitboxBox->bone], vMin);
	math::vector_transform(pHitboxBox->bbmax, MatrixArray[pHitboxBox->bone], vMax);

	vRet = (vMin + vMax) * 0.5f;

	return vRet;
}

float CBaseEntity::HitChance(CBaseEntity* player, QAngle ang) {
	auto weapon = this->GetWeapon();

	if (!weapon || !weapon->IsGun())
		return 0.f;

	weapon->update_accuracy_penalty();

	Vector forward, right, up, dir;
	Vector eye = this->GetEyePosition();

	math::AngleVectors(ang, &forward, &right, &up);

	const float fl_weapon_spread = weapon->get_spread();
	const float fl_weapon_inaccuracy = weapon->get_inaccuracy();

	int seeds_hit = 0;

	for (int i = 0; i < 256; i++)
	{
		U::RandomSeed((i & 0xFFF) + 1);

		float a = U::RandomFloat(0.f, 1.f);
		float b = U::RandomFloat(0.f, 2.f * M_PI_F);
		float c = U::RandomFloat(0.f, 1.f);
		float d = U::RandomFloat(0.f, 2.f * M_PI_F);

		float inaccuracy = a * fl_weapon_inaccuracy;
		float spread = c * fl_weapon_spread;

		Vector v_spread((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0);
		dir = forward + (right * v_spread) + (up * v_spread);

		Vector TraceEnd = eye + (dir * this->GetWeapon()->GetCSWpnData()->flRange);

		trace_t tr;
		Ray_t ray;
		ray.Init(eye, TraceEnd);
		CTraceFilter filter;
		filter.pSkip = this;

		I::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		if (tr.m_pEnt == player)
			seeds_hit++;
	}
	return float(seeds_hit) / 256.f * 100.f;
}

float CBaseEntity::GetSimulationTime()
{
	return *(float*)((DWORD)this + offsets.m_flSimulationTime);
}
Vector CBaseEntity::GetBonePosition(int iBone)
{
	matrix3x4_t boneMatrixes[MAXSTUDIOBONES];
	if (this->SetupBones(boneMatrixes, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Globals->cur_time))
	{
		matrix3x4_t boneMatrix = boneMatrixes[iBone];
		return Vector(boneMatrix.m_flMatVal[0][3], boneMatrix.m_flMatVal[1][3], boneMatrix.m_flMatVal[2][3]);
	}
	return Vector(0, 0, 0);
}

Vector CBaseEntity::GetAbsOrigin() {

	return *(Vector*)((DWORD)this + offsets.m_vecAbsOrigin);
}
int CBaseEntity::m_fEffects() {
	return *(int*)((DWORD)this + offsets.m_fEffects);
}
void CBaseEntity::SetAbsOrigin(const Vector& origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector& origin);
	static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)offsets.SetAbsOrigin;

	SetAbsOrigin(this, origin);
}

void CBaseEntity::InvalidateBoneCache()
{
	static unsigned long g_iModelBoneCounter = (unsigned long)(offsets.invalidateBoneCache + 10);
	*(unsigned int*)((DWORD)this + 0x2928) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)this + 0x2690) = (g_iModelBoneCounter - 1);
	*(unsigned int*)((DWORD)this + 0x26AC) = 0;
}

Vector CBaseEntity::GetEyePosition()
{	
	Vector origin = this->GetOrigin();
	Vector offset = *(Vector*)((DWORD)this + offsets.m_vecViewOffset);

	return(origin + offset);
}

int CBaseEntity::GetHitboxSet()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_nHitboxSet);
}
int CBaseEntity::GetTickBase()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_nTickBase);
}

int CBaseEntity::SetTickBase(int tick)
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_nTickBase) = tick;
}
QAngle CBaseEntity::GetPunch()
{
	return *(QAngle*)((DWORD)this + offsets.m_aimPunchAngle);
}

CBaseCombatWeapon* CBaseEntity::GetWeapon()
{
	
	if (!I::Engine->is_connected())
		return nullptr;
	if (!this->GetAlive())
		return nullptr;

	HANDLE weaponData = *(HANDLE*)((DWORD)this + offsets.m_hActiveWeapon);
	return (CBaseCombatWeapon*)I::ClientEntList->GetClientEntityFromHandle(weaponData);
}

WeaponInfo_t* CBaseCombatWeapon::GetCSWpnData()
{
	using OriginalFn = WeaponInfo_t * (__thiscall*)(void*);
	return ((OriginalFn)U::GetVFunc<OriginalFn>(this, 461))(this);
}

bool CBaseEntity::IsPlayer() {
	typedef float(__thiscall* OriginalFn)(void*);
	return U::GetVFunc<OriginalFn>(this, 158)(this);
}
int CBaseEntity::GetShotsFired()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_iShotsFired);
}
bool CBaseEntity::HasHelmet() {
	return *(bool*)((DWORD)this + offsets.m_bHasHelmet);
}

float CBaseEntity::NextAttack() {
	return *(float*)((DWORD)this + offsets.m_flNextAttack);
}

void CBaseEntity::StudioFrameAdvance()
{
	typedef void(__thiscall* OriginalFn)(void*);
	U::GetVFunc<OriginalFn>(this, 220);
}
void  CBaseEntity::SetSequence(int iSequence)
{
	typedef void(__thiscall* OriginalFn)(void*,int);
	U::GetVFunc<OriginalFn>(this, 219)(this,iSequence);
}
float CBaseEntity::GetFallVelocity() {
	return *(float*)((DWORD)this + offsets.m_flFallVelocity);
}
float CBaseEntity::SetFallVelocity(float set) {
	return *(float*)((DWORD)this + offsets.m_flFallVelocity) = set;
}
void CBaseEntity::UpdateCollisionBounds()
{
	typedef void(__thiscall* OriginalFn)(void*);
	U::GetVFunc<OriginalFn>(this, 340);
}
int CBaseEntity::GetFlags()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_fFlags);
}
Vector CBaseEntity::GetVelocity()
{
	return *(Vector*)((DWORD)this + offsets.m_vecVelocity);
}

bool CBaseEntity::PhysicsRunThink(int unk01) {
	// @xref: from sub with "CLIENT:  %s(%s) thinking for %.02f ms!!!\n"
	using PhysicsRunThinkFn = bool(__thiscall*)(void*, int);
	static PhysicsRunThinkFn oPhysicsRunThink = (PhysicsRunThinkFn)(offsets.m_PhysicsRunThink);
	return oPhysicsRunThink(this, unk01);
}
void CBaseEntity::RunPreThink() {
	typedef void(__thiscall* OriginalFn)(void*);
	return U::GetVFunc<OriginalFn>(this, 318)(this);
}
void CBaseEntity::RunThink() {
	typedef void(__thiscall* OriginalFn)(void*);
	return U::GetVFunc<OriginalFn>(this, 138)(this);
}
void CBaseEntity::RunPostThink() {
	// @ida postthink: 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07
	using PostThinkVPhysicsFn = bool(__thiscall*)(CBaseEntity*);
	static PostThinkVPhysicsFn oPostThinkVPhysics = (PostThinkVPhysicsFn)(offsets.m_oPostThinkVPhysics);
	
	using SimulatePlayerSimulatedEntitiesFn = void(__thiscall*)(CBaseEntity*);
	static SimulatePlayerSimulatedEntitiesFn oSimulatePlayerSimulatedEntities = (SimulatePlayerSimulatedEntitiesFn)(offsets.m_oSimulatePlayerSimulatedEntities);

	I::MDLCache->BeginLock();
	
	if (this->GetAlive() || *(bool*)((DWORD)this + 0x9AE1))
	{
		this->UpdateCollisionBounds();

		if (this->GetFlags() & FL_ONGROUND)
		{
			this->SetFallVelocity(0.f);
		}

		if (*(int*)((DWORD)this + offsets.m_nSequence) == -1)
			this->SetSequence(0);

		this->StudioFrameAdvance();
		oPostThinkVPhysics(this);
	}

	oSimulatePlayerSimulatedEntities(this);

	I::MDLCache->EndLock();
	
}
int CBaseEntity::GetMoveType()
{
	if (!I::Engine->is_connected())
		return 0;

	return *(int*)((DWORD)this + offsets.m_nMoveType);
}

/// <summary>
/// weapon
/// </summary>
/// <returns></returns>
short& CBaseCombatWeapon::GetWeaponID()
{
	return *(short*)((DWORD)this + offsets.m_iItemDefinitionIndex);
}

void CBaseCombatWeapon::update_accuracy_penalty()
{
	using fn = void(__thiscall*)(void*);
	U::GetVFunc<fn>(this, 484)(this);
}
float CBaseCombatWeapon::get_spread()
{
	using fn = float(__thiscall*)(void*);
	return U::GetVFunc<fn>(this, 453)(this);
}
float CBaseCombatWeapon::get_inaccuracy()
{
	using fn = float(__thiscall*)(void*);
	return U::GetVFunc<fn>(this, 483)(this);
}
bool CBaseCombatWeapon::CanFire()
{
	static decltype(this) stored_weapon = nullptr;
	static auto stored_tick = 0;
	if (stored_weapon != this || stored_tick >= G::LocalPlayer->GetTickBase()) {
		stored_weapon = this;
		stored_tick = G::LocalPlayer->GetTickBase();
		return false; //cannot shoot first tick after switch
	}

	if (this->Clip1() <= 0 || !G::LocalPlayer || this->IsReloading())
		return false;

	auto flServerTime = G::LocalPlayer->GetTickBase() * I::Globals->interval_per_tick;

	return this->GetNextPrimaryAttack() <= flServerTime;
}


float& CBaseCombatWeapon::GetNextPrimaryAttack()
{
	return *(float*)((DWORD)this + offsets.m_flNextPrimaryAttack);
}

float& CBaseCombatWeapon::GetNextSecondaryAttack()
{
	return *(float*)((DWORD)this + offsets.m_flNextSecondaryAttack);
}

bool CBaseCombatWeapon::IsPinPulled() {
	return *(bool*)((DWORD)this + offsets.m_bPinPulled);
}
float CBaseCombatWeapon::GetThrowTime()
{
	return *(float*)((DWORD)this + offsets.m_fThrowTime);
}
float CBaseCombatWeapon::GetThrowStrength()
{
	return *(float*)((DWORD)this + offsets.m_flThrowStrength);
}
int CBaseCombatWeapon::Clip1()
{
	if (!I::Engine->is_connected() || !this)
		return 0;

	return *(int*)((DWORD)this + offsets.m_iClip1);
}

int CBaseCombatWeapon::GetZoomLevel()
{
	if (!I::Engine->is_connected() || !this)
		return 0;

	return *(int*)((DWORD)this + offsets.m_zoomLevel);
}

bool CBaseCombatWeapon::IsReloading()
{
	return *(bool*)((uintptr_t)this + 0x32A5);
}
std::string CBaseCombatWeapon::GetWeaponName()
{
	if (!this)
		return "";

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
		return ("Desert Eagle");
	case WEAPON_ELITE:
		return ("Dual Berettas");
	case WEAPON_FIVESEVEN:
		return ("Five-SeveN");
	case WEAPON_GLOCK:
		return ("Glock-18");
	case WEAPON_AK47:
		return ("AK-47");
	case WEAPON_AUG:
		return ("AUG");
	case WEAPON_AWP:
		return ("AWP");
	case WEAPON_FAMAS:
		return ("FAMAS");
	case WEAPON_G3SG1:
		return ("G3SG1");
	case WEAPON_GALILAR:
		return ("Galil");
	case WEAPON_M249:
		return ("M249");
	case WEAPON_M4A1:
		return ("M4A1");
	case WEAPON_MAC10:
		return ("MAC-10");
	case WEAPON_P90:
		return ("P90");
	case WEAPON_UMP45:
		return ("UMP-45");
	case WEAPON_XM1014:
		return ("XM1014");
	case WEAPON_BIZON:
		return ("PP-Bizon");
	case WEAPON_MAG7:
		return ("MAG-7");
	case WEAPON_NEGEV:
		return ("Negev");
	case WEAPON_SAWEDOFF:
		return ("Sawed-Off");
	case WEAPON_TEC9:
		return ("Tec-9");
	case WEAPON_TASER:
		return ("Taser");
	case WEAPON_HKP2000:
		return ("P2000");
	case WEAPON_MP7:
		return ("MP7");
	case WEAPON_MP9:
		return ("MP9");
	case WEAPON_NOVA:
		return ("Nova");
	case WEAPON_P250:
		return ("P250");
	case WEAPON_SCAR20:
		return ("SCAR-20");
	case WEAPON_SG556:
		return ("SG 553");
	case WEAPON_SSG08:
		return ("SSG 08");
	case WEAPON_KNIFE:
		return ("Knife");
	case WEAPON_FLASHBANG:
		return ("Flashbang");
	case WEAPON_HEGRENADE:
		return ("HE Grenade");
	case WEAPON_SMOKEGRENADE:
		return ("Smoke Grenade");
	case WEAPON_MOLOTOV:
		return ("Molotov");
	case WEAPON_DECOY:
		return ("Decoy");
	case WEAPON_INCGRENADE:
		return ("Incendiary Grenade");
	case WEAPON_C4:
		return ("C4");
	case WEAPON_KNIFE_T:
		return ("Knife");
	case WEAPON_M4A1_SILENCER:
		return ("M4A1-S");
	case WEAPON_USP_SILENCER:
		return ("USP-S");
	case WEAPON_CZ75A:
		return ("CZ75-Auto");
	case WEAPON_REVOLVER:
		return ("R8 Revolver");
	case WEAPON_MP5SD:
		return ("MP5-SD");
	default:
		return ("Knife");
	}

	return "";
}
bool CBaseCombatWeapon::IsGun()
{
	if (!this)
		return false;


	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
	case WEAPON_MP5SD:
		return true;
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
		return false;
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsSniper()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
		return false;
	case WEAPON_AWP:
		return true;
	case WEAPON_FAMAS:
		return false;
	case WEAPON_G3SG1:
		return true;
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_MP5SD:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
		return false;
	case WEAPON_SCAR20:
		return true;
	case WEAPON_SG556:
		return false;
	case WEAPON_SSG08:
		return true;
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return false;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsOneShotSniper()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
		return false;
	case WEAPON_AWP:
		return true;
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_MP5SD:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
		return false;
	case WEAPON_SSG08:
		return true;
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return false;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsRifle()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
		return false;
	case WEAPON_AK47:
	case WEAPON_AUG:
		return true;
	case WEAPON_AWP:
		return false;
	case WEAPON_FAMAS:
		return true;
	case WEAPON_G3SG1:
		return false;
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
		return true;
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
		return false;
	case WEAPON_NEGEV:
		return true;
	case WEAPON_SAWEDOFF:
		return false;
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP5SD:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
		return false;
	case WEAPON_SG556:
		return true;
	case WEAPON_SSG08:
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
		return false;
	case WEAPON_M4A1_SILENCER:
		return true;
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return false;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsSmg()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
		return false;
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
		return true;
	case WEAPON_XM1014:
		return false;
	case WEAPON_BIZON:
		return true;
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
		return false;
	case WEAPON_MP7:
	case WEAPON_MP5SD:
	case WEAPON_MP9:
		return true;
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return false;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsShotgun()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
		return false;
	case WEAPON_XM1014:
		return true;
	case WEAPON_BIZON:
		return false;
	case WEAPON_MAG7:
		return true;
	case WEAPON_NEGEV:
		return false;
	case WEAPON_SAWEDOFF:
		return true;
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP5SD:
	case WEAPON_MP9:
		return false;
	case WEAPON_NOVA:
		return true;
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return false;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsPistol()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
		return true;
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
		return false;
	case WEAPON_TEC9:
	case WEAPON_TASER:
	case WEAPON_HKP2000:
		return true;
	case WEAPON_MP7:
	case WEAPON_MP5SD:
	case WEAPON_MP9:
	case WEAPON_NOVA:
		return false;
	case WEAPON_P250:
		return true;
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
	case WEAPON_M4A1_SILENCER:
		return false;
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsGrenade()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_SMOKEGRENADE:
	case WEAPON_HEGRENADE:
	case WEAPON_INCGRENADE:
	case WEAPON_FLASHBANG:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
		return true;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsKnife()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_BAYONET:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return true;
	default:
		return false;
	}
}
bool CBaseCombatWeapon::IsRevolver()
{
	if (!this)
		return false;

	switch (this->GetWeaponID())
	{
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}