#pragma once

class CRageBot
{
public:
	void RageBot_run(bool& SendPaket);
	
private:
	void FindTarget(bool& SendPaket);

	bool TargetSpecificEnt(int pEnt, int hitbox, bool& SendPaket);

	float BestHitPoint(CBaseEntity* player, int prioritized, float minDmg, mstudiohitboxset_t* hitset, matrix3x4_t matrix[], Vector& vecOut);

	bool HitChance(QAngle angles, CBaseEntity* ent, float chance);

	Vector CalculatePoint(CBaseEntity* pEnt, int prioritized, float minDmg, bool onlyPrioritized);

	bool CheckTarget(int i);

	CBaseEntity* pLocal = nullptr;
	CUserCmd* pUserCmd = nullptr;
	CBaseCombatWeapon* pLocalWeapon = nullptr;
	
	int prev_aimtarget = 0;

	float cur_time = 0.f;
	float m_bestfov = 0.f;
	int m_hitbox = 0;
	float  m_distance = 0.f;
};

