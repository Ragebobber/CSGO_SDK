#pragma once
class VectorAligned : public Vector
{
public:
	VectorAligned() {}

	VectorAligned(const Vector& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	float w;
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};
FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}
FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}
struct Ray_t
{
	Ray_t() { }
	Ray_t(Vector const& start, Vector const& end) {
		Init(start, end);
	}

	VectorAligned		m_Start;
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const matrix3x4_t* m_pWorldAxisTransform;
	bool				m_IsRay;
	bool				m_IsSwept;
	
	void Init(Vector vecStart, Vector vecEnd)
	{
		m_Delta = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}
	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}
};

/*struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};*/

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t	GetTraceType()
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(CBaseEntity* pServerEntity, int contentsMask)
	{
		return false;
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};
class c_trace_filter_skip_entity : public ITraceFilter {
public:
	void* m_skip;

	c_trace_filter_skip_entity(CBaseEntity* pEntityHandle) {
		m_skip = pEntityHandle;
	}

	bool should_hit_entity(CBaseEntity* pEntityHandle, int /*contentsMask*/) {
		return !(pEntityHandle == m_skip);
	}
	TraceType_t get_trace_type() const  {
		return TraceType_t::TRACE_EVERYTHING;
	}
};
class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities(void* pPassEnt1, void* pPassEnt2)
	{
		pPassEntity1 = pPassEnt1;
		pPassEntity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pPassEntity1 || pEntityHandle == pPassEntity2);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pPassEntity1;
	void* pPassEntity2;
};

typedef bool(*ShouldHitFunc_t)(IHandleEntity* pHandleEntity, int contentsMask);

class CTraceFilterSimple : public CTraceFilter
{
public:
	// It does have a base, but we'll never network anything below here..
	CTraceFilterSimple(const IHandleEntity* passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL);
	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask);
	virtual void SetPassEntity(const IHandleEntity* pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

	const IHandleEntity* GetPassEntity(void) { return m_pPassEnt; }

private:
	const IHandleEntity* m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;

};
class CTraceFilterIgnoreGrenades {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle) {
			return !(pEntityHandle == pSkip);
		}

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );

		ClientClass* clclass = ((CBaseEntity*)pEntityHandle)->GetIClientNetworkable()->GetClientClass();

		if (clclass) {
			if (strstr(clclass->m_pNetworkName, "CBaseCSGrenadeProjectile"))
				return false;

			//if ( clclass->m_ClassID == ClassID::_CBaseCSGrenadeProjectile )
			//	return false;
		}

		return !(pEntityHandle == pSkip);

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );
	}

	virtual TraceType_t    GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	IHandleEntity* pSkip;
	int m_icollisionGroup;
};
class CGrenadePredictTraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return !(pEntityHandle == pSkip);
		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );


		//if (contentsMask & CONTENTS_WINDOW)
		//    return false;

		if (pEntityHandle == pSkip)
			return false;

	//	if (((CBaseEntity*)pEntityHandle)->IsPlayer())
		//	return false;

		ClientClass* clclass = ((CBaseEntity*)pEntityHandle)->GetIClientNetworkable()->GetClientClass();

		if (clclass) {
			if (strstr(clclass->m_pNetworkName, "CBaseCSGrenadeProjectile"))
				return false;

			//if ( ReadInt( ( uintptr_t ) &clclass->m_ClassID ) == ClassID::_CBaseCSGrenadeProjectile )
			//return false;
		}

		return !(pEntityHandle == pSkip);

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );
	}

	virtual TraceType_t    GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	IHandleEntity* pSkip;
	int m_icollisionGroup;
};
class CBaseTrace
{
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

struct csurface_t
{
	const char* name;
	short			surfaceProps;
	unsigned short	flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const;
	int                     GetEntityIndex() const;
	bool                    DidHit() const;
	bool					IsVisible() const;

public:

	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	CBaseEntity* m_pEnt;
	int                     hitbox;

	CGameTrace() { }

private:
	CGameTrace(const CGameTrace& vOther);
};

inline bool CGameTrace::IsVisible() const
{
	return fraction > 0.97f;
}

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

typedef CGameTrace trace_t;