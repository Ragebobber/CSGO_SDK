#include "Cheat.h"


CreateMoveFn oCreateMove;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* cmd)
{
	static bool ret = oCreateMove(flInputSampleTime, cmd);

	G::LocalPlayer = I::ClientEntList->GetClientEntity(I::Engine->get_local_player());

	if (!cmd->command_number || !cmd)
		return ret;

	if (ret)
		I::Prediction->SetLocalViewAngles(cmd->view_angles);

	G::UserCmd = cmd;

	auto ebp = (uintptr_t*)(uintptr_t(_AddressOfReturnAddress()) - sizeof(void*));
	bool& bSendPacket =  * reinterpret_cast<bool*>(*ebp - 0x1C);
	
	E::Misc->MRun();
	
	E::Prediction->PreStart();

	E::Prediction->Start();

	if(Vars.Triggerbot.tEnable)
		E::Triggerbot->do_Triggerbot();
	if (Vars.Legitbot.aEnable)
		E::LegitBot->LegitBot_run(bSendPacket);
	if (Vars.RageBot.rEnable)
		E::RageBot->RageBot_run(bSendPacket);

	
	E::Prediction->End();

	G::UserCmd->view_angles.Normalized();
	G::UserCmd->view_angles.Clamp();

	return false;
}