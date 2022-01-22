#pragma once

class CVisuals
{
public:
	void Draw();
private:
	bool inGame();
	void doEsp(int entIndex);
	void HeadDot(CBaseEntity* ent);
	bool EntityIsInvalid(CBaseEntity* Entity, int idx);
	bool GetBox(CBaseEntity* entity, int& x, int& y, int& w, int& h);
	void PlayerBox(CBaseEntity* pEntityPlayer, int x, int y, int w, int h, Color clr);
	void Health(CBaseEntity* player, int x, int y, int w, int h, int health);
	void ArmorBar(CBaseEntity* player,int x,int y,int w,int h, int armor);
	void Debug();
	int x, y, w, h;
};