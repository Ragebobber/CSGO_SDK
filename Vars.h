#pragma once
class cRagebot {
public:
	bool rEnable;
	bool rOnKey;
	float FOV;
	int rKey;
	int Hitbox;
	bool LagCompensation;
	bool BodyAim;
	int BodyAimAferShoots;
	float MinDamage;
	bool RagePrioretez;
	bool rage_multiHitboxes[14];
	float pointScale;
	bool MultiPoint;
	bool NoRecoil;
	bool AutoScope;
	float t_hChance;
	bool AutoShoot;
	bool silent;
};


class cLegitbot {
public:
	bool aEnable;
	bool aFriendlyFire;
	bool aOnKey;
	int  aKey;
	struct
	{
		float 	FOV;
		int 	Hitbox;
		bool autowall;
		float mindamage;
		int targetset;
		float RCSAmountX;
		float RCSAmountY;
		float	Speed;
		int AimType;
		int StartBullet;
		int EndBullet;
		bool FastZoom;
		bool 	pSilent;
		float	PFOV;

	} Weapon[526];
};

class cVisuals {
public:
	bool vEnable;	
	//ESP
	bool vEsp;
	bool vHead_dot;
	bool vHealth;
	bool vArmor;
	bool vDebug;
	bool vOverrideFov;
	int vOverrideFovf;
	int vOverrideViewf;
	bool vShowName;
	float esp_player_bbox_color_t[3] = { 1.f, 0.f, 0.3f };
	float esp_player_bbox_color_ct[3] = { 0.f, 0.2f, 1.f };
	float esp_player_bbox_color_t_visible[3] = { 1.f, 1.f, 0.0f };
	float esp_player_bbox_color_ct_visible[3] = { 0.f, 0.7f, 1.f };	
	struct
	{
		bool ShowTeam ;
	} Filter;
};

class cTriggerbot {
public:
		bool tEnable;
		int tKey;
		float t_hChance;
		bool t_SmokeCheck;
		struct
		{
			bool Head;
			bool Chest;
			bool Stomach;
			bool Arms;
			bool Legs;
		} Filter;
};

class cMenu
{
public:
	bool	Opened = false;
};

class cMisc
{
public:
	int curcfg = 1;
	bool	mBhop;
	bool	mAStrafe;
};

class Variables
{
public:	
	cMenu Menu;
	cVisuals Visuals;
	cMisc Misc;
	cTriggerbot Triggerbot;
	cLegitbot Legitbot;
	cRagebot RageBot;
	int wpn;

	Variables()
	{
		wpn = -1;
	}
};

extern Variables Vars;