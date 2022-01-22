#pragma once


class IMoveHelper
{
public:
	virtual	char const* GetName(CBaseHandle handle) const = 0;

	// sets the entity being moved
	virtual void	SetHost(CBaseEntity* host) = 0;
	
	virtual void	ResetTouchList(void) = 0;
	virtual bool	AddToTouched(const CGameTrace & tr, const Vector & impactvelocity) = 0;
	virtual void	ProcessImpacts(void) = 0;
};

