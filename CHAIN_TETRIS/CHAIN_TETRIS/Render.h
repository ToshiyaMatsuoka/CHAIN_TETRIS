#ifndef RENDER_H
#define RENDER_H
#include "Main.h"


void Render(void);


struct OBJECT_STATE
{
	float x, y, scale;
};


extern STAGE_STATE StagePosition;
extern TEXTURE_STATE Logo;
extern TEXTURE_STATE g_Girl;
extern int BlockColor;
extern int g_scene;

#endif // !RENDER_H

