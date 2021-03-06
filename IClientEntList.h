#pragma once
class IClientEntityList
{
public:
	virtual void Function0() = 0;
	virtual void Function1() = 0;
	virtual void Function2() = 0;

	virtual CBaseEntity* GetClientEntity(int iIndex) = 0;
	virtual CBaseEntity* GetClientEntityFromHandle(HANDLE hHandle) = 0;//4
	virtual int				NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int				GetHighestEntityIndex() = 0;
	virtual void                  SetMaxEntities(int maxEnts) = 0;
	virtual int                   GetMaxEntities() = 0;
};