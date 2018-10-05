#ifndef CONTROL_H
#define CONTROL_H
#include "Main.h"
#include "Render.h"
#include "DX9.h"

extern bool isRight, isLeft, isDown, isRotateR, isRotateL, isPause, isHold, isDrop;
extern bool GameStop;
extern bool g_moveup;
extern int NowScorePoint;


void Control(void);
void LogoControl(void);
void ResultControl(void);

#endif //!CONTROL_H