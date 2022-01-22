#include "Cheat.h"

HFont F::Meme;
HFont F::ESP;
HFont F::Watermark;
HFont F::Revue;

void D::SetupFonts()
{
	I::Surface->SetFontGlyphSet(F::Revue = I::Surface->Create_Font(), ("Revue"), 24, FW_DONTCARE, NULL, NULL, FONTFLAG_ANTIALIAS);
	I::Surface->SetFontGlyphSet(F::Meme = I::Surface->Create_Font(), ("MS Sans Serif"), 10, FW_DONTCARE, NULL, NULL, FONTFLAG_ANTIALIAS);
	I::Surface->SetFontGlyphSet(F::ESP = I::Surface->Create_Font(), ("Verdana"), 12, 800, NULL, NULL, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(F::Watermark = I::Surface->Create_Font(), ("MS Sans Serif"), 12, FW_DONTCARE, NULL, NULL, FONTFLAG_OUTLINE);
}

void D::DrawString(HFont font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf_s(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	I::Surface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(r, g, b, a);
	I::Surface->DrawSetTextPos(x, y - height / 2);
	I::Surface->DrawPrintText(wbuf, wcslen(wbuf));
}


void D::Text(int x, int y, Color color, DWORD font, const char* text, ...)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	I::Surface->DrawSetTextFont(font);

	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(wcstring, wcslen(wcstring));
}

void D::DrawStringUnicode(HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...)
{
	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int iWidth, iHeight;

	I::Surface->GetTextSize(font, msg, iWidth, iHeight);
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(r, g, b, a);
	I::Surface->DrawSetTextPos(!bCenter ? x : x - iWidth / 2, y - iHeight / 2);
	I::Surface->DrawPrintText(msg, wcslen(msg));
}

void D::DrawRect(int x, int y, int w, int h, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void D::Rectangle(int x, int y, int w, int h, int px, Color col)
{
	DrawRect(x, (y + h - px), w, px, col);
	DrawRect(x, y, px, h, col);
	DrawRect(x, y, w, px, col);
	DrawRect((x + w - px), y, px, h, col);
}

void D::Border(int x, int y, int w, int h, int line, Color col)
{
	DrawRect(x, y, w, line, col);
	DrawRect(x, y, line, h, col);
	DrawRect((x + w), y, line, h, col);
	DrawRect(x, (y + h), (w + line), line, col);
}

void D::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float& flRainbow)
{
	
}

void D::DrawRectGradientHorizontal(int x, int y, int width, int height, Color color1, Color color2)
{
	
}

void D::DrawPixel(int x, int y, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawFilledRect(x, y, x + 1, y + 1);
}

void D::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
		I::Surface->DrawSetColor(col);
		I::Surface->DrawOutlinedRect(x, y, x + w, y + h);	
}

void D::DrawOutlinedCircle(int x, int y, int r, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawOutlinedCircle(x, y, r, 1);
}

void D::DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawLine(x0, y0, x1, y1);
}


void D::DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw)
{
	int iRealX = bRight ? iX - iWidth : iX;
	int iRealY = bDown ? iY - iHeight : iY;

	if (bDown && bRight)
		iWidth = iWidth + 1;

	D::DrawRect(iRealX, iY, iWidth, 1, colDraw);
	D::DrawRect(iX, iRealY, 1, iHeight, colDraw);

	D::DrawRect(iRealX, bDown ? iY + 1 : iY - 1, !bDown && bRight ? iWidth + 1 : iWidth, 1, Color(0, 0, 0, 255));
	D::DrawRect(bRight ? iX + 1 : iX - 1, bDown ? iRealY : iRealY - 1, 1, bDown ? iHeight + 2 : iHeight + 1, Color(0, 0, 0, 255));
}

void D::DrawPolygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = I::Surface->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	I::Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	I::Surface->DrawSetColor(color);
	I::Surface->DrawSetTexture(Texture);

	I::Surface->DrawTexturedPolygon(count, Vertexs);
}

void D::DrawBox(int x, int y, int w, int h, Color color)
{
	// top
	DrawRect(x, y, w, 1, color);
	// right
	DrawRect(x, y + 1, 1, h - 1, color);
	// left
	DrawRect(x + w - 1, y + 1, 1, h - 1, color);
	// bottom
	DrawRect(x, y + h - 1, w - 1, 1, color);
}

void D::DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col)
{
	std::vector<Vertex_t> p;
	for (int _i = 0; _i < 3; _i++)
	{
		int _x = x + (_i < 2 && r || w - r);
		int _y = y + (_i % 3 > 0 && r || h - r);
		for (int i = 0; i < v; i++)
		{
			float a = RAD2DEG((i / v) * -90 - _i * 90);
			p.push_back(Vertex_t(Vector2D(_x + sin(a) * r, _y + cos(a) * r)));
		}
	}

	D::DrawPolygon(4 * (v + 1), &p[0], col);
}

bool D::WorldToScreen(const Vector& origin, Vector& screen)
{
	return I::DebugOverlay->ScreenPosition(origin, screen) != 1;
}


int D::GetStringWidth(HFont font, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf_s(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int iWidth, iHeight;

	I::Surface->GetTextSize(font, wbuf, iWidth, iHeight);

	return iWidth;
}

RECT D::GetTextSize(HFont font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	I::Surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}





void D::Draw3DBox(Vector* boxVectors, Color color)
{
	
}

void D::DrawCircle(int x, int y, int r, int s, Color color)
{
	auto Step = M_PI * 2.0 / s;
	for (double a = 0; a < (M_PI * 2.0); a += Step)
	{
		auto x1 = r * cos(a) + x;
		auto y1 = r * sin(a) + y;
		auto x2 = r * cos(a + Step) + x;
		auto y2 = r * sin(a + Step) + y;

		DrawLine((int)x1, (int)y1, (int)x2, (int)y2, color);
	}
}

void D::ShowRadar(float X, float Y, float Size)
{
	
}

void D::DrawRadarEntity(float X, float Y, float RadarSize, Color _Color, Vector RadarX, Vector RadarY, QAngle Angle, int HP, bool IsFriend)
{

	
}


