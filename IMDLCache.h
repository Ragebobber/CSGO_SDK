#pragma once

#pragma region mdlcache_enumerations
enum
{
	MDLHANDLE_INVALID = static_cast<MDLHandle_t>(~0)
};

enum MDLCacheDataType_t : int
{
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK,
};

enum MDLCacheFlush_t : unsigned int
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,
	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};
#pragma endregion


class IMDLCache 
{
public://25

	void EndLock()
	{
		typedef void(__thiscall* OriginalFn)(void*);
		return U::GetVFunc<OriginalFn>(this, 34)(this);
	}
	void BeginLock()
	{
		typedef void(__thiscall* OriginalFn)(void*);
		return U::GetVFunc<OriginalFn>(this, 33)(this);
	}

};

