#include "Cheat.h"

void CMisc::MRun() {
	if (!I::Engine->is_ingame() || !G::LocalPlayer->GetAlive() || !G::LocalPlayer)
		return;

	Vars.wpn = G::LocalPlayer->GetWeapon()->GetWeaponID();

	player = G::LocalPlayer;
	cmd = G::UserCmd;
	if(Vars.Misc.mBhop)
		this->BHop(cmd,player);
	if (Vars.Misc.mAStrafe)
		this->AStrafe(cmd, player);

}

void CMisc::BHop(CUserCmd* pCmd, CBaseEntity* pLocal) {

	if (pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP || pLocal->GetMoveType() == MOVETYPE_OBSERVER)
		return;

	static bool bLastJumped = false, bShouldFake = false;

	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		pCmd->buttons |= in_jump;
	}
	else if (pCmd->buttons & in_jump)
	{
		if (pLocal->GetFlags() & FL_ONGROUND || pLocal->GetFlags() & FL_PARTIALGROUND)
		{
			bLastJumped = true;
			bShouldFake = true;
		}
		else
		{
			pCmd->buttons &= ~in_jump;
			bLastJumped = false;
		}
	}
	else
	{
		bLastJumped = false;
		bShouldFake = false;
	}
}


void CMisc::AStrafe(CUserCmd* pCmd, CBaseEntity* pLocal) {
	if (pLocal->GetFlags() & FL_ONGROUND && !(cmd->buttons & in_jump))
		return;

	if (pLocal->GetMoveType() & (MOVETYPE_LADDER | MOVETYPE_NOCLIP))
		return;

	QAngle viewangles;
	I::Engine->GetViewAngles(&viewangles);

	static bool side_switch = false;
	side_switch = !side_switch;

	cmd->forward_move = 0.f;
	cmd->side_move = side_switch ? 450.f : -450.f;

	float velocity_yaw = pLocal->GetVelocity().y;

	float rotation = this->clamp< float >(RAD2DEG(std::atan2(15.f, pLocal->GetVelocity().Length2D())), 0.f, 90.f);

	float abs_yaw = fabs(math::NormalizeAngle(velocity_yaw - viewangles.y));

	float ideal_yaw_rotation = (side_switch ? rotation : -rotation) + (abs_yaw < 5.f ? velocity_yaw : viewangles.y);

	float cos_rot = cos(DEG2RAD(viewangles.y - ideal_yaw_rotation));
	float sin_rot = sin(DEG2RAD(viewangles.y - ideal_yaw_rotation));

	cmd->forward_move = (cos_rot * cmd->forward_move) - (sin_rot * cmd->side_move);
	cmd->side_move = (sin_rot * cmd->forward_move) + (cos_rot * cmd->side_move);
}

void CMisc::FixMovement(CUserCmd* usercmd, QAngle& wish_angle)
{
	Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	auto viewangles = usercmd->view_angles;
	viewangles.Normalize();

	math::AngleVectors(wish_angle, view_fwd, view_right, view_up);
	math::AngleVectors(viewangles, cmd_fwd, cmd_right, cmd_up);

	const float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	const float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	const float v12 = sqrtf(view_up.z * view_up.z);

	const Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	const Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	const Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	const float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
	const float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
	const float v18 = sqrtf(cmd_up.z * cmd_up.z);

	const Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
	const Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
	const Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

	const float v22 = norm_view_fwd.x * usercmd->forward_move;
	const float v26 = norm_view_fwd.y * usercmd->forward_move;
	const float v28 = norm_view_fwd.z * usercmd->forward_move;
	const float v24 = norm_view_right.x * usercmd->side_move;
	const float v23 = norm_view_right.y * usercmd->side_move;
	const float v25 = norm_view_right.z * usercmd->side_move;
	const float v30 = norm_view_up.x * usercmd->up_move;
	const float v27 = norm_view_up.z * usercmd->up_move;
	const float v29 = norm_view_up.y * usercmd->up_move;

	usercmd->forward_move = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
		+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
		+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
	usercmd->side_move = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
		+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
		+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
	usercmd->up_move = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
		+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
		+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

	usercmd->forward_move = clamp(usercmd->forward_move, -450.f, 450.f);
	usercmd->forward_move = clamp(usercmd->side_move, -450.f, 450.f);
	usercmd->forward_move = clamp(usercmd->up_move, -320.f, 320.f);
}