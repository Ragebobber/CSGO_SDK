#pragma once
#define in_attack (1 << 0)
#define in_jump (1 << 1)
#define in_duck (1 << 2)
#define in_forward (1 << 3)
#define in_back  (1 << 4)
#define in_use  (1 << 5)
#define in_cancel  (1 << 6)
#define in_left  (1 << 7)
#define in_right  (1 << 8)
#define in_moveleft  (1 << 9)
#define in_moveright  (1 << 10)
#define in_attack2  (1 << 11)
#define in_run  (1 << 12)
#define in_reload  (1 << 13)
#define in_alt1  (1 << 14)
#define in_alt2  (1 << 15)
#define in_score  (1 << 16)
#define in_speed  (1 << 17)
#define in_walk  (1 << 18)
#define in_zoom  (1 << 19)
#define in_weapon1  (1 << 20)
#define in_weapon2  (1 << 21)
#define in_bullrush  (1 << 22)
#define in_grenade1  (1 << 23)
#define in_grenade2  (1 << 24)
#define in_attack3  (1 << 25



class CUserCmd
{
public:
	CUserCmd() {};
	virtual ~CUserCmd() {};
	int command_number;
	int tick_count;
	QAngle view_angles;
	Vector aim_direction;
	float forward_move;
	float side_move;
	float up_move;
	int buttons;
	unsigned char impulse;
	int weapon_select;
	int weapon_subtype;
	int random_seed;
	short mouse_dx;
	short mouse_dy;
	bool is_predicted;
	Vector	vecHeadAngles;
	Vector	vecHeadOffset;
};

class CMoveData
{
public:
	bool			bFirstRunOfFunctions : 1;
	bool			bGameCodeMovedPlayer : 1;
	bool			bNoAirControl : 1;
	DWORD*			hPlayerHandle;		// edict index on server, client entity handle on client=
	int				nImpulseCommand;	// impulse command issued.
	QAngle			angViewAngles;		// command view angles (local space)
	QAngle			angAbsViewAngles;	// command view angles (world space)
	int				nButtons;			// attack buttons.
	int				nOldButtons;		// from host_client->oldbuttons;
	float			flForwardMove;
	float			flSideMove;
	float			flUpMove;
	float			flMaxSpeed;
	float			flClientMaxSpeed;
	Vector			vecVelocity;		// edict::velocity	// current movement direction.
	Vector			vecTrailingVelocity;
	float			flTrailingVelocityTime;
	Vector			vecAngles;			// edict::angles
	Vector			vecOldAngles;
	float			flOutStepHeight;	// how much you climbed this move
	Vector			vecOutWishVel;		// this is where you tried 
	Vector			vecOutJumpVel;		// this is your jump velocity
	Vector			vecConstraintCenter;
	float			flConstraintRadius;
	float			flConstraintWidth;
	float			flConstraintSpeedFactor;
	bool			bConstraintPastRadius;
	Vector			vecAbsOrigin;
};