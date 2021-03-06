#ifndef CONTROL_H
#define CONTROL_H


enum KEYSTATE
{
	//! 押されていない
	KeyOff,
	//! 押されている
	KeyOn,
	//! 押し続けている
	KeyPush,
	//! 離された
	KeyRelease
};

extern bool isRight, isLeft, isDown, isRotateR, isRotateL, isPause, isHold, isDrop;
extern bool GameStop;
extern bool g_moveup;
extern int NowScorePoint;
extern BYTE KeyOldState[256];

int GetKeyBoardState(int KeyName);
void Control(void);
void LogoControl(void);
void ResultControl(void);

#endif //!CONTROL_H