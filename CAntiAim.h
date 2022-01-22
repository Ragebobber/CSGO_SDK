#pragma once


class CAntiAim {
public:
	void AntiAim_run(CBaseEntity* pLocall, CUserCmd* cmd, bool& SendPacket);
private:
	CBaseEntity* player;
	CUserCmd* cmd;
	QAngle angSentView = { };
	void SetPitch(CUserCmd* pCmd, CBaseEntity* pLocal);
};