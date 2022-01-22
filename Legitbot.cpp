#include "Cheat.h"

void CLegitBot::LegitBot_run(bool& SendPacket) {
	if (!I::Engine->is_ingame() || !G::LocalPlayer->GetAlive() || !G::LocalPlayer)
		return;

	this->pLocal = G::LocalPlayer;
	this->cmd = G::UserCmd;

	this->curTime = this->GetTickbase(cmd) * I::Globals->interval_per_tick;
	this->doAim(SendPacket);
	
}

float CLegitBot::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	delta.Normalized();
	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

int CLegitBot::GetCurrentBone(CBaseEntity* Entity, CUserCmd* cmd) {
	int bone = -1;
	if (Vars.Legitbot.Weapon[Vars.wpn].Hitbox >= 9)
	{
		if (this->ClosestBone(Entity, cmd, bone)) 
			return bone;
		
	}
	else
		return Vars.Legitbot.Weapon[Vars.wpn].Hitbox;
		
	return bone;
}


bool CLegitBot::ClosestBone(CBaseEntity* Entity, CUserCmd* cmd, int& aimbone)
{
		float BestDist = 360.f;
		matrix3x4_t matrix[MAXSTUDIOBONES];

		if (!Entity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Globals->cur_time))
			return false;

		studiohdr_t* pStudioModel = I::ModelInfo->GetStudioModel(Entity->GetIClientRenderable()->get_model());
		if (!pStudioModel)
			return false;

		mstudiohitboxset_t* set = pStudioModel->get_hitbox_set(Entity->GetHitboxSet());
		if (!set)
			return false;

		for (int i = 0; i < set->numhitboxes; i++)
		{
			if (i == HITBOX_RIGHT_THIGH || i == HITBOX_LEFT_THIGH || i == HITBOX_RIGHT_CALF
				|| i == HITBOX_LEFT_CALF || i == HITBOX_RIGHT_FOOT || i == HITBOX_LEFT_FOOT
				|| i == HITBOX_RIGHT_HAND || i == HITBOX_LEFT_HAND || i == HITBOX_RIGHT_UPPER_ARM
				|| i == HITBOX_RIGHT_FOREARM || i == HITBOX_LEFT_UPPER_ARM || i == HITBOX_LEFT_FOREARM)
				continue;

			mstudiobbox_t* hitbox = set->get_hitbox(i);

			if (!hitbox)
				continue;

			auto thisdist = this->GetFov(cmd->view_angles, math::CalcAngle(this->pLocal->GetEyePosition(), Vector(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3])));

			if (BestDist > thisdist)
			{
				BestDist = thisdist;
				aimbone = hitbox->bone;
			}
		}
		return true;
}

int CLegitBot::FindTarget(CBaseEntity* pLocalPlayer, CUserCmd* pCmd)
{
	int iBestTarget = -1;
	float bestFov = Vars.Legitbot.Weapon[Vars.wpn].FOV;
	float flFoV = 0.f;
	if (this->pLocal->GetAlive() && !this->pLocal->GetDormant()) {
		for (int i = 0; i < I::ClientEntList->GetHighestEntityIndex(); i++)
		{
			CBaseEntity* pEntityPlayer = I::ClientEntList->GetClientEntity(i);
			if (!pEntityPlayer)
				continue;
			if (pEntityPlayer ==pLocal)
				continue;
			if (!pEntityPlayer->GetAlive())
				continue;
			if (pEntityPlayer->GetDormant())
				continue;
			if (pEntityPlayer->GetTeam() ==pLocal->GetTeam() && !Vars.Legitbot.aFriendlyFire)
				continue;

			Vector vecPos = pEntityPlayer->GetBonePosition(this->GetCurrentBone(pEntityPlayer, pCmd));
			if (!this->isautowall(vecPos, pEntityPlayer, pCmd))
				continue;

				flFoV = this->GetFov(pCmd->view_angles, math::CalcAngle(this->pLocal->GetEyePosition(), pEntityPlayer->GetBonePosition(this->GetCurrentBone(pEntityPlayer, pCmd))));
		
				if (flFoV < bestFov)
				{
					bestFov = flFoV;
					iBestTarget = i;
				}	
		}
	}
	return iBestTarget;
}

