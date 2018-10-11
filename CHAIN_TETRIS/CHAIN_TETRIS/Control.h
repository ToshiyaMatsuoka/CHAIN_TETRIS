#ifndef CONTROL_H
#define CONTROL_H


enum KEYSTATE
{
	//! ������Ă��Ȃ�
	KeyOff,
	//! ������Ă���
	KeyOn,
	//! ���������Ă���
	KeyPush,
	//! �����ꂽ
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