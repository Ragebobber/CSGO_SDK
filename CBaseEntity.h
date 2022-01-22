#pragma once

class CBaseCombatWeapon;
class IClientNetworkable;
class bf_read;
class IClientUnknown;
class IClientRenderable;

class CBaseEntity 
{
public:
	int	index;
	IClientNetworkable* GetIClientNetworkable();
	int GetHealth();
	bool HasImmunity();
	bool IsScoped();
	int GetArmor();
	bool GetDormant();
	IClientUnknown* GetIClientUnknown();
	IClientRenderable* GetIClientRenderable();
	HANDLE GetObserverTargetHandle();
	ICollideable* GetCollideable();
	Vector GetOrigin();
	matrix3x4_t& coordinate_frame();
	bool GetAlive();
	bool GetLifeState();
	player_info_t GetPlayerInfo();
	std::string GetName();
	int& GetTakeDamage();
	int GetTeam();
	bool IsEnemy();
	bool isVisible();
	bool SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	studiohdr_t* GetStudioModel();
	mstudiohitboxset_t* GetHitBoxSet();
	mstudiobbox_t* GetHitBox(int nHitbox);
	Vector GetHitboxPosition(int nHitbox);
	float HitChance(CBaseEntity* player, QAngle ang);
	float GetSimulationTime();
	Vector GetBonePosition(int iBone);
	Vector GetAbsOrigin();
	int m_fEffects();
	void SetAbsOrigin(const Vector& origin);
	void InvalidateBoneCache();
	Vector GetEyePosition();
	int GetHitboxSet();
	int GetTickBase();
	int SetTickBase(int tick);
	QAngle GetPunch();
	CBaseCombatWeapon* GetWeapon();
	bool IsPlayer();
	int GetShotsFired();
	bool HasHelmet();
	float NextAttack();
	void StudioFrameAdvance();
	void SetSequence(int iSequence);
	float GetFallVelocity();
	float SetFallVelocity(float set);
	void UpdateCollisionBounds();
	int GetFlags();
	Vector GetVelocity();
	bool PhysicsRunThink(int unk01);
	void RunPreThink();
	void RunThink();
	void RunPostThink();
	int GetMoveType();
};
class WeaponInfo_t
{
public:
	void* vtable;		//0x0000
	char* szConsoleName; //0x0004
	char pad_0008[8]; //0x0008
	void* pBaseItemDefinition; //0x0010
	int32_t iMaxClip1; //0x0014
	int32_t iMaxClip2; //0x0018
	int32_t iDefaultClip1; //0x001C
	int32_t iDefaultClip2; //0x0020
	int32_t iMaxReservedAmmo; //0x0024
	char pad_0028[4]; //0x0028
	char* szWorldModel; //0x002C
	char* szViewModel; //0x0030
	char* szDroppedModel; //0x0034
	char* szEmptySound; //0x0038
	char* szShotSound; //0x003C
	char pad_0040[20]; //0x0040
	char* szReloadSound; //0x0054
	char pad_0058[16]; //0x0058
	char* szShotSoundSpecial; //0x0068
	char pad_006C[12]; //0x006C
	char* szNearlyEmptySound; //0x0078
	char pad_007C[4]; //0x007C
	char* szBulletType; //0x0080
	char pad_0084[4]; //0x0084
	char* szHudName; //0x0088
	char* szWeaponName; //0x008C
	char pad_0090[12]; //0x0090
	int32_t iWeaponWeight; //0x009C
	int32_t iRumbleEffect; //0x00A0
	char pad_00A4[32]; //0x00A4
	void* pItemDefinition; //0x00C4
	EWeaponType WeaponType; //0x00C8
	int32_t WeaponGroup; //0x00CC
	int32_t iWeaponPrice; //0x00D0
	int32_t iKillAward; //0x00D4
	char* szPlayerAnimationExtension; //0x00D8
	float flCycleTime; //0x00DC
	float flCycleTimeAlt; //0x00E0
	float flTimeToIdle; //0x00E4
	float flIdleInterval; //0x00E8
	bool bFullAuto; //0x00EC
	char pad_00ED[3]; //0x00ED
	int32_t iDamage; //0x00F0
	float fHeadshotMultiplier; //0x00F4
	float flArmorRatio; //0x00F8
	int32_t iBullets; //0x00FC
	float flPenetration; //0x0100
	float flFlinchVelocityModifierLarge; //0x0104
	float flFlinchVelocityModifierSmall; //0x0108
	float flRange; //0x010C
	float flRangeModifier; //0x0110
	float flThrowVelocity; //0x0114
	char pad_0118[12]; //0x0118
	bool bHasSilencer; //0x0124
	char pad_0125[3]; //0x0125
	char* szSilencerModel; //0x0128
	int32_t iCrosshairMinDistance; //0x012C
	int32_t iCrosshairDeltaDistance; //0x0130
	float flMaxPlayerSpeed; //0x0134
	float flMaxPlayerSpeedAlt; //0x0138
	int32_t flAttackMoveSpeedFactor; //0x013C
	float flSpread; //0x0140
	float flSpreadAlt; //0x0144
	float flInaccuracyCrouch; //0x0148
	float flInaccuracyCrouchAlt; //0x014C
	float flInaccuracyStand; //0x0150
	float flInaccuracyStandAlt; //0x0154
	float flInaccuracyJumpInitial; //0x0158
	char pad_015C[4]; //0x015C
	float flInaccuracyJump; //0x0160
	float flInaccuracyJumpAlt; //0x0164
	float flInaccuracyLand; //0x0168
	float flInaccuracyLandAlt; //0x016C
	float flInaccuracyLadder; //0x0170
	float flInaccuracyLadderAlt; //0x0174
	float flInaccuracyFire; //0x0178
	float flInaccuracyFireAlt; //0x017C
	float flInaccuracyMove; //0x0180
	float flInaccuracyMoveAlt; //0x0184
	float flInaccuracyReload; //0x0188
	int32_t iRecoilSeed; //0x018C
	float flRecoilAngle; //0x0190
	float flRecoilAngleAlt; //0x0194
	float flRecoilAngleVariance; //0x0198
	float flRecoilAngleVarianceAlt; //0x019C
	float flRecoilMagnitude; //0x01A0
	float flRecoilMagnitudeAlt; //0x01A4
	float flRecoilMagnitudeVariance; //0x01A8
	float flRecoilMagnitudeVarianceAlt; //0x01AC
	int32_t iSpreadSeed; //0x01B0
	float flRecoveryTimeCrouch; //0x01B4
	float flRecoveryTimeStand; //0x01B8
	float flRecoveryTimeCrouchFinal; //0x01BC
	float flRecoveryTimeStandFinal; //0x01C0
	int32_t iRecoveryTransitionStartBullet; //0x01C4
	int32_t iRecoveryTransitionEndBullet; //0x01C8
	bool bUnzoomAfterShot; //0x01CC
	bool bHideViewModelZoomed; //0x01CD
	char pad_01CE[2]; //0x01CE
	int32_t iZoomLevels; //0x01D0
	int32_t iZoomFOVs[2]; //0x01D4
	float flZoomTimes[3]; //0x01DC
	char* szAddonLocation; //0x01E8
	char pad_01EC[4]; //0x01EC
	float flAddonScale; //0x01F0
	char* szEjectBrassEffect; //0x01F4
	char* szTracerEffect; //0x01F8
	int32_t iTracerFrequency; //0x01FC
	int32_t iTracerFrequencyAlt; //0x0200
	char* szFlashEffectFirstPerson; //0x0204
	char pad_0208[4]; //0x0208
	char* szFlashEffectThirdPerson; //0x020C
	char pad_0210[4]; //0x0210
	char* szHeatEffect; //0x0214
	float flHeatPerShot; //0x0218
	char* szZoomInSound; //0x021C
	char* szZoomOutSound; //0x0220
	float flInaccuracyPitchShift; //0x0224
	float flInaccuracySoundThreshold; //0x0228
	float flBotAudibleRange; //0x022C
	void* pPaintData; //0x0230
	char pad_0234[4]; //0x0234
	char* szWrongTeamMsg; //0x0238
	bool bHasBurstMode; //0x023C
	bool bIsRevolver; //0x023D
	bool bUnknown01; //0x023E
	bool bUnknown02; //0x023F
}; //Size=0x1090
class CBaseCombatWeapon
{
public:
	WeaponInfo_t* GetCSWpnData();
	short& GetWeaponID();
	void update_accuracy_penalty();
	float get_spread();
	float get_inaccuracy();
	bool CanFire();
	float& GetNextPrimaryAttack();
	float& GetNextSecondaryAttack();
	bool IsPinPulled();
	float GetThrowTime();
	float GetThrowStrength();
	int Clip1();
	int GetZoomLevel();
	bool IsReloading();
	bool IsRifle();
	bool IsSmg();
	bool IsShotgun();
	bool IsPistol();
	bool IsGrenade();
	bool IsKnife();
	bool IsRevolver();
	bool IsGun();
	bool IsSniper();
	bool IsOneShotSniper();
	std::string GetWeaponName();
};