void CLegitBot::doAim(bool& SendPacket) {

	if (!this->pLocal->GetAlive())
		return;

	if (!this->pLocal->GetWeapon()->IsGun())
		return;

	if (!((Vars.Legitbot.aOnKey && G::PressedKeys[Vars.Legitbot.aKey])))
		return;

	if (this->pLocal->NextAttack() > this->curTime)
		return;

	int iTarget = this->FindTarget(this->pLocal, cmd);

	if (iTarget == -1)
		return;

	CBaseEntity* pEntityPlayer = I::ClientEntList->GetClientEntity(iTarget);

	Vector vecBone = pEntityPlayer->GetBonePosition(this->GetCurrentBone(pEntityPlayer, cmd));

	if (vecBone.IsZero())
		return;

	QAngle qAim = math::CalcAngle(this->pLocal->GetEyePosition(), vecBone);

	static bool psile = false;

	psile = (Vars.Legitbot.Weapon[Vars.wpn].pSilent && (this->pLocal->GetShotsFired() <= 1)) ? true : false;

	float FOV = psile ? Vars.Legitbot.Weapon[Vars.wpn].PFOV : Vars.Legitbot.Weapon[Vars.wpn].FOV;

	if (this->GetFov(cmd->view_angles, qAim) > FOV)
		return;

	auto flRecoilX = Vars.Legitbot.Weapon[Vars.wpn].RCSAmountX;
	auto flRecoilY = Vars.Legitbot.Weapon[Vars.wpn].RCSAmountY;

	if ((flRecoilX > 1 || flRecoilY > 1) &&pLocal->GetShotsFired() > 2) {
		if (this->pLocal->GetPunch().Length() > 0 && pLocal->GetPunch().Length() < 150)
		{
			if (flRecoilX > 1)
				qAim.x -=pLocal->GetPunch().x * (flRecoilX / 50.f);

			if (flRecoilY > 1)
				qAim.y -=pLocal->GetPunch().y * (flRecoilY / 50.f);
		}
	}

	SendPacket = psile ? false : true;

	float flSmooth = psile ? 0.f : Vars.Legitbot.Weapon[Vars.wpn].Speed;

	if (flSmooth >= 1.f && !psile)
	{
		QAngle delta = cmd->view_angles - qAim;
		if (!delta.IsZero())
		{
			delta.Normalized();
			qAim = cmd->view_angles - delta / flSmooth;
		}
	}
	qAim.Normalized();
	qAim.Clamp();

	cmd->view_angles = qAim;

	if (!psile || !Vars.Legitbot.Weapon[Vars.wpn].pSilent)
	{
		I::Engine->SetViewAngles(&cmd->view_angles);
	}
	
}


bool CLegitBot::isautowall(Vector& point, CBaseEntity* pBaseEntity, CUserCmd* pCmd) {

	if (Vars.Legitbot.Weapon[Vars.wpn].autowall)
	{
		if (!pBaseEntity->isVisible())
		{
			if (E::AutoWall->GetDamage(point, pBaseEntity) <= Vars.Legitbot.Weapon[Vars.wpn].mindamage)
				return false;
			else
				return true;
		}
		else
			return true;
	}
	else
	{
		if (!pBaseEntity->isVisible())
			return false;
		else
			return true;
	}
}

int CLegitBot::GetTickbase(CUserCmd* ucmd) {

	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;

	if (!ucmd)
		return g_tick;

	if (!g_pLastCmd || g_pLastCmd->is_predicted) {
		g_tick =pLocal->GetTickBase();
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}

	g_pLastCmd = ucmd;
	return g_tick;
}