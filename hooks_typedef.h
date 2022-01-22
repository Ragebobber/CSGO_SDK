#pragma once
using EndSceneFn = long(__stdcall*)(IDirect3DDevice9* device);
extern EndSceneFn oEndScene;

using ResetFn = long(__stdcall*)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
extern ResetFn oReset;

using CreateMoveFn = bool(__stdcall*)(float, CUserCmd*);
extern CreateMoveFn oCreateMove;

using OverrideViewFn = void(__thiscall*)(void*, CViewSetup*);
extern OverrideViewFn oOverrideView;

using PaintTraverseFn = void(__thiscall*)(void*, unsigned int, bool, bool);
extern PaintTraverseFn oPaintTraverse;

using LockCursorFn = void(__thiscall*)(void*);
extern LockCursorFn o_LockCursor;

using GetViewModelFOVfn = float(__stdcall*)();
extern GetViewModelFOVfn oGetViewModel;

namespace Hooks {
	extern void __fastcall		OverrideView(void* ecx, void* edx, CViewSetup* vsView);
	extern bool __stdcall		CreateMove(float flInputSampleTime, CUserCmd* cmd);
	extern long __stdcall		EndScene(IDirect3DDevice9* pDevice);
	extern long __stdcall		Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern float __stdcall		GetViewModelFOV();
	extern WNDPROC				oldWindowProc;
	extern LRESULT __stdcall	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern void __fastcall		PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce);
	extern void __fastcall		LockCursor_h(void* ecx, void*);
}