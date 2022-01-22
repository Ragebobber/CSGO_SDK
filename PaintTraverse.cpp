#include "Cheat.h"

PaintTraverseFn oPaintTraverse;
void __fastcall Hooks::PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(ecx, panel, forceRepaint, allowForce);

	static uint32_t FocusOverlayPanel;
	if (!FocusOverlayPanel)
	{
		if (strstr(I::VPanel->GetName(panel), "MatSystemTopPanel"))
		{
			FocusOverlayPanel = panel;
			D::SetupFonts();
		}
	}
	if (panel != FocusOverlayPanel)
		return;

	D::DrawPixel(1, 1, Color(0, 0, 0));
	E::Visuals->Draw();
	
}