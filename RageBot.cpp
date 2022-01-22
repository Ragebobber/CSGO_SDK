#include "Cheat.h"

void CRageBot::RageBot_run(bool & SendPaket) {
	if (!I::Engine->is_ingame() || !G::LocalPlayer->GetAlive() || !G::LocalPlayer)
		return;

	this->pLocal = G::LocalPlayer;
	this->pUserCmd = G::UserCmd;
	this->pLocalWeapon = pLocal->GetWeapon();
	this->cur_time = E::Prediction->GetTickbase(pUserCmd,pLocal) * I::Globals->interval_per_tick;

	if (!pLocalWeapon)
		return;

	if (pLocal->NextAttack() > cur_time)
		return;
	
	if (Vars.RageBot.rOnKey && !G::PressedKeys[Vars.RageBot.rKey])
		return;

	if (!pLocalWeapon->IsGun() || pLocalWeapon->Clip1() < 1)
		return;

	FindTarget(SendPaket);
}

float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;

	math::AngleVectors(viewAngle, aim);
	math::AngleVectors(aimAngle, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

void CRageBot::FindTarget(bool& SendPaket) {
	m_bestfov = Vars.RageBot.FOV;
	m_hitbox = Vars.RageBot.Hitbox;
	
	if (pLocalWeapon->IsPistol() && pUserCmd->tick_count % 2)
	{
		static int lastshot;
		if (pUserCmd->buttons & in_attack)
			lastshot++;

		if (!pUserCmd->buttons & in_attack || lastshot > 1)
		{
			pUserCmd->buttons &= ~in_attack;
			lastshot = 0;
		}
		return;
	}

	if (prev_aimtarget && CheckTarget(prev_aimtarget))
	{
		if (TargetSpecificEnt(prev_aimtarget, m_hitbox, SendPaket))
			return;
	}

	for (int i = 1; i < I::Engine->GetMaxClients(); i++)
	{
		// Failed to shoot at him again, reset him and try others.
		if (prev_aimtarget && prev_aimtarget == i)
		{
			prev_aimtarget = 0;
			continue;
		}

		if (!CheckTarget(i))
			continue;

		CBaseEntity* Entity = I::ClientEntList->GetClientEntity(i);
		
		Vector hitboxvec = Entity->GetBonePosition(m_hitbox);
		
		
		float fov = GetFov(pUserCmd->view_angles, math::CalcAngle(pLocal->GetEyePosition(), hitboxvec));
		
		if (fov < m_bestfov)
		{
			m_bestfov = fov;
			TargetSpecificEnt(i, m_hitbox, SendPaket);
		}
		
	}
}

bool CRageBot::TargetSpecificEnt(int idx, int hitbox, bool& SendPaket) {
	CBaseEntity* pEntityPlayer = I::ClientEntList->GetClientEntity(idx);
	auto firedShots = pLocal->GetShotsFired();
	Vector vecBone;
	static bool shooting = false;

	if (Vars.RageBot.BodyAim && firedShots > Vars.RageBot.BodyAimAferShoots)
		vecBone = CalculatePoint(pEntityPlayer, HITBOX_PELVIS, Vars.RageBot.MinDamage,Vars.RageBot.RagePrioretez);
	else
		vecBone = CalculatePoint(pEntityPlayer, m_hitbox, Vars.RageBot.MinDamage, Vars.RageBot.RagePrioretez);
	
	if (vecBone.IsZero())
		return false;

	QAngle new_aim_angles = math::CalcAngle(pLocal->GetEyePosition(), vecBone) - (Vars.RageBot.NoRecoil ? pLocal->GetPunch() * 2.f : QAngle(0, 0, 0));
	
	new_aim_angles.Normalized();

	pUserCmd->view_angles = new_aim_angles;

	if (pLocalWeapon->CanFire())
	{
		// Save more fps by remembering to try the same entity again next time.
		prev_aimtarget = pEntityPlayer->GetIClientNetworkable()->get_index();

		if (Vars.RageBot.AutoScope && pLocalWeapon->IsSniper() && pLocalWeapon->GetZoomLevel() == 0)
		{
			pUserCmd->buttons |= in_attack2;
		}
		else if (this->HitChance(pUserCmd->view_angles,pEntityPlayer, Vars.RageBot.t_hChance))
		{
			if (Vars.RageBot.AutoShoot)
			{
				pUserCmd->buttons |= in_attack;

				if (Vars.RageBot.silent)//may be incorrecct with AA
					SendPaket = pUserCmd->buttons |= in_attack ? false : true;

			}
		}
	}

	if(!Vars.RageBot.silent)
		I::Engine->SetViewAngles(&pUserCmd->view_angles);

	return true;
}

float CRageBot::BestHitPoint(CBaseEntity* player, int prioritized, float minDmg, mstudiohitboxset_t* hitset, matrix3x4_t matrix[], Vector& vecOut)
{
	mstudiobbox_t* hitbox = hitset->get_hitbox(prioritized);
	if (!hitbox)
		return 0.f;

	std::vector<Vector> vecArray;
	float flHigherDamage = 0.f;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector max;
	Vector min;

	Vector tr0 = Vector(hitbox->bbmax.x + mod, hitbox->bbmax.y + mod, hitbox->bbmax.z + mod);
	Vector tr1 = Vector(hitbox->bbmin.x + mod, hitbox->bbmin.y + mod, hitbox->bbmin.z + mod);

	math::vector_transform(tr0, matrix[hitbox->bone], max);
	math::vector_transform(tr1, matrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	QAngle curAngles = math::CalcAngle(center, pLocal->GetEyePosition());

	Vector forward;
	math::AngleVectors(curAngles, forward);

	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);

	Vector top = Vector(0, 0, 1);
	Vector bot = Vector(0, 0, -1);

	const float POINT_SCALE = Vars.RageBot.pointScale;
	if (Vars.RageBot.MultiPoint)
	{
		switch (prioritized)
		{
		case HITBOX_HEAD:
			for (auto i = 0; i < 4; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[1] += top * (hitbox->m_flRadius * POINT_SCALE);
			vecArray[2] += right * (hitbox->m_flRadius * POINT_SCALE);
			vecArray[3] += left * (hitbox->m_flRadius * POINT_SCALE);
			break;

		default:

			for (auto i = 0; i < 2; ++i)
			{
				vecArray.emplace_back(center);
			}
			vecArray[0] += right * (hitbox->m_flRadius * POINT_SCALE);
			vecArray[1] += left * (hitbox->m_flRadius * POINT_SCALE);
			break;
		}
	}
	else
		vecArray.emplace_back(center);

	for (Vector cur : vecArray)
	{
		float flCurDamage = E::AutoWall->GetDamageRage(cur, player, prioritized);

		if (!flCurDamage)
			continue;

		if ((flCurDamage > flHigherDamage) && (flCurDamage > minDmg))
		{
			flHigherDamage = flCurDamage;
			vecOut = cur;
		}
	}
	return flHigherDamage;
}
bool CRageBot::HitChance(QAngle angles, CBaseEntity* ent, float chance)
{

	if (!pLocalWeapon)
		return false;

	Vector forward, right, up;
	Vector src = pLocal->GetEyePosition();
	math::AngleVectors(angles, forward, right, up);

	int cHits = 0;
	int cNeededHits = static_cast<int>(150.f * (chance / 100.f));

	pLocalWeapon->update_accuracy_penalty();
	float weap_spread = pLocalWeapon->get_spread();
	float weap_inaccuracy = pLocalWeapon->get_inaccuracy();

	for (int i = 0; i < 150; i++)
	{
		float a = U::RandomFloat(0.f, 1.f);
		float b = U::RandomFloat(0.f, 2.f * M_PI_F);
		float c = U::RandomFloat(0.f, 1.f);
		float d = U::RandomFloat(0.f, 2.f * M_PI_F);

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		if (pLocalWeapon->GetWeaponID() == 64)
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		Vector spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalize();

		QAngle viewAnglesSpread;
		math::VectorAngles(direction, up, viewAnglesSpread);
		viewAnglesSpread.Normalized();

		Vector viewForward;
		math::AngleVectors(viewAnglesSpread, viewForward);
		viewForward.NormalizeInPlace();

		viewForward = src + (viewForward * pLocalWeapon->GetCSWpnData()->flRange);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, viewForward);
		I::EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, ent, &tr);

		if (tr.m_pEnt == ent)
			++cHits;

		if (static_cast<int>((static_cast<float>(cHits) / 150.f) * 100.f) >= chance)
			return true;

		if ((150 - i + cHits) < cNeededHits)
			return false;
	}
	return false;
}
Vector CRageBot::CalculatePoint(CBaseEntity* pEnt, int prioritized, float minDmg, bool onlyPrioritized) {
	
	Vector vecOutput = Vector(0, 0, 0);

	matrix3x4_t matrix[MAXSTUDIOBONES];

	if (!pEnt->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Globals->cur_time))
		return vecOutput;

	studiohdr_t* pStudioModel = I::ModelInfo->GetStudioModel(pEnt->GetIClientRenderable()->get_model());
	if (!pStudioModel)
		return vecOutput;

	mstudiohitboxset_t* set = pStudioModel->get_hitbox_set(pEnt->GetHitboxSet());
	if (!set)
		return vecOutput;

	if (BestHitPoint(pEnt, prioritized, minDmg, set, matrix, vecOutput) > minDmg && onlyPrioritized)
	{
		return vecOutput;
	}
	else
	{
		float flHigherDamage = 0.f;

		Vector vecCurVec;

		// why not use all the hitboxes then
		//static Hitboxes hitboxesLoop;
		static int hitboxesLoop[] =
		{
			HITBOX_HEAD,
			HITBOX_PELVIS,
			HITBOX_UPPER_CHEST,
			HITBOX_CHEST,
			HITBOX_NECK,
			HITBOX_LEFT_FOREARM,
			HITBOX_RIGHT_FOREARM,
			HITBOX_RIGHT_HAND,
			HITBOX_LEFT_THIGH,
			HITBOX_RIGHT_THIGH,
			HITBOX_LEFT_CALF,
			HITBOX_RIGHT_CALF,
			HITBOX_LEFT_FOOT,
			HITBOX_RIGHT_FOOT
		};

		int loopSize = ARRAYSIZE(hitboxesLoop);

		for (int i = 0; i < loopSize; ++i)
		{
			if (!Vars.RageBot.rage_multiHitboxes[i])
				continue;

			float flCurDamage = BestHitPoint(pEnt, hitboxesLoop[i], minDmg, set, matrix, vecCurVec);

			if (!flCurDamage)
				continue;

			if (flCurDamage > flHigherDamage)
			{
				flHigherDamage = flCurDamage;
				vecOutput = vecCurVec;
				if (static_cast<int32_t>(flHigherDamage) >= pEnt->GetHealth())
					break;
			}
		}
		return vecOutput;
	}
}
bool CRageBot::CheckTarget(int i)
{
	CBaseEntity* player = I::ClientEntList->GetClientEntity(i);

	if (!player || player == nullptr)
		return false;

	if (player == pLocal)
		return false;

	if (!player->IsEnemy())
		return false;

	if (player->GetIClientNetworkable()->is_dormant())
		return false;

	if (player->HasImmunity())
		return false;

	if (!player->GetAlive())
		return false;

	return true;
}