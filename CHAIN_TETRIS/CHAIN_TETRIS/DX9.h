#ifndef DX9_H
#define DX9_H
#include "Main.h"

enum PADSTATE
{
	PadOn,
	PadOff,
	PadPush,
	PadRelease
};

enum ButtonIndex
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonRB,
	ButtonLB,
	ButtonStart,
	ButtonBack,
	ButtonUP,
	ButtonDOWN,
	ButtonLEFT,
	ButtonRIGHT,
	buttomindexMAX
};

enum Analog
{
	ANALOGUP,
	ANALOGDOWN,
	ANALOGLEFT,
	ANALOGRIGHT,
	ANALOGMAX
};

void GetControl(void);
extern PADSTATE GetButton(ButtonIndex index);
extern bool GetAnalogL(Analog analogstate);
extern PADSTATE PadState[buttomindexMAX];
extern PADSTATE PadOldState[buttomindexMAX];

void CheckButtonState(WORD, int);
HRESULT InitD3d(HWND hWnd);
HRESULT InitDinput(HWND hWnd);
void BuildDXDevice();
#endif // !RENDER_H