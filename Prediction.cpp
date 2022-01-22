#include "Cheat.h"

void CPrediction::PreStart() {
	if (!I::Engine->is_ingame())
		return;

	if (I::ClientMode->m_nDeltaTick > 0) {
		I::Prediction->Update(I::ClientMode->m_nDeltaTick, I::ClientMode->m_nDeltaTick > 0,
			I::ClientMode->last_command_ack, I::ClientMode->lastoutgoingcommand + I::ClientMode->chokedcommands);
	}
}
void CPrediction::Start() {
	//CPrediction::ProcessMovement
	if (!G::UserCmd || !I::MoveHelper || !I::Engine->is_connected() || !I::Engine->is_ingame())
		return;
	
	this->pLocal = G::LocalPlayer;

	this->ucmd = G::UserCmd;

	if (!pLocal)
		return;
	//m_pCurrentCommand
	*(CUserCmd**)((DWORD)pLocal + 0x3348) = ucmd;
	//m_pLastCommand
	*(CUserCmd*)((DWORD)pLocal + 0x3288) = *ucmd;
	//ida xref Activity %s has %d sequences with a total weight of %d! or SelectWeightedSequence
	if ((!iPredictionRandomSeed || !pSetPredictionEntity) && !firstInit) {
		iPredictionRandomSeed = *(int**)(offsets.m_iPredictionRandomSeed);
		pSetPredictionEntity = *(CBaseEntity**)(offsets.m_pSetPredictionEntity);
		firstInit = true;
	}

	*iPredictionRandomSeed = ucmd ? ucmd->random_seed : -1;
	pSetPredictionEntity = pLocal;

	flOldCurrentTime = I::Globals->cur_time;
	flOldFrameTime = I::Globals->frame_time;
	iOldTickCount = I::Globals->tick_count;

	const int iOldTickBase = pLocal->GetTickBase();

	const bool bOldIsFirstPrediction = I::Prediction->bIsFirstTimePredicted;
	const bool bOldInPrediction = I::Prediction->bInPrediction;

	I::Globals->cur_time = TICKS_TO_TIME(this->GetTickbase(ucmd, pLocal));
	I::Globals->frame_time = I::Prediction->bEnginePaused ? 0.f : TICK_INTERVAL;
	I::Globals->tick_count = this->GetTickbase(ucmd, pLocal);

	I::Prediction->bIsFirstTimePredicted = false;
	I::Prediction->bInPrediction = true;
	
	I::GameMovement->StartTrackPredictionErrors(pLocal);

	if (ucmd->impulse)
		*(unsigned char*)((DWORD)pLocal + 0x320C) = ucmd->impulse;

	
	ucmd->buttons |= *(int*)((DWORD)pLocal + 0x3344);// GetButtonForced
	ucmd->buttons &= ~*(int*)((DWORD)pLocal + 0x3340);// GetButtonDisabled
	
	UpdateButtonState(pLocal, ucmd);

	I::Prediction->CheckMovingGround(pLocal, I::Globals->frame_time);

	I::Prediction->SetLocalViewAngles(ucmd->view_angles); 
	
	if (pLocal->PhysicsRunThink(0))
		pLocal->RunPreThink();

	int* iNextThinkTick = (int*)((DWORD)pLocal + 0x0FC);
	if (*iNextThinkTick > 0 && *iNextThinkTick <= GetTickbase(ucmd, pLocal))
	{
		*iNextThinkTick = TICK_NEVER_THINK;
		pLocal->RunThink();
	}

	I::MoveHelper->SetHost(pLocal);

	I::Prediction->SetupMove(pLocal, ucmd, I::MoveHelper, &moveData);
	I::GameMovement->ProcessMovement(pLocal, &moveData);

	I::Prediction->FinishMove(pLocal, ucmd, &moveData);
	I::MoveHelper->ProcessImpacts();

	pLocal->RunPostThink();

	pLocal->SetTickBase(iOldTickBase);

	I::Prediction->bInPrediction = bOldInPrediction;
	I::Prediction->bIsFirstTimePredicted = bOldIsFirstPrediction;
}

int CPrediction::GetTickbase(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	static int iTick = 0;
	static CUserCmd* pLastCmd = nullptr;

	if (pCmd != nullptr)
	{
		// if command was not predicted - increment tickbase
		if (pLastCmd == nullptr || pLastCmd->is_predicted)
			iTick = pLocal->GetTickBase();
		else
			iTick++;
	}
	pLastCmd = pCmd;
	return iTick;
}


void  CPrediction::UpdateButtonState(CBaseEntity* pLocal,CUserCmd* cmd)
{
	const int iButtons = cmd->buttons;
	int* nPlayerButtons = (int*)((DWORD)pLocal + 0x3208);
	const int nButtonsChanged = iButtons ^ *nPlayerButtons;

	// m_afButtonLast 
	*(int*)((DWORD)pLocal + 0x31FC) = *nPlayerButtons;

	// m_nButtons  
	*nPlayerButtons = iButtons;

	// m_afButtonPressed  
	*(int*)((DWORD)pLocal + 0x3200) = iButtons & nButtonsChanged;

	// m_afButtonReleased 
	*(int*)((DWORD)pLocal + 0x3204) = nButtonsChanged & ~iButtons;

}


void CPrediction::End() {
	if (!pLocal->GetAlive() || I::MoveHelper == nullptr)
		return;

	// reset host player
	I::MoveHelper->SetHost(nullptr);

	// restore globals
	I::Globals->cur_time = flOldCurrentTime;
	I::Globals->frame_time = flOldFrameTime;
	I::Globals->tick_count = iOldTickCount;

	I::GameMovement->FinishTrackPredictionErrors(pLocal);

	// finish command
	*(CUserCmd**)((DWORD)pLocal + 0x3348) = nullptr;

	// reset prediction seed
	*iPredictionRandomSeed = -1;

	// reset prediction entity
	pSetPredictionEntity = nullptr;

	// reset move
	I::GameMovement->Reset();
}