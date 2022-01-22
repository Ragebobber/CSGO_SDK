#pragma once

class IPrediction {
public:
	void Update(int iStartFrame, bool bValidFrame, int nIncomingAcknowledged, int nOutgoingCommand)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, bool, int, int);
		return U::GetVFunc<OriginalFn>(this, 3)(this, iStartFrame, bValidFrame, nIncomingAcknowledged, nOutgoingCommand);
	}
	void GetLocalViewAngles(QAngle& angView)
	{
		typedef void(__thiscall* OriginalFn)(void*, QAngle&);
		return U::GetVFunc<OriginalFn>(this, 12)(this, angView);
	}

	void SetLocalViewAngles(QAngle& angView)
	{
		typedef void(__thiscall* OriginalFn)(void*, QAngle&);
		return U::GetVFunc<OriginalFn>(this, 13)(this, angView);
	}

	void CheckMovingGround(CBaseEntity* pEntity, double dbFrametime)
	{
		typedef void(__thiscall* OriginalFn)(void*, CBaseEntity*, double);
		return U::GetVFunc<OriginalFn>(this, 18)(this, pEntity, dbFrametime);
	}

	void SetupMove(CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* pHelper, CMoveData* pMoveData)
	{
		typedef void(__thiscall* OriginalFn)(void*, CBaseEntity* , CUserCmd* , IMoveHelper* , CMoveData* );
		return U::GetVFunc<OriginalFn>(this, 20)(this,pEntity, pCmd, pHelper, pMoveData);
	}

	void FinishMove(CBaseEntity* pEntity, CUserCmd* pCmd, CMoveData* pMoveData)
	{
		typedef void(__thiscall* OriginalFn)(void*, CBaseEntity*, CUserCmd*, CMoveData*);
		return U::GetVFunc<OriginalFn>(this, 21)(this, pEntity, pCmd,pMoveData);
	}
public:

	char 			pad0[0x4];						// 0x0000
	DWORD*			hLastGround;					// 0x0004
	bool			bInPrediction;					// 0x0008
	bool			bIsFirstTimePredicted;			// 0x0009
	bool			bEnginePaused;					// 0x000A
	bool			bOldCLPredictValue;				// 0x000B
	int				iPreviousStartFrame;			// 0x000C
	int				nIncomingPacketNumber;			// 0x0010
	float			flLastServerWorldTimeStamp;		// 0x0014
};