class IClientNetworkable {
public:
	virtual IClientUnknown* get_client_unknown() = 0;
	virtual void release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void notify_should_transmit(int state) = 0;
	virtual void on_pre_data_changed(int updateType) = 0;
	virtual void on_data_changed(int updateType) = 0;
	virtual void pre_data_update(int updateType) = 0;
	virtual void post_data_update(int updateType) = 0;
	virtual void __unkn() = 0;
	virtual bool is_dormant() = 0;
	virtual int get_index() const = 0;
	virtual void receive_message(int classID, bf_read& msg) = 0;
	virtual void* get_data_table_base_ptr() = 0;
	virtual void set_destroyed_on_recreate_entities() = 0;
};

class IClientUnknown {
public:

};

class IClientRenderable {
public:
	virtual IClientUnknown* get_client_unknown() = 0;
	virtual Vector& get_render_origin(void) = 0;
	virtual Vector& get_render_angles(void) = 0;
	virtual bool should_draw(void) = 0;
	virtual int get_render_flags(void) = 0; // ERENDERFLAGS_xxx
	virtual void unused(void) const {}
	virtual unsigned short get_shadow_handle() const = 0;
	virtual unsigned short& render_handle() = 0;
	virtual const model_t* get_model() const = 0;
	virtual int draw_model(int flags, const uint8_t& instance) = 0;
	virtual int get_body() = 0;
	virtual void get_color_modulation(float* color) = 0;
	virtual bool lod_test() = 0;
	virtual bool setup_bones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void setup_weights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void do_animation_events(void) = 0;
	virtual void* /*IPVSNotify*/ get_pvs_notify_interface() = 0;
	virtual void get_render_bounds(Vector& mins, Vector& maxs) = 0;
	virtual void get_render_bounds_worldspace(Vector& mins, Vector& maxs) = 0;
	virtual void get_shadow_render_bounds(Vector& mins, Vector& maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool should_receive_projected_textures(int flags) = 0;
	virtual bool get_shadow_cast_distance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool get_shadow_cast_direction(Vector* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool is_shadow_dirty() = 0;
	virtual void mark_shadow_dirty(bool bDirty) = 0;
	virtual IClientRenderable* get_shadow_parent() = 0;
	virtual IClientRenderable* first_shadow_child() = 0;
	virtual IClientRenderable* next_shadow_peer() = 0;
	virtual int /*ShadowType_t*/ shadow_cast_type() = 0;
	virtual void create_model_instance() = 0;
	virtual unsigned short get_model_instance() = 0;
	virtual const matrix3x4_t& renderable_to_world_transform() = 0;
	virtual int lookup_attachment(const char* pAttachmentName) = 0;
	virtual bool get_attachment(int number, Vector& origin, Vector& angles) = 0;
	virtual bool get_attachment(int number, matrix3x4_t& matrix) = 0;
	virtual float* get_render_clip_plane(void) = 0;
	virtual int get_skin() = 0;
	virtual void on_threaded_draw_setup() = 0;
	virtual bool uses_flex_delayed_weights() = 0;
	virtual void record_tool_message() = 0;
	virtual bool should_draw_for_split_screen_user(int nSlot) = 0;
	virtual uint8_t override_alpha_modulation(uint8_t nAlpha) = 0;
	virtual uint8_t override_shadow_alpha_modulation(uint8_t nAlpha) = 0;
};