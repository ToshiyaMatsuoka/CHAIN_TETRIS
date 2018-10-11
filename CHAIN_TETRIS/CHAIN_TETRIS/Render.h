#ifndef RENDER_H
#define RENDER_H
#include "Main.h"
//フィールドの縦幅・横幅
#define FIELD_HEIGHT	24
#define FIELD_WIDTH		12

struct OBJECT_STATE
{
	float x, y, scale;
};
struct TEXTURE_STATE
{
	float x, y, scale;
};
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

extern TEXTURE_STATE Logo;
extern TEXTURE_STATE g_Girl;

extern int g_scene;
void Render(void);
void DrowTexture(CUSTOMVERTEX vertex[4], int textureNumber);
void DrowWord(RECT rect, LPCSTR text, int FontNumber, int textformat = DT_LEFT, DWORD color = 0xffffffff);

#endif // !RENDER_H

