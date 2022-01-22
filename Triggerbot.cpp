#include "Cheat.h"

void CTriggerbot::do_Triggerbot() {
	if (!I::Engine->is_ingame() || !G::LocalPlayer->GetAlive() || !G::LocalPlayer)
		return;
	if (!G::PressedKeys[Vars.Triggerbot.tKey])
		return;
	Vector rem, forward, src = G::LocalPlayer->GetEyePosition();
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = G::LocalPlayer;

	QAngle viewangles = G::UserCmd->view_angles;

	viewangles += G::LocalPlayer->GetPunch() * 2.f;

	math::AngleVectors(viewangles, forward);

	forward *= G::LocalPlayer->GetWeapon()->GetCSWpnData()->flRange;

	rem = src + forward;

	if (Vars.Triggerbot.t_SmokeCheck && U::LineGoesThroughSmoke(src, rem))
		return;

	ray.Init(src, rem);

	I::EngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

	if (!tr.m_pEnt)
		return;

	bool dh = false;
	if (Vars.Triggerbot.Filter.Head && tr.hitgroup == HITGROUP_HEAD)
		dh = true;
	if (Vars.Triggerbot.Filter.Chest && tr.hitgroup == HITGROUP_CHEST)
		dh = true;
	if (Vars.Triggerbot.Filter.Stomach && tr.hitgroup == HITGROUP_STOMACH)
		dh = true;
	if (Vars.Triggerbot.Filter.Arms && (tr.hitgroup == HITGROUP_LEFTARM || tr.hitgroup == HITGROUP_RIGHTARM))
		dh = true;
	if (Vars.Triggerbot.Filter.Legs && (tr.hitgroup == HITGROUP_LEFTLEG || tr.hitgroup == HITGROUP_RIGHTLEG))
		dh = true;

	auto player = tr.m_pEnt;

	
	if (player && !player->GetDormant() && player->GetHealth() > 0 && player->IsPlayer() && G::LocalPlayer->HitChance(player, viewangles) >= Vars.Triggerbot.t_hChance)
	{
		if (player->GetTeam() != G::LocalPlayer->GetTeam())
		{
				if (dh && (!(G::UserCmd->buttons & in_attack) && !I::InputSystem->IsButtonDown(::MOUSE_LEFT))) // if you don't attack currently
						G::UserCmd->buttons |= in_attack;

				static bool already_shot = false;
				if (G::LocalPlayer->GetWeapon()->IsPistol())
				{
					if (G::UserCmd->buttons & in_attack)
						if (already_shot)
							G::UserCmd->buttons &= ~in_attack;

					already_shot = G::UserCmd->buttons & in_attack ? true : false;
				}
			
		}
	}
}
