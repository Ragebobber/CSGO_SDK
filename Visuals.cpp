#include "Cheat.h"

void CVisuals::Draw() {
	if (!Vars.Visuals.vEnable || !inGame()) return;
	if (Vars.Visuals.vDebug) Debug();
	int idx = I::ClientEntList->GetHighestEntityIndex();
	for (int i : boost::irange(0, idx))
		doEsp(i);
}

bool CVisuals::inGame() {
	if (!I::Engine->is_ingame() || !G::LocalPlayer) return false;
	else return true;
}

void CVisuals::doEsp(int idx) {

	CBaseEntity* Entity = I::ClientEntList->GetClientEntity(idx);
	
	if (!Entity) return;
	player_info_t playerInfo = { };

	if (!I::Engine->GetPlayerInfo(Entity->GetIClientNetworkable()->get_index(), &playerInfo)) return;
	if (EntityIsInvalid(Entity,idx)) return;
	if(!GetBox(Entity,x,y,w,h)) return;
	if (!Vars.Visuals.Filter.ShowTeam && !Entity->IsEnemy()) return;
	if (Vars.Visuals.vEsp) {
		PlayerBox(Entity, x,y,w,h, Entity->isVisible() ? (Entity->IsEnemy() ?

			Color(int(Vars.Visuals.esp_player_bbox_color_ct_visible[0] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_ct_visible[1] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_ct_visible[2] * 255.f))
			:
			Color(int(Vars.Visuals.esp_player_bbox_color_t_visible[0] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_t_visible[1] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_t_visible[2] * 255.f))) :

			(Entity->IsEnemy() ? Color(int(Vars.Visuals.esp_player_bbox_color_ct[0] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_ct[1] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_ct[2] * 255.f))
				: Color(int(Vars.Visuals.esp_player_bbox_color_t[0] * 255.f),
					int(Vars.Visuals.esp_player_bbox_color_t[1] * 255.f),
					int(Vars.Visuals.esp_player_bbox_color_t[2] * 255.f))));
	}
	if (Vars.Visuals.vHead_dot)
		HeadDot(Entity);
	if (Vars.Visuals.vHealth)
		Health(Entity, x, y, w, h, Entity->GetHealth());
	if (Vars.Visuals.vArmor)
		ArmorBar(Entity, x, y + 10, w, h, Entity->GetArmor());
	if (Vars.Visuals.vShowName)
	{
		// get player name
		std::string szName = playerInfo.name;

		// truncate name
		if (szName.length() > 24U)
			szName = szName.substr(0U, 24U).append(("..."));
		D::DrawString(F::ESP, x + w + 11, y + 4, Color::White(), FONT_LEFT, "%s", szName.c_str());
	}
}

void CVisuals::HeadDot(CBaseEntity* ent) {
	if (!ent) return;
	Vector head2D;
	Vector head3D = ent->GetBonePosition(8);
	if (!D::WorldToScreen(Vector(head3D.x, head3D.y, head3D.z), head2D))
		return;
	D::DrawCircle((int)head2D.x, (int)head2D.y, 2, 30, ent->isVisible() ? (ent->IsEnemy() ? Color(int(Vars.Visuals.esp_player_bbox_color_ct_visible[0] * 255.f),
		int(Vars.Visuals.esp_player_bbox_color_ct_visible[1] * 255.f),
		int(Vars.Visuals.esp_player_bbox_color_ct_visible[2] * 255.f))
		:
		Color(int(Vars.Visuals.esp_player_bbox_color_t_visible[0] * 255.f),
			int(Vars.Visuals.esp_player_bbox_color_t_visible[1] * 255.f),
			int(Vars.Visuals.esp_player_bbox_color_t_visible[2] * 255.f))) :

		(ent->IsEnemy() ? Color(int(Vars.Visuals.esp_player_bbox_color_ct[0] * 255.f),
			int(Vars.Visuals.esp_player_bbox_color_ct[1] * 255.f),
			int(Vars.Visuals.esp_player_bbox_color_ct[2] * 255.f))
			: Color(int(Vars.Visuals.esp_player_bbox_color_t[0] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_t[1] * 255.f),
				int(Vars.Visuals.esp_player_bbox_color_t[2] * 255.f))));
}

