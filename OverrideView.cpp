#include "Cheat.h"

OverrideViewFn oOverrideView;
void __fastcall Hooks::OverrideView(void* ecx, void* edx, CViewSetup* vsView)
{
	if(!I::Engine->is_connected() || !I::Engine->is_ingame())
		return oOverrideView(ecx, vsView);


	if (Vars.Visuals.vOverrideFov)
	{
		if (!G::LocalPlayer->IsScoped() && Vars.Visuals.vOverrideFovf != 0 && Vars.Visuals.vEnable)
			vsView->fov = vsView->fov + Vars.Visuals.vOverrideFovf;
		else
			vsView->fov = vsView->fov;
	}
	
	oOverrideView(ecx, vsView);
}

void GetViewModelFOV_func(float& fov)
{
	if (I::Engine->is_ingame()) {
		if (!G::LocalPlayer->IsScoped() && Vars.Visuals.vOverrideViewf != 0 && Vars.Visuals.vEnable) {
			fov += Vars.Visuals.vOverrideViewf;
		}
	}
}

GetViewModelFOVfn oGetViewModel;
float __stdcall Hooks::GetViewModelFOV() {

	float fov = oGetViewModel();

	if (Vars.Visuals.vOverrideFov)
	GetViewModelFOV_func(fov);

	return fov;
}