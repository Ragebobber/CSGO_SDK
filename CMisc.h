#pragma once

class CMisc {
public:
	void MRun();
	void FixMovement(CUserCmd* usercmd, QAngle& wish_angle);
private:
	CBaseEntity* player;
	CUserCmd* cmd;
	void BHop(CUserCmd* pCmd, CBaseEntity* pLocal);
	void AStrafe(CUserCmd* pCmd, CBaseEntity* pLocal);

	

	template < typename t >
	t clamp(t in, t low, t high) {
		return min(max(in, low), high);
	}

};