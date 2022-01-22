#include "Cheat.h"

LockCursorFn o_LockCursor;
void __fastcall Hooks::LockCursor_h(void* ecx, void*)
{
	if (!o_LockCursor) return;

	o_LockCursor(ecx);

	if (Vars.Menu.Opened)
	{
		I::Surface->UnlockCursor();
		I::InputSystem->EnableInput(false);
	}
	else {
		I::InputSystem->EnableInput(true);
	}
}