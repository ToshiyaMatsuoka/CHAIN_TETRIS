#ifndef CONTROL_H
#define CONTROL_H
#include "Main.h"
#include "Render.h"
#include "DX9.h"

enum KEYSTATE
{
	//! ‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢
	KeyOff,
	//! ‰Ÿ‚³‚ê‚Ä‚¢‚é
	KeyOn,
	//! ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é
	KeyPush,
	//! —£‚³‚ê‚½
	KeyRelease
};

extern bool isRight, isLeft, isDown, isRotateR, isRotateL, isPause, isHold, isDrop;
extern bool GameStop;
extern bool g_moveup;
extern int NowScorePoint;

int GetKeyBoardState(int KeyName);
void Control(void);
void LogoControl(void);
void ResultControl(void);

#endif //!CONTROL_H