#include "DX9.h"
#include "Control.h"
#include "Render.h"

bool LEFT = false;
bool RIGHT = false;
bool UP = false;
bool DOWN = false;
bool KEY_Z = false;
bool KEY_C = false;
bool SPACE = false;


void TutorialControl(){
	static bool SkipTTRL = false;
	static int firstTime = 0;
	firstTime++;
	if (KeyOn == GetKeyBoardState(DIK_RIGHT)){
		RIGHT = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_LEFT)){
		LEFT = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_DOWN)){
		DOWN = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_UP)){
		UP = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_Z)){
		KEY_Z = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_C)){
		KEY_C = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_SPACE)){
		SPACE = true;
	}
	if ((KeyRelease == GetKeyBoardState(DIK_RETURN)) || (KeyRelease == GetKeyBoardState(DIK_NUMPADENTER))) {
		SkipTTRL = false;
		if (firstTime > 5) {
			SkipTTRL = true;
			firstTime = 0;
		}
	}
	if (SkipTTRL) {
		g_scene = STATE_MAIN;
		LEFT = false;
		RIGHT = false;
		UP = false;
		DOWN = false;
		KEY_Z = false;
		KEY_C = false;
		SPACE = false;
		SkipTTRL = false;
	}

	GetControl();
	CheckButtonState(XINPUT_GAMEPAD_A, ButtonA);
	CheckButtonState(XINPUT_GAMEPAD_B, ButtonB);
	CheckButtonState(XINPUT_GAMEPAD_X, ButtonX);
	CheckButtonState(XINPUT_GAMEPAD_Y, ButtonY);
	CheckButtonState(XINPUT_GAMEPAD_START, ButtonStart);
	CheckButtonState(XINPUT_GAMEPAD_BACK, ButtonBack);
	CheckButtonState(XINPUT_GAMEPAD_LEFT_SHOULDER, ButtonLB);
	CheckButtonState(XINPUT_GAMEPAD_RIGHT_SHOULDER, ButtonRB);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_UP, ButtonUP);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_DOWN, ButtonDOWN);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_LEFT, ButtonLEFT);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_RIGHT, ButtonRIGHT);
	CheckButtonState(XINPUT_GAMEPAD_START, ButtonStart);
	CheckButtonState(XINPUT_GAMEPAD_BACK, ButtonBack);
	CheckButtonState(XINPUT_GAMEPAD_LEFT_SHOULDER, ButtonLB);
	CheckButtonState(XINPUT_GAMEPAD_RIGHT_SHOULDER, ButtonRB);

	if (GetAnalogL(ANALOGRIGHT)){
		RIGHT = true;
	}
	if (GetAnalogL(ANALOGLEFT)){
		LEFT = true;
	}
	if (!GetAnalogL(ANALOGUP)){
		DOWN = true;
	}
	if (!PadState[ButtonA]) {
		KEY_C = true;
	}
	if (!PadState[ButtonB]) {
		UP = true;
	}
	if (!PadState[ButtonX]) {
		KEY_Z = true;
	}
	if (!PadState[ButtonY]) {
		SPACE = true;
	}
	if (PadRelease == PadState[ButtonStart]) {
		SkipTTRL = false;
		if (firstTime > 5) {
			SkipTTRL = true;
			firstTime = 0;
		}
	}
	if (!PadState[ButtonDOWN]) {
		UP = true;
	}
	if (!PadState[ButtonLEFT]) {
		LEFT = true;
	}
	if (!PadState[ButtonRIGHT]) {
		RIGHT = true;
	}
}
/**
*描画処理
*/
void TutorialRender(void){

	//頂点情報を入れる--------------------------------------
	CUSTOMVERTEX tutorial_bg[4]
	{
	{ 0.f,   0.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ WIDTH,   0.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ WIDTH, HEIGHT, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 0.f,HEIGHT, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	//チェックマークのそれぞれの座表情報の設定
	CUSTOMVERTEX rightcheck[4]
	{
	{ 275.f, 172.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f, 172.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f, 227.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f, 227.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX leftcheck[4]
	{
	{ 275.f,172.f + 58.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + 58.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + 58.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + 58.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX upcheck[4]
	{
	{ 275.f,172.f + 116.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + 116.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + 116.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + 116.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX downcheck[4]
	{
	{ 275.f, 172.f + 174.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f, 172.f + 174.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f, 227.f + 174.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f, 227.f + 174.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX key_ccheck[4]
	{
	{ 275.f, 172.f + 290.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f, 172.f + 290.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f, 227.f + 290.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f, 227.f + 290.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX key_zcheck[4]
	{
	{ 275.f,172.f + 232.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + 232.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + 232.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + 232.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX spacecheck[4]
	{
	{ 275.f,172.f + 340.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + 340.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + 340.f, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + 340.f, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	
	//-----------------------------------------------------
	
	//画面の消去
	g_pD3Device->Clear(0, NULL,	D3DCLEAR_TARGET,D3DCOLOR_XRGB(0x00, 0x00, 0x00),1.0, 0);
	//描画の開始
	g_pD3Device->BeginScene();
	DrowTexture(tutorial_bg, "TUTORIAL_TEX");
	if (RIGHT){
		DrowTexture(rightcheck, "CHECK_TEX");
	}
	if (LEFT){
		DrowTexture(leftcheck, "CHECK_TEX");
	}
	if (DOWN){
		DrowTexture(upcheck, "CHECK_TEX");
	}
	if (UP){
		DrowTexture(downcheck, "CHECK_TEX");
	}
	if (KEY_Z){
		DrowTexture(key_zcheck, "CHECK_TEX");
	}
	if (KEY_C){
		DrowTexture(key_ccheck, "CHECK_TEX");
	}
	if (SPACE){
		DrowTexture(spacecheck, "CHECK_TEX");
	}
	//描画の終了
	g_pD3Device->EndScene();
	//表示
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}