bool CVisuals::EntityIsInvalid(CBaseEntity* Entity,int idx)
{
	if (!Entity)
		return true;

	CBaseEntity* observerTarget = I::ClientEntList->GetClientEntityFromHandle(G::LocalPlayer->GetObserverTargetHandle());

	if (Entity == observerTarget)
		return true;
	if (Entity->GetHealth() <= 0)
		return true;
	if (Entity == G::LocalPlayer) 
		return true;
	if (Entity->GetDormant())
		return true;

	return false;
}

bool CVisuals::GetBox(CBaseEntity* entity, int& x, int& y, int& w, int& h)
{
	Vector vOrigin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	vOrigin = entity->GetOrigin();
	min = entity->GetCollideable()->obb_mins() + vOrigin;
	max = entity->GetCollideable()->obb_maxs() + vOrigin;

	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	if (!D::WorldToScreen(points[3], flb) || !D::WorldToScreen(points[5], brt)
		|| !D::WorldToScreen(points[0], blb) || !D::WorldToScreen(points[4], frt)
		|| !D::WorldToScreen(points[2], frb) || !D::WorldToScreen(points[1], brb)
		|| !D::WorldToScreen(points[6], blt) || !D::WorldToScreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	x = (int)left;
	y = (int)top;
	w = (int)(right - left);
	h = (int)(bottom - top);

	return true;

}

void CVisuals::PlayerBox(CBaseEntity* pEntityPlayer, int x, int y, int w, int h, Color clr)
{
			D::DrawOutlinedRect(x, y, w, h, clr);
			D::DrawOutlinedRect(x - 1, y - 1, w + 2, h + 2, Color(0, 0, 0));
			D::DrawOutlinedRect(x + 1, y + 1, w - 2, h - 2, Color(0, 0, 0));
}


void CVisuals::Health(CBaseEntity* player, int x, int y, int w, int h, int health) {
			int hp = health;
			if (hp > 100)
				hp = 100;

			int Width = (w * (hp / 100));

			int Red = (255 - (hp * 3));
			int Green = (hp * 3);


		D::DrawRect(x - 1, y + h + 1, Width + 2, 5, Color(10, 10, 10, 150));
		D::DrawRect(x, y + h + 2, Width, 3, Color(Red, Green, 0, 255));
		D::DrawString(F::Watermark, x + Width, y + h + 4, Color(255, 255, 255), FONT_LEFT, "%i", health);
}

void CVisuals::ArmorBar(CBaseEntity* player, int x, int y, int w, int h, int armor)
{
		int hp = armor;
		if (hp > 100)
			hp = 100;
		int Width = (w * (hp / 100));
		D::DrawRect(x - 1, y + h + 1, Width + 2, 5, Color(10, 10, 10, 150));
		D::DrawRect(x, y + h + 2, Width, 3, Color(0, 0, 255, 255));
		D::DrawString(F::Watermark, x + Width, y + h + 4, Color(255, 255, 255), FONT_LEFT, "%i", armor);
}

void CVisuals::Debug() {
	int y = 18 + 7;
	D::DrawString(F::Watermark, 5, y, Color::White(), FONT_LEFT, "Blatant_hack Debug section");
	D::DrawString(F::Watermark, 5, y*2, Color::White(), FONT_LEFT, "Local = 0x%X",G::LocalPlayer);
	//D::DrawString(F::Watermark, 5, y * 3, Color::White(), FONT_LEFT, "Hit_chance = %.1f", G::LocalPlayer->HitChance(G::LocalPlayer));
	D::DrawString(F::Watermark, 5, y * 2, Color::White(), FONT_LEFT, "GetHealth %.i", G::LocalPlayer->GetHealth());

}