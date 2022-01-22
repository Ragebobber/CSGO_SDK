#pragma once
using FnCommandCallbackV1_t = void(__cdecl*)();
using FnChangeCallback_t = void(__cdecl*)(void*, const char*, float);

class CWrappedProtect
{
public:
	CWrappedProtect(void* pBaseAddress, std::size_t uLength, DWORD dwFlags)
	{
		this->pBaseAddress = pBaseAddress;
		this->uLength = uLength;

		if (!VirtualProtect(pBaseAddress, uLength, dwFlags, &dwOldFlags))
			throw std::system_error(GetLastError(), std::system_category(), ("failed to protect region"));
	}

	~CWrappedProtect()
	{
		VirtualProtect(pBaseAddress, uLength, dwOldFlags, &dwOldFlags);
	}
private:
	void* pBaseAddress;
	std::size_t		uLength;
	DWORD			dwOldFlags;
};

class CConVar
{
public:
	const char* GetName()
	{
		typedef const char*(__thiscall* OriginalFn)(void*);
		return U::GetVFunc<OriginalFn>(this, 5)(this);
	}

	float GetFloat() // idx @ 11
	{
		DWORD uXored = *(DWORD*)(&pParent->flValue) ^ (DWORD)(this);
		return *reinterpret_cast<float*>(&uXored);
	}

	int GetInt() // idx @ 12
	{
		return (int)(pParent->iValue ^ (int)(this));
	}

	bool GetBool() // idx @ 13
	{
		return !!GetInt();
	}

	const char* GetString() const
	{
		char const* szValue = pParent->szString;
		return szValue ? szValue : "";
	}

	void SetValue(const char* szValue)
	{
		typedef void(__thiscall* OriginalFn)(void*, const char*);
		return U::GetVFunc<OriginalFn>(this, 14)(this, szValue);
	}

	void SetValue(float flValue)
	{
		typedef void(__thiscall* OriginalFn)(void*, float);
		return U::GetVFunc<OriginalFn>(this, 15)(this, flValue);
	}

	void SetValue(int iValue)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		return U::GetVFunc<OriginalFn>(this, 16)(this, iValue);
	}

	void SetValue(Color colValue)
	{
		typedef void(__thiscall* OriginalFn)(void*, Color);
		return U::GetVFunc<OriginalFn>(this, 17)(this, colValue);
	}

public:
	char					pad0[0x4];			//0x00
	CConVar* pNext;				//0x04
	bool							bRegistered;		//0x08
	const char* szName;				//0x0C
	const char* szHelpString;		//0x10
	int								nFlags;				//0x14
	FnCommandCallbackV1_t			pCallback;			//0x18
	CConVar* pParent;			//0x1C
	const char* szDefaultValue;		//0x20
	char* szString;			//0x24
	int								iStringLength;		//0x28
	float							flValue;			//0x2C
	int								iValue;				//0x30
	bool							bHasMin;			//0x34
	float							flMinValue;			//0x38
	bool							bHasMax;			//0x3C
	float							flMaxValue;			//0x40
	CUtlVector<FnChangeCallback_t>	fnChangeCallbacks;	//0x44
};

class CSpoofedConVar // @credits: markhc
{
public:
	CSpoofedConVar() = default;
	CSpoofedConVar(const char* szCVar);
	CSpoofedConVar(CConVar* pCVar);
	~CSpoofedConVar();

	// Check
	bool	IsSpoofed() const;
	void	Spoof();

	// Flags
	void	SetFlags(int iFlags) const;
	int		GetFlags() const;

	// Set
	void	SetBool(bool bValue) const;
	void	SetInt(int iValue) const;
	void	SetFloat(float flValue) const;
	void	SetString(const char* szValue) const;

private:
	CConVar* pOriginalCVar = nullptr;
	CConVar* pDummyCVar = nullptr;
	char		szDummyName[128] = { };
	char		szDummyValue[128] = { };
	char		szOriginalName[128] = { };
	char		szOriginalValue[128] = { };
	int			iOriginalFlags = 0;
};