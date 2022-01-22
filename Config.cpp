#include "Cheat.h"

inline char* strcats(char* s1, char* s2)
{
	std::string ss1(s1);
	std::string ss2(s2);
	std::string Buff = ss1 + ss2;
	char* Buffer = _strdup(Buff.c_str());
	return Buffer;
}

const char* GetWeaponName(int iWeaponID)
{
	switch (iWeaponID)
	{
	default:
		return "none";
	case WEAPON_DEAGLE:
		return "deagle";
	case WEAPON_ELITE:
		return "duals";
	case WEAPON_FIVESEVEN:
		return "five7";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_AUG:
		return "aug";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_GALILAR:
		return "galil";
	case WEAPON_M249:
		return "m249";
	case WEAPON_M4A1:
		return "m4a4";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_P90:
		return "p90";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_TASER:
		return "taser";
	case WEAPON_USP_SILENCER:
		return "usp-s";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_P250:
		return "p250";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_SG556:
		return "sg556";
	case WEAPON_SSG08:
		return "ssg08";
	case WEAPON_M4A1_SILENCER:
		return "m4a1-s";
	case WEAPON_HKP2000:
		return "p2000";
	case WEAPON_CZ75A:
		return "cz75";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_BUTTERFLY:
		return "butterfly";
	case WEAPON_KNIFE_FALCHION:
		return "falshion";
	case WEAPON_KNIFE_FLIP:
		return "flip";
	case WEAPON_KNIFE_GUT:
		return "gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "karambit";
	case WEAPON_KNIFE_M9_BAYONET:
		return "m9";
	case WEAPON_KNIFE_TACTICAL:
		return "huntsman";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "bowie";
	case WEAPON_KNIFE_PUSH:
		return "daggers";
	case WEAPON_KNIFE_URSUS:
		return "URSUS";
	case WEAPON_KNIFE_STILETTO:
		return "STILETTO";
	case WEAPON_KNIFE_WIDOWMAKER:
		return "WIDOWMAKER";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return "Navaja";
	case WEAPON_MP5SD:
		return "MP5-SD";
	}
}

