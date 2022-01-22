#pragma once

std::pair<int,int> Getscreen_center() {
	int x, y;
	I::Engine->GetScreenSize(x, y);
	return std::pair<int, int>(x / 2, y / 2);
}

int tab;
void getCurrentTab(int x) {
	tab = x;
}

IDirect3DVertexDeclaration9* vertDec;
IDirect3DVertexShader9* vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;
void SaveState(IDirect3DDevice9* device)
{
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	device->GetVertexDeclaration(&vertDec);
	device->GetVertexShader(&vertShader);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
}
void RestoreState(IDirect3DDevice9* device)
{
	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vertDec);
	device->SetVertexShader(vertShader);
}

