#pragma once

class CAutoWall
{
public:
	float GetDamage(const Vector& point, CBaseEntity* ent);

	float GetDamageRage(const Vector& vecPoint, CBaseEntity* player, int hitbox);

private:


	struct FireBulletData
	{
		Vector          src;
		trace_t         enter_trace;
		Vector          direction;
		CTraceFilter    filter;
		WeaponInfo_t*   wpn_data;
		CBaseEntity*	entity;
		float           trace_length;
		float           trace_length_remaining;
		float			current_damage;
		int             penetrate_count;
	};
	std::string m_enter_material_name;
	std::string m_exit_material_name;

	float GetHitgroupDamageMultiplier(int iHitGroup);

	bool IsArmored(CBaseEntity* Entity, int ArmorValue, int Hitgroup);

	void ScaleDamage(int Hitgroup, CBaseEntity* Entity, float WeaponArmorRatio, float& Damage);

	bool IsBreakable(CBaseEntity* entity);

	bool TraceToExit(Vector& end, const Vector& start, const Vector& dir, trace_t* enter_trace, trace_t* exit_trace);

	bool HandleBulletPenetration(FireBulletData& data);

	void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr);

	void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr, FireBulletData& data);

	bool SimulateFireBullet(FireBulletData& data);

	bool SimulateFireBulletRage(FireBulletData& data, CBaseEntity* player, int hitbox);

};