void CConfig::Setup()
{
	//triggerbot
	SetupValue(Vars.Triggerbot.tEnable, false, ("Triggerbot"), ("Enabled"));
	SetupValue(Vars.Triggerbot.t_SmokeCheck, false, ("Triggerbot"), ("t_SmokeCheck"));
	SetupValue(Vars.Triggerbot.Filter.Arms, false, ("Triggerbot"), ("Arms"));
	SetupValue(Vars.Triggerbot.Filter.Chest, false, ("Triggerbot"), ("Chest"));
	SetupValue(Vars.Triggerbot.Filter.Head, false, ("Triggerbot"), ("Head"));
	SetupValue(Vars.Triggerbot.Filter.Legs, false, ("Triggerbot"), ("Legs"));
	SetupValue(Vars.Triggerbot.Filter.Stomach, false, ("Triggerbot"), ("Stomach"));
	SetupValue(Vars.Triggerbot.tKey, 0, ("Triggerbot"), ("Key"));
	SetupValue(Vars.Triggerbot.t_hChance, 20.f, ("Triggerbot"), ("Chance"));

	//visuals
	SetupValue(Vars.Visuals.vEnable, false, ("Visuals"), ("Enabled"));
	SetupValue(Vars.Visuals.vEsp, false, ("Visuals"), ("vEsp"));
	SetupValue(Vars.Visuals.vHead_dot, false, ("Visuals"), ("vHead_dot"));
	SetupValue(Vars.Visuals.vHealth, false, ("Visuals"), ("vHealth"));
	SetupValue(Vars.Visuals.vArmor, false, ("Visuals"), ("vArmor"));
	SetupValue(Vars.Visuals.vShowName, false, ("Visuals"), ("vShowName"));
	SetupValue(Vars.Visuals.Filter.ShowTeam, false, ("Visuals"), ("ShowTeam"));
	
	SetupValue(Vars.Visuals.vOverrideFov, false, ("Visuals"), ("vOverrideFov"));
	SetupValue(Vars.Visuals.vOverrideFovf, 0, ("Visuals"), ("OverrideViewmodelFov"));
	SetupValue(Vars.Visuals.vOverrideViewf, 10, ("Visuals"), ("ViewmodelFov"));
	//aimbot
	SetupValue(Vars.Legitbot.aEnable, false, ("Legitbot"), ("Enabled"));
	SetupValue(Vars.Legitbot.aOnKey, false, ("Legitbot"), ("aOnKey"));
	SetupValue(Vars.Legitbot.aKey, 0, ("Legitbot"), ("Key"));

	for (int id = 0; id < 64; id++) {
		SetupValue(Vars.Legitbot.Weapon[id].Hitbox, 9, GetWeaponName(id), ("Hitbox"));
		SetupValue(Vars.Legitbot.Weapon[id].Speed, 30.f, GetWeaponName(id), ("Smooth"));
		SetupValue(Vars.Legitbot.Weapon[id].FOV, 2.f, GetWeaponName(id), ("FOV"));
		SetupValue(Vars.Legitbot.Weapon[id].RCSAmountX, 0.f, GetWeaponName(id), ("RCSAmountX"));
		SetupValue(Vars.Legitbot.Weapon[id].RCSAmountY, 0.f, GetWeaponName(id), ("RCSAmountY"));
		SetupValue(Vars.Legitbot.Weapon[id].autowall, false, GetWeaponName(id),("Autowall"));
		SetupValue(Vars.Legitbot.Weapon[id].mindamage, 0.f, GetWeaponName(id), ("MinDamage"));
		SetupValue(Vars.Legitbot.Weapon[id].pSilent, false, GetWeaponName(id), ("pSilent"));
		SetupValue(Vars.Legitbot.Weapon[id].PFOV, 2.f, GetWeaponName(id), ("pFOV"));
	}

	//colors
	SetupValue(Vars.Visuals.esp_player_bbox_color_t[0], 0.0f, ("Colors"), ("ColorTeam.R"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_t[1], 0.0f, ("Colors"), ("ColorTeam.G"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_t[2], 0.0f, ("Colors"), ("ColorTeam.B"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_t_visible[0], 0.0f, ("Colors"), ("ColorTeamVisible.R"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_t_visible[1], 0.0f, ("Colors"), ("ColorTeamVisible.G"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_t_visible[2], 0.0f, ("Colors"), ("ColorTeamVisible.B"));

	SetupValue(Vars.Visuals.esp_player_bbox_color_ct[0], 0.0f, ("Colors"), ("ColorEnemy.R"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_ct[1], 0.0f, ("Colors"), ("ColorEnemy.G"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_ct[2], 0.0f, ("Colors"), ("ColorEnemy.B"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_ct_visible[0], 0.0f, ("Colors"), ("ColorEnemyVisible.R"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_ct_visible[1], 0.0f, ("Colors"), ("ColorEnemyVisible.G"));
	SetupValue(Vars.Visuals.esp_player_bbox_color_ct_visible[2], 0.0f, ("Colors"), ("ColorEnemyVisible.B"));

	//misc
	SetupValue(Vars.Misc.mBhop, false, ("Misc"), ("mBhop"));
	SetupValue(Vars.Misc.mAStrafe, false, ("Misc"), ("mAStrafe"));
}

void CConfig::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\BlatantHack\\");

		char bufcfg[350];

		switch (Vars.Misc.curcfg)
		{
		case 0:
			sprintf_s(bufcfg, "\\BlatantHack\\MM.bh");
			break;
		case 1:
			sprintf_s(bufcfg, "\\BlatantHack\\Legit.bh");
			break;
		case 2:
			sprintf_s(bufcfg, "\\BlatantHack\\MMHvH.bh");
			break;
		case 3:
			sprintf_s(bufcfg, "\\BlatantHack\\Rage.bh");
			break;
		case 4:
			sprintf_s(bufcfg, "\\BlatantHack\\Public.bh");
			break;
		default:
			sprintf_s(bufcfg, "\\BlatantHack\\Error.bh");
			break;
		}


		file = std::string(path) + bufcfg;
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());



}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, 0x001a, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\BlatantHack\\");

		char bufcfg[350];

		switch (Vars.Misc.curcfg)
		{
		case 0:
			sprintf_s(bufcfg, "\\BlatantHack\\MM.bh");
			break;
		case 1:
			sprintf_s(bufcfg, "\\BlatantHack\\Legit.bh");
			break;
		case 2:
			sprintf_s(bufcfg, "\\BlatantHack\\MMHvH.bh");
			break;
		case 3:
			sprintf_s(bufcfg, "\\BlatantHack\\Rage.bh");
			break;
		case 4:
			sprintf_s(bufcfg, "\\BlatantHack\\Public.bh");
			break;
		default:
			sprintf_s(bufcfg, "\\BlatantHack\\Error.bh");
			break;
		}


		file = std::string(path) + bufcfg;
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = (float)atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}


CConfig* Config = new CConfig();
Variables Vars;