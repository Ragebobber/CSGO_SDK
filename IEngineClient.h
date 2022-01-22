#pragma once
struct client_textmessage_t;
class CSentence;
class CAudioSource;
class IEngineClient
{
public:
	virtual void unk0(void) = 0;
	virtual void unk1(void) = 0;
	virtual void unk2(void) = 0;
	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool        CopyFile(const char* source, const char* destination) = 0;
	virtual void        GetScreenSize(int& width, int& height) = 0;//5
	virtual void                  ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
	virtual void                  ClientCmd(const char* szCmdString) = 0;
	virtual bool                  GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;
	virtual int                   GetPlayerForUserID(int userID) = 0;
	virtual client_textmessage_t* TextMessageGet(const char* pName) = 0; // 10
	virtual bool                  Con_IsVisible(void) = 0;
	virtual int                   get_local_player(void) = 0;
	virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;
	virtual float                 GetLastTimeStamp(void) = 0;
	virtual CSentence* GetSentence(CAudioSource* pAudioSource) = 0; // 15
	virtual float                 GetSentenceLength(CAudioSource* pAudioSource) = 0;
	virtual bool                  IsStreaming(CAudioSource* pAudioSource) const = 0;
	virtual void                  GetViewAngles(QAngle* va) = 0;
	virtual void                  SetViewAngles(QAngle* va) = 0;
	virtual int                   GetMaxClients(void) = 0; // 20
	virtual const char* Key_LookupBinding(const char* pBinding) = 0;
	virtual const char* Key_BindingForKey(int& code) = 0;
	virtual void                  Key_SetBinding(int, char const*) = 0;
	virtual void                  StartKeyTrapMode(void) = 0;
	virtual bool                  CheckDoneKeyTrapping(int& code) = 0;
	virtual bool                  is_ingame(void) = 0;
	virtual bool                  is_connected(void) = 0;
	virtual bool                  IsDrawingLoadingImage(void) = 0;
	virtual void                  HideLoadingPlaque(void) = 0;
	virtual void                  Con_NPrintf(int pos, const char* fmt, ...) = 0; // 30
	virtual void                  Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
	virtual int                   IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;
	virtual int                   IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;
	virtual bool                  CullBox(const Vector& mins, const Vector& maxs) = 0;
	virtual void                  Sound_ExtraUpdate(void) = 0;
	virtual const char* GetGameDirectory(void) = 0;
	virtual const v_matrix& w2s_m() = 0;
	virtual const v_matrix& WorldToViewMatrix() = 0;

	bool IsActiveApp()
	{
		using fn = bool(__thiscall*)(PVOID);
		return U::GetVFunc<fn>(this, 196)(this);
	}
private:

};

