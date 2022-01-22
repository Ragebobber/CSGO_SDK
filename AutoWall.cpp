#include "Cheat.h"
//#include "ImGui/imstb_truetype.h"

float	CAutoWall::GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 1.f;
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_CHEST:
		return 1.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTARM:
		return 1.f;
	case HITGROUP_RIGHTARM:
		return 1.f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 1.f;
	default:
		break;
	}
	return 1.f;
}

bool	CAutoWall::IsArmored(CBaseEntity* Entity, int ArmorValue, int Hitgroup)
{
	bool result = false;

	if (ArmorValue > 0)
	{
		switch (Hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			result = true;
			break;
		case HITGROUP_HEAD:
			result = Entity->HasHelmet();
			break;
		}
	}

	return result;
}

void	CAutoWall::ScaleDamage(int Hitgroup, CBaseEntity* Entity, float WeaponArmorRatio, float& Damage)
{
	int HeavyArmor = Entity->HasHelmet();
	int ArmorValue = Entity->GetArmor(); 

	switch (Hitgroup)
	{
	case HITGROUP_HEAD:
		if (HeavyArmor)
			Damage = (Damage * 4.f) * 0.5f;
		else
			Damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		Damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		Damage *= 0.75f;
		break;
	}

	if (IsArmored(Entity, ArmorValue, Hitgroup))
	{
		float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = WeaponArmorRatio * 0.5f;

		if (HeavyArmor)
		{
			ArmorBonusRatio = 0.33f;
			ArmorRatio = (WeaponArmorRatio * 0.5f) * 0.5f;
			v47 = 0.33f;
		}

		auto NewDamage = Damage * ArmorRatio;

		if (HeavyArmor)
			NewDamage *= 0.85f;

		if (((Damage - (Damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > ArmorValue)
			NewDamage = Damage - (ArmorValue / ArmorBonusRatio);

		Damage = NewDamage;
	}
}

bool	CAutoWall::IsBreakable(CBaseEntity* entity) {
	if (!entity || !entity->index)
		return false;

	// backup original take_damage value.
	const int take_damage = entity->GetTakeDamage();

	auto client_class = entity->GetIClientNetworkable()->GetClientClass();
	if (!client_class)
		return false;

	// convert to string since std::string::at throws std::out_of_range when pos > length
	std::string network_name = client_class->m_pNetworkName;

		if (network_name.at(1) == 'B' && network_name.at(9) == 'e' && network_name.at(10) == 'S' && network_name.at(16) == 'e')
			entity->GetTakeDamage() = 2;

		else if (network_name.at(1) != 'B' && network_name.at(5) != 'D')
			entity->GetTakeDamage() = 2;

		else if (network_name.at(1) != 'F' && network_name.at(4) != 'c' && network_name.at(5) != 'B' && network_name.at(9) != 'h') // CFuncBrush
			entity->GetTakeDamage() = 2;


	using IsBreakableEntity_t = bool(__thiscall*)(CBaseEntity*);
	static IsBreakableEntity_t IsBreakableEntityEx = nullptr;

	if (!IsBreakableEntityEx)
		IsBreakableEntityEx = (IsBreakableEntity_t)U::FindPattern("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");

	entity->GetTakeDamage() = take_damage;

	return IsBreakableEntityEx(entity);
}

bool	CAutoWall::TraceToExit(Vector& end, const Vector& start, const Vector& dir, trace_t* enter_trace, trace_t* exit_trace)
{
	float dist = 0.f;

	while (dist <= 90.f) {
		dist += 4.f;
		end = start + dir * dist;

		const int point_contents = I::EngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		Vector a1 = end - dir * 4.f;
		I::EngineTrace->TraceRay(Ray_t{ end, a1 }, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
			c_trace_filter_skip_entity filter(exit_trace->m_pEnt);
			I::EngineTrace->TraceRay(Ray_t{ end, start }, MASK_SHOT_HULL, &filter, exit_trace);

			if ((exit_trace->fraction < 1.f || exit_trace->allsolid) && !exit_trace->startsolid) {
				end = exit_trace->endpos;
				return true;
			}

			continue;
		}

		if (!exit_trace->DidHit() || exit_trace->startsolid) {
			if (enter_trace->m_pEnt && (enter_trace->m_pEnt != nullptr && enter_trace->m_pEnt !=I::ClientEntList->GetClientEntity(0))) {
				*exit_trace = *enter_trace;
				exit_trace->endpos = start + dir;
				return true;
			}

			continue;
		}

		if (!exit_trace->DidHit() || exit_trace->startsolid) {
			if (enter_trace->m_pEnt != nullptr && !enter_trace->m_pEnt->index == 0 && IsBreakable(enter_trace->m_pEnt)) {
				*exit_trace = *enter_trace;
				exit_trace->endpos = start + dir;
				return true;
			}

			continue;
		}

		if (exit_trace->surface.flags >> 7 & SURF_LIGHT && !(enter_trace->surface.flags >> 7 & SURF_LIGHT))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.f) {
			end = end - dir * (exit_trace->fraction * 4.f);
			return true;
		}
	}

	return false;
}

bool	CAutoWall::HandleBulletPenetration(FireBulletData& data)
{
	surfacedata_t* enter_surface_data = I::PhysicsSurfaceProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	const unsigned short enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)(pow(data.wpn_data->flRangeModifier, (data.trace_length * 0.002)));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;
	bool a5 = data.enter_trace.contents >> 3 & CONTENTS_SOLID;
	bool v19 = data.enter_trace.surface.flags >> 7 & SURF_LIGHT;

	if(!TraceToExit(dummy, data.enter_trace.endpos, data.direction, &data.enter_trace, &trace_exit))
		return false;

	if (data.enter_trace.surface.name)
		m_enter_material_name = data.enter_trace.surface.name;

	if (trace_exit.surface.name)
		m_exit_material_name = trace_exit.surface.name;

	if (m_enter_material_name.find("TELA") != std::string::npos && m_exit_material_name.find("ITALY") != std::string::npos)
		return false;

	surfacedata_t* exit_surface_data = I::PhysicsSurfaceProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	float damage_modifier = 0.16f;
	float average_penetration_modifier = (enter_surface_data->game.flPenetrationModifier + exit_surface_data->game.flPenetrationModifier) / 2;

	if (enter_material == 71 || enter_material == 89) {
		damage_modifier = 0.05f;
		average_penetration_modifier = 3.f;
	}
	else if (a5 || v19) {
		damage_modifier = 0.16f;
		average_penetration_modifier = 1.f;
	}

	if (enter_material == exit_surface_data->game.material) {
		if (exit_surface_data->game.material == 87 || exit_surface_data->game.material == 85)
			average_penetration_modifier = 3.f;
		if (exit_surface_data->game.material == 76)
			average_penetration_modifier = 2.f;
	}


	const float modifier = std::fmaxf(0.f, 1.f / average_penetration_modifier);
	const float pen_len = (trace_exit.endpos - data.enter_trace.endpos).Length();
	const float lost_dmg = modifier * 3.f * std::fmaxf(0.f, 3.f / data.wpn_data->flPenetration * 1.25f) + data.current_damage * damage_modifier + pen_len * pen_len * modifier / 24.f;

	data.current_damage -= std::fmaxf(0.f, lost_dmg);
	if (data.current_damage < 1.f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;

}

void	CAutoWall::TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;
	I::EngineTrace->TraceRay(ray, mask, &filter, ptr);
}

void	CAutoWall::ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr, FireBulletData& data)
{

	trace_t trace;
	Ray_t ray;
	float this_fraction = tr->fraction;
	
	ray.Init(vecAbsStart, vecAbsEnd);

	if (filter && filter->ShouldHitEntity(data.entity, mask) == false)
		return;

	I::EngineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, data.entity, &trace);

	if (trace.fraction < this_fraction)
	{
		*tr = trace;
		this_fraction = trace.fraction;
	}
}

