#pragma once

class CPrediction
{
public:
	void Start();
	int GetTickbase(CUserCmd* pCmd, CBaseEntity* pLocal);
	void PreStart();
	void End();
private:
	bool firstInit = false;
	int* iPredictionRandomSeed = nullptr;
	CBaseEntity* pSetPredictionEntity = nullptr;
	CBaseEntity* pLocal = nullptr;
	CUserCmd* ucmd = nullptr;
	CMoveData moveData = { };
	/* backup */
	float flOldCurrentTime = 0.f;
	float flOldFrameTime = 0.f;
	int iOldTickCount = 0;

	void UpdateButtonState(CBaseEntity* pLocal, CUserCmd* cmd);
};