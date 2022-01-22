#include "Cheat.h"

void CAntiAim::AntiAim_run(CBaseEntity* pLocal,CUserCmd* ucmd, bool& SendPacket) {
	if (!I::Engine->is_ingame() || !G::LocalPlayer->GetAlive() || !G::LocalPlayer)
		return;
	this->cmd = ucmd;
	this->player = pLocal;

	if(player->GetFlags() & FL_FROZEN)
		return;

	// is not uses anything (open doors, defuse, etc)
	if (cmd->buttons & in_use)
		return;

	CBaseCombatWeapon* pWeapon = player->GetWeapon();

	if (pWeapon == nullptr)
		return;

	WeaponInfo_t* pWeaponData = pWeapon->GetCSWpnData();

	if (pWeaponData == nullptr)
		return;

	float flServerTime = TICKS_TO_TIME(E::Prediction->GetTickbase(cmd,player));
	// weapon shoot check
	if (pWeapon->IsGun() && pWeapon->CanFire() && (cmd->buttons & in_attack || (pWeapon->GetWeaponID() == WEAPON_REVOLVER && cmd->buttons & in_attack2)))
		return;
	else if (pWeaponData->WeaponType == WEAPONTYPE_KNIFE)
	{
		// slash
		if ((this->cmd->buttons & in_attack && pWeapon->GetNextPrimaryAttack() <= flServerTime) ||
			// stab
			(this->cmd->buttons & in_attack2 && pWeapon->GetNextSecondaryAttack() <= flServerTime))
			return;
	}
	// grenade throw check
	else if (pWeapon->IsGrenade() && pWeaponData->WeaponType == WEAPONTYPE_GRENADE)
	{
		// check is being thrown a grenade
		if (!pWeapon->IsPinPulled() || cmd->buttons & (in_attack | in_attack2))
		{
			if (pWeapon->GetThrowTime() > 0.f)
				return;
		}
	}
	// save angles to modify it later
	angSentView = cmd->view_angles;

	this->SetPitch(cmd, this->player);

	// send angles
	cmd->view_angles = angSentView;
}

void CAntiAim::SetPitch(CUserCmd* pCmd, CBaseEntity* pLocal) {

	
}