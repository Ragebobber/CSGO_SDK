#include "Cheat.h"
#include "helper.h"

namespace ImGui {
	const char* KKeyNames[] = {
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

	};
	
	bool ColorButton(const char* desc_id, const float col[], ImGuiColorEditFlags flags, ImVec2 size)
	{
		return ImGui::ColorButton(desc_id, ImColor(col[0], col[1], col[2], col[3]), flags, size);
	}
	ImGuiID Colorpicker_Close = 0;
	__inline void CloseLeftoverPicker() { if (Colorpicker_Close) ImGui::CloseCurrentPopup(); }
	void ColorPickerBox(const char* picker_idname, float col_ct[], float col_t[], float col_ct_invis[], float col_t_invis[], bool alpha = true)
	{
		ImGui::SameLine();
		static bool switch_entity_teams = false;
		static bool switch_color_vis = false;
		bool open_popup = ImGui::ColorButton(picker_idname, switch_entity_teams ? (switch_color_vis ? col_t : col_t_invis) : (switch_color_vis ? col_ct : col_ct_invis), ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip, ImVec2(36, 0));
		if (open_popup)
		{
			ImGui::OpenPopup(picker_idname);
			Colorpicker_Close = ImGui::GetID(picker_idname);
		}

		if (ImGui::BeginPopup(picker_idname))
		{
			const char* button_name0 = switch_entity_teams ? "Teammates" : "Enemies";
			if (ImGui::Button(button_name0, ImVec2(-1, 0)))
				switch_entity_teams = !switch_entity_teams;

			const char* button_name1 = switch_color_vis ? "Invisible" : "Visible";
			if (ImGui::Button(button_name1, ImVec2(-1, 0)))
				switch_color_vis = !switch_color_vis;

			std::string id_new = picker_idname;
			id_new += "##pickeritself_";

			ImGui::ColorPicker4(id_new.c_str(), switch_entity_teams ? (switch_color_vis ? col_t : col_t_invis) : (switch_color_vis ? col_ct : col_ct_invis), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_PickerHueBar | (alpha ? ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar : 0));
			ImGui::EndPopup();
		}
	}
	bool Hotkey(const char* label, int* k, const ImVec2& size_arg)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;

		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);
		ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
		const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
		const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id, &frame_bb, ImGuiItemAddFlags_Focusable))
			return false;

		const bool hovered = ItemHoverable(frame_bb, id);
		if (hovered) 
			g.MouseCursor = ImGuiMouseCursor_TextInput;
		
		const bool focus_requested_by_code = (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByCode) != 0;
		const bool focus_requested_by_tabbing = (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;

		const bool user_clicked = hovered && io.MouseClicked[0];

		if (focus_requested_by_code || user_clicked)
		{
			if (g.ActiveId != id) {
				memset(io.MouseDown, 0, sizeof(io.MouseDown));
				memset(io.KeysDown, 0, sizeof(io.KeysDown));
				*k = 0;
			}
			SetActiveID(id, window);
			SetFocusID(id, window);
			FocusWindow(window);
		}
		else if (io.MouseClicked[0]) {
			if (g.ActiveId == id)
				ClearActiveID();
		}

		bool value_changed = false;
		int key = *k;

		if (g.ActiveId == id) {
			for (auto i = 0; i < 5; i++) {
				if (io.MouseDown[i]) {
					switch (i) {
					case 0:
						key = 0x01;
						break;
					case 1:
						key = 0x02;
						break;
					case 2:
						key = 0x04;
						break;
					case 3:
						key = 0x05;
						break;
					case 4:
						key = 0x06;
						break;
					}
					value_changed = true;
					ClearActiveID();
				}
			}
			if (!value_changed) {
				for (auto i = 0x08; i <= 0xA5; i++) {
					if (io.KeysDown[i]) {
						key = i;
						value_changed = true;
						ClearActiveID();
					}
				}
			}

			if (IsKeyPressedMap(ImGuiKey_Escape)) {
				*k = 0;
				ClearActiveID();
			}
			else {
				*k = key;
			}
		}

		// Render
		// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

		char buf_display[64] = "None";

		RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

		if (*k != 0 && g.ActiveId != id) {
			strcpy_s(buf_display, KKeyNames[*k]);
		}
		else if (g.ActiveId == id) {
			strcpy_s(buf_display, "<Press a key>");
		}

		const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
		ImVec2 render_pos = frame_bb.Max + style.FramePadding;
		RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL);
		//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

		if (label_size.x > 0)
			RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

		return value_changed;
	}
}
const char* opt_MultiHitboxes[14] = { "Head", "Pelvis", "Upper Chest", "Chest", "Neck", "Left Forearm", "Right Forearm", "Hands", "Left Thigh", "Right Thigh", "Left Calf", "Right Calf", "Left Foot", "Right Foot" };
const char* opt_AimHitboxSpot[] = { "Head", "Neck", "Body", "Pelvis" };
namespace gui {
	void guiinit(IDirect3DDevice9* pDevice) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplWin32_Init(G::Window);
		ImGui_ImplDX9_Init(pDevice);

		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(8, 8);
		style.WindowRounding = 0.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.FramePadding = ImVec2(4, 3);
		style.FrameRounding = 0.0f;
		style.ItemSpacing = ImVec2(8, 4);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 10.0f;
		style.ScrollbarRounding = 3.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		style.CurveTessellationTol = 1.25f;
		style.AntiAliasedLines = true;
		style.CurveTessellationTol = 1.25f;