bool	CAutoWall::SimulateFireBullet(FireBulletData& data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;

	if (data.wpn_data == NULL)
		return false;

	data.current_damage = (float)data.wpn_data->iDamage;
	trace_t trace;
	c_trace_filter_skip_entity filter(G::LocalPlayer);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = data.wpn_data->flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		TraceLine(data.src, end, 0x4600400B, G::LocalPlayer, &data.enter_trace);
		ClipTraceToPlayers(data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace,data);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if (data.enter_trace.hitgroup > HITGROUP_GENERIC && data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG && 
			data.enter_trace.m_pEnt != nullptr && data.entity == data.enter_trace.m_pEnt) 
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= (float)pow(data.wpn_data->flRangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, data.wpn_data->flArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(data))
			break;
	}

	return false;
}

bool CAutoWall::SimulateFireBulletRage(FireBulletData& data, CBaseEntity* player, int hitbox)
{
	auto GetHitgroup = [](int hitbox) -> int
	{
		switch (hitbox)
		{
		case HITBOX_HEAD:
		case HITBOX_NECK:
			return HITGROUP_HEAD;
		case HITBOX_LOWER_CHEST:
		case HITBOX_CHEST:
		case HITBOX_UPPER_CHEST:
			return HITGROUP_CHEST;
		case HITBOX_STOMACH:
		case HITBOX_PELVIS:
			return HITGROUP_STOMACH;
		case HITBOX_LEFT_HAND:
		case HITBOX_LEFT_UPPER_ARM:
		case HITBOX_LEFT_FOREARM:
			return HITGROUP_LEFTARM;
		case HITBOX_RIGHT_HAND:
		case HITBOX_RIGHT_UPPER_ARM:
		case HITBOX_RIGHT_FOREARM:
			return HITGROUP_RIGHTARM;
		case HITBOX_LEFT_THIGH:
		case HITBOX_LEFT_CALF:
		case HITBOX_LEFT_FOOT:
			return HITGROUP_LEFTLEG;
		case HITBOX_RIGHT_THIGH:
		case HITBOX_RIGHT_CALF:
		case HITBOX_RIGHT_FOOT:
			return HITGROUP_RIGHTLEG;
		default:
			return -1;
		}
	};
	data.penetrate_count = 4;
	data.trace_length = 0.0f;

	if (data.wpn_data == NULL)
		return false;

	data.current_damage = (float)data.wpn_data->iDamage;
	trace_t trace;
	c_trace_filter_skip_entity filter(G::LocalPlayer);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = data.wpn_data->flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		TraceLine(data.src, end, 0x4600400B, G::LocalPlayer, &data.enter_trace);
		ClipTraceToPlayers(data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace, data);

		if (data.enter_trace.fraction == 1.0f)
		{
			if (player && !(player->GetFlags() & FL_ONGROUND))
			{
				data.enter_trace.hitgroup = GetHitgroup(hitbox);
				data.enter_trace.m_pEnt = player;
			}
			else
				break;
		}

		if (data.enter_trace.hitgroup > HITGROUP_GENERIC && data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG &&
			data.enter_trace.m_pEnt != nullptr && data.entity == data.enter_trace.m_pEnt)
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= (float)pow(data.wpn_data->flRangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, data.wpn_data->flArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(data))
			break;
	}

	return false;


}


