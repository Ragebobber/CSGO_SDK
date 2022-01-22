#pragma once

class CLegitBot {
public:
	void LegitBot_run(bool& SendPacket);
	
private:
	CBaseEntity* pLocal;
	CUserCmd* cmd;
	float GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
	int GetCurrentBone(CBaseEntity* Entity, CUserCmd* cmd);
	bool ClosestBone(CBaseEntity* Entity, CUserCmd* cmd, int& aimbone);
	int FindTarget(CBaseEntity* pLocalPlayer, CUserCmd* pCmd);
	void doAim(bool& SendPacket);
	bool isautowall(Vector& point, CBaseEntity* pBaseEntity, CUserCmd* pCmd);
	int GetTickbase(CUserCmd* ucmd);
	float curTime;
};