		G::d3dinit = true;
	}
	void Aimbot() {
		static bool trigger_tab = false;
		static bool aim_tab = true;
		static bool ragebot = false;
		ImGui::Text("Legitbot");
		ImGui::Separator();
		if (ImGui::Button("LegitBot", ImVec2(415, 25)))
		{trigger_tab = false; aim_tab = true; ragebot = false;}
		ImGui::SameLine();
		if (ImGui::Button("Triggerbot", ImVec2(415, 25))) { aim_tab = false; trigger_tab = true; ragebot = false;}
		ImGui::Separator();
		if (ImGui::Button("RageBot", ImVec2(415, 25))) { aim_tab = false; trigger_tab = false; ragebot = true; }
		if (trigger_tab) 
		{
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Triggerbot");
				ImGui::Checkbox(("Trigger enable"), &Vars.Triggerbot.tEnable);
				ImGui::Hotkey("Trigger hot key ##Aimbot Key", &Vars.Triggerbot.tKey, ImVec2(220, 20));
				ImGui::SliderFloat(("Trigger HitChance"), &Vars.Triggerbot.t_hChance, 0.1f, 100.f, "%.1f");
				ImGui::Checkbox(("Trigger SmokeCheck"), &Vars.Triggerbot.t_SmokeCheck);

			}
			ImGui::NextColumn();
			{
				ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 6),true);
				{
					ImGui::Selectable(("Trigger  On Head"), &Vars.Triggerbot.Filter.Head);
					ImGui::Selectable(("Trigger  On Chest"), &Vars.Triggerbot.Filter.Chest);
					ImGui::Selectable(("Trigger  On Stomach"), &Vars.Triggerbot.Filter.Stomach);
					ImGui::Selectable(("Trigger  On Arms"), &Vars.Triggerbot.Filter.Arms);
					ImGui::Selectable(("Trigger  On Legs"), &Vars.Triggerbot.Filter.Legs);
				}
				ImGui::EndChild();
			}
			ImGui::Columns(1);
		}
		if (aim_tab)
		{
			ImGui::Columns(2, NULL, false);
			{
				ImGui::BeginChild("COL1", ImVec2(0, 0), true);
				{
					ImGui::Text("LegitbotSettings");
					ImGui::Separator();
					ImGui::Checkbox(("Enable Legit Aimbot"), &Vars.Legitbot.aEnable);
					ImGui::Checkbox(("Aim on Key"), &Vars.Legitbot.aOnKey);
					if (Vars.Legitbot.aOnKey) {
						ImGui::SameLine();
						ImGui::Hotkey("Key##Aimbot Key", &Vars.Legitbot.aKey, ImVec2(100, 20));
					}
					ImGui::Checkbox(("Friendly Fire"), &Vars.Legitbot.aFriendlyFire);	
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				{
					ImGui::BeginChild("COL2", ImVec2(0, 0), true);
					{
						ImGui::Text("WeaponSettings");
						ImGui::Separator();
						if (G::LocalPlayer->GetAlive() && G::LocalPlayer->GetWeapon()->IsGun()) {
							ImGui::Text("Current Weapon: %s", G::LocalPlayer->GetWeapon()->GetWeaponName().c_str());

							ImGui::Combo(("Hitbox"), &Vars.Legitbot.Weapon[Vars.wpn].Hitbox, ("PELVIS\0\rSpine01\0\rSpine02\0\rHIP\0\rLOWER SPINE\0\rMIDDLE SPINE\0\rUPPER SPINE\0\rNECK\0\rHEAD\0\rNEAREST\0\0"), -1);
							ImGui::SliderFloat(("FOV"), &Vars.Legitbot.Weapon[Vars.wpn].FOV, 0.1f, 25.f, "%.1f");
							ImGui::Checkbox("Perfect Silent", &Vars.Legitbot.Weapon[Vars.wpn].pSilent);
							if(Vars.Legitbot.Weapon[Vars.wpn].pSilent)
								ImGui::SliderFloat(("Perfect Silent FOV"), &Vars.Legitbot.Weapon[Vars.wpn].PFOV, 0.1f, 2.f, "%.1f");

							ImGui::SliderFloat(("Smooth"), &Vars.Legitbot.Weapon[Vars.wpn].Speed, 0.1f, 100.f, "%.1f");
							ImGui::Checkbox(("AutoWall"), &Vars.Legitbot.Weapon[Vars.wpn].autowall);
							if(Vars.Legitbot.Weapon[Vars.wpn].autowall)
								ImGui::SliderFloat(("MinDamage"), &Vars.Legitbot.Weapon[Vars.wpn].mindamage, 0.1f, 100.f, "%.0f");

							if (!G::LocalPlayer->GetWeapon()->IsSniper()) {
								ImGui::SliderFloat(("RCS Y"), &Vars.Legitbot.Weapon[Vars.wpn].RCSAmountY, 1.f, 100.f, "%.0f", 1.f);
								ImGui::SliderFloat(("RCS X"), &Vars.Legitbot.Weapon[Vars.wpn].RCSAmountX, 1.f, 100.f, "%.0f", 1.f);
							}
							
						}	
						ImGui::EndChild();
					}
				}
				ImGui::Columns(1);
			}
		}
		if (ragebot) {
			ImGui::Columns(1, NULL, false);
			{
				ImGui::BeginChild("RAGEBOT", ImVec2(0, 0), true);
				{
					ImGui::Columns(1, NULL, true);
					{
						ImGui::Checkbox("Enable Rage##Rage", &Vars.RageBot.rEnable);
						
					}
					ImGui::Checkbox("Use Rage Aimkey##Rage", &Vars.RageBot.rOnKey);
					ImGui::SliderFloat("FOV##Rage", &Vars.RageBot.FOV, 1.f, 180.f, "%.0f");
					if (Vars.RageBot.rOnKey) {
						ImGui::SameLine();
						ImGui::Hotkey("Key##Aimbot Key", &Vars.RageBot.rKey, ImVec2(100, 20));
					}
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox("Silent##Rage", &Vars.RageBot.silent);
						ImGui::Checkbox("No Recoil##Rage", &Vars.RageBot.NoRecoil);
						ImGui::Checkbox("Auto Shoot##Rage", &Vars.RageBot.AutoShoot);
						//ImGui::Checkbox("Auto Revolver##Rage", &g_Options.rage_autocockrevolver);
					}
					ImGui::NextColumn();
					{
						ImGui::Checkbox("Auto Scope##Rage", &Vars.RageBot.AutoScope);
					//	ImGui::Checkbox("Auto Crouch##Rage", &g_Options.rage_autocrouch);
						//ImGui::Checkbox("Auto Stop##Rage", &g_Options.rage_autostop);
						ImGui::Checkbox("Auto Body-Aim##Rage", &Vars.RageBot.BodyAim);
						if (Vars.RageBot.BodyAim)
							ImGui::SliderInt("BAim X Shots##Rage", &Vars.RageBot.BodyAimAferShoots, 0, 10);
					}
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::Columns(2, NULL, false);
					{
						ImGui::PushItemWidth(235);
						ImGui::Text("lagcomp");
						//ImGui::Checkbox("Lag Compensation##Rage", &g_Options.rage_lagcompensation);
						//if (g_Options.rage_lagcompensation)
						//	ImGui::Combo("Type##Rage", &g_Options.rage_lagcompensation_type, opt_LagCompType, 3);

						//ImGui::Checkbox("Synchronize##Rage", &g_Options.rage_fixup_entities);
						ImGui::PopItemWidth();
					}
					ImGui::NextColumn();
					{
						// lets be hacky :>
					}

					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::Columns(2, NULL, true);
					{
						ImGui::Text("Autowall");
						ImGui::SliderFloat("Min. damage##Rage", &Vars.RageBot.MinDamage, 0.f, 120.f, "%.2f");
					}
					ImGui::NextColumn();
					{
						ImGui::Text("Hitchance");
						ImGui::SliderFloat("Amount %##Rage", &Vars.RageBot.t_hChance, 0.f, 100.0f, "%.2f");
					}


					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::Text("Target");
					ImGui::Separator();
					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox("Prioritize Selected Hitbox##Rage", &Vars.RageBot.RagePrioretez);
						ImGui::Combo("Select Hitbox##Rage", &Vars.RageBot.Hitbox, opt_AimHitboxSpot, 4);
						ImGui::Checkbox("Multipoint##Rage", &Vars.RageBot.MultiPoint);
						if (Vars.RageBot.MultiPoint)
							ImGui::SliderFloat("Pointscale##Rage", &Vars.RageBot.pointScale, 0.0f, 1.0f);
					}
					ImGui::NextColumn();
					{
						ImGui::Text("Hitboxes To Scan:");
						ImGui::BeginChild("#MULTIPOINT", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
						for (int i = 0; i < ARRAYSIZE(opt_MultiHitboxes); ++i)
						{
							ImGui::Selectable(opt_MultiHitboxes[i], &Vars.RageBot.rage_multiHitboxes[i]);
						}
						ImGui::EndChild();
					}
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::EndChild();
				}
			}
		}
	}
	void Visual() {
		ImGui::Text("Visual"); ImGui::SameLine();
		ImGui::Checkbox("Visuals Enable", &Vars.Visuals.vEnable);
		ImGui::Separator();
		ImGui::Columns(2, NULL, false);
		{
			ImGui::BeginChild("COL1", ImVec2(0, 0), true);
			{
				ImGui::Text("ESP");
				ImGui::Separator();
				ImGui::Columns(1);
				{
					ImGui::Checkbox(("2D Box"), &Vars.Visuals.vEsp);
					if(Vars.Visuals.vEsp)
						ImGui::ColorPickerBox("ESPColor", Vars.Visuals.esp_player_bbox_color_ct, Vars.Visuals.esp_player_bbox_color_t, Vars.Visuals.esp_player_bbox_color_ct_visible, Vars.Visuals.esp_player_bbox_color_t_visible, false);
					ImGui::Checkbox(("Head Dot"), &Vars.Visuals.vHead_dot);
					ImGui::Checkbox(("Health"), &Vars.Visuals.vHealth);
					ImGui::Checkbox("Armor", &Vars.Visuals.vArmor);
					ImGui::Checkbox("Name", &Vars.Visuals.vShowName);
					ImGui::Checkbox(("Debug"), &Vars.Visuals.vDebug);
					ImGui::Text("Filters");
					ImGui::Separator();
					ImGui::Columns(2, NULL, false);
					{
						ImGui::BeginChild("COL1", ImVec2(0, 0), true);
						{
							ImGui::Columns(1);
							{							
								ImGui::Selectable(("Show team"), &Vars.Visuals.Filter.ShowTeam);
							}
							ImGui::EndChild();
						}
					}
					ImGui::NextColumn();
					{
						ImGui::BeginChild("COL2", ImVec2(0, 0), true);
						{
							ImGui::Columns(1, NULL, true);
							{
								ImGui::Text("debug2");
							}
							ImGui::EndChild();
						}
					}

				}
				ImGui::EndChild();
			}
			ImGui::NextColumn();
			{
				ImGui::BeginChild("COL2", ImVec2(0, 0), true);
				{
					ImGui::Text("View");
					ImGui::Separator();
					ImGui::Columns(1, NULL, true);
					{
						ImGui::Checkbox(("OverrideHelper"), &Vars.Visuals.vOverrideFov);
						if (Vars.Visuals.vOverrideFov) {
							ImGui::SliderInt(("ViewFov"), &Vars.Visuals.vOverrideViewf, 0, 120);
							ImGui::SliderInt(("OverrideFov"), &Vars.Visuals.vOverrideFovf, 0, 120);
						}
					}
					ImGui::Columns(1, NULL, true);
					{
						ImGui::Text("debug4");
					}
					ImGui::EndChild();
				}
			}
		}
	}
	void Misc() {
		ImGui::Separator();
		ImGui::Columns(2, NULL, false);
		{
			ImGui::BeginChild("COL1", ImVec2(0, 0), true);
			{
				ImGui::Text("Config");
				ImGui::Separator();
				ImGui::Columns(1);
				{
					ImGui::Combo(("##CFGs"), &Vars.Misc.curcfg, ("Off\0\rMM\0\rLegit\0\rMM HvH\0\rRage\0\rPublic\0\0"), -1);

					if (ImGui::Button(("Load"), ImVec2(60.275f, 20.f)))
					Config->Load();
					ImGui::SameLine();
					if (ImGui::Button(("Save"), ImVec2(60.275f, 20.f)))
					Config->Save();
				}
				ImGui::EndChild();
			}
			ImGui::NextColumn();
			{
				ImGui::BeginChild("COL2", ImVec2(0, 0), true);
				{
					ImGui::Text("Movement");
					ImGui::Separator();
					ImGui::Checkbox("BHop", &Vars.Misc.mBhop);
					ImGui::Checkbox("AStrafe", &Vars.Misc.mAStrafe);

					ImGui::EndChild();
				}
			}
		}
	}
	void MenuDraw() {
		if (!Vars.Menu.Opened) return;
		auto size = Getscreen_center();
		ImGui::SetNextWindowPos(ImVec2((float)(size.first / 2 - 190),(float)(size.second / 2)));
		ImGui::SetNextWindowSize(ImVec2(195.f, 234.f));
		if (ImGui::Begin("BlatantHack"), &Vars.Menu.Opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar  | ImGuiWindowFlags_NoSavedSettings| ImGuiWindowFlags_NoMove)
		{
			if (ImGui::Button("LegitBot", ImVec2(180, 50))) {
				getCurrentTab(1);
			}
			if (ImGui::Button("Visuals", ImVec2(180, 50))) {
				getCurrentTab(2);
			}
			if (ImGui::Button("Misc", ImVec2(180, 50))) {
				getCurrentTab(3);
			}
		}
		ImGui::Separator();
		ImGui::End();

		if (tab > 0) {
			ImGui::SetNextWindowPos(ImVec2((float)(size.first / 2 + 15), (float)(size.second / 2)));
			ImGui::SetNextWindowSize(ImVec2(830.f, 360.f));
			if (ImGui::Begin("Tabsss", &Vars.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				if (tab == 1) {
					Aimbot();
				}
				else if (tab == 2) {
					Visual();
				}
				else if (tab == 3) {
					Misc();
				}
				ImGui::End();
			}
		}
	}
}



EndSceneFn oEndScene;
long __stdcall Hooks::EndScene(IDirect3DDevice9* pDevice)
{
	if(!Vars.Menu.Opened) 
		return oEndScene(pDevice);

	SaveState(pDevice);
	if (!G::d3dinit)
		gui::guiinit(pDevice);

	ImGui::GetIO().MouseDrawCursor = Vars.Menu.Opened;
	

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	gui::MenuDraw();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	RestoreState(pDevice);

	return oEndScene(pDevice);
}

ResetFn oReset;
long __stdcall Hooks::Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (!G::d3dinit)
		return oReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = oReset(pDevice, pPresentationParameters);

	if (hr == D3D_OK)
	{
		ImGui_ImplDX9_CreateDeviceObjects();		
	}
	return hr;
}