float	CAutoWall::GetDamage(const Vector& point, CBaseEntity* ent)
{
	float damage = 0.f;
	Vector dst = point;
	FireBulletData data;
	data.src = G::LocalPlayer->GetEyePosition();
	data.wpn_data = G::LocalPlayer->GetWeapon()->GetCSWpnData();
	data.filter.pSkip = G::LocalPlayer;
	data.entity = ent;
	QAngle angles = math::CalcAngle(data.src, dst);
	math::AngleVectors(angles, data.direction);
	Vector dataNormalized;

	data.direction.Normalize();

	if (SimulateFireBullet(data))
	{
		damage = data.current_damage;
	}

	return damage;
}

float CAutoWall::GetDamageRage(const Vector& vecPoint, CBaseEntity* player, int hitbox) {
	
	float damage = 0.f;
	Vector dst = vecPoint;
	FireBulletData data;
	data.src = G::LocalPlayer->GetEyePosition();
	data.wpn_data = G::LocalPlayer->GetWeapon()->GetCSWpnData();
	data.filter.pSkip = G::LocalPlayer;
	data.entity = player;
	QAngle angles = math::CalcAngle(data.src, dst);
	math::AngleVectors(angles, data.direction);
	Vector dataNormalized;

	data.direction.Normalize();

	if (SimulateFireBulletRage(data, player, hitbox))
		damage = data.current_damage;

	return damage;
}