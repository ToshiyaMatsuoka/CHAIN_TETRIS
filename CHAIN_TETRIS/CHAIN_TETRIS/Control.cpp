#include "Main.h"
#include "Render.h"
#include "DX9.h"
#include "TetriminoState.h"

enum KEYNUMBER {
	RIGHT,
	LEFT,
	DOWN,
	ROTATERIGHT,
	ROTATELEFT,
	PAUSE,
	HOLD,
	DROP

};

#define SWITCH_RESULT 80
//タイトルロゴ落下量
#define FALL_SPEED 5.f

bool isRight, isLeft, isDown, isRotateR, isRotateL, isPause, isHold, isDrop;
bool GameStop = false;
bool g_moveup = true;
int NowScorePoint;
BYTE KeyOldState[256];


void Control() {

	bool PushEnter = false;
	//XInputデバイス操作
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

	if (GetAnalogL(ANALOGRIGHT))	{
		isRight = true;
	}

	if (GetAnalogL(ANALOGLEFT))	{
		isLeft = true;
	}

	//ホームポジションで上下が入力され続ける
	if (!GetAnalogL(ANALOGUP))	{
		isDown = true;
	}

	if (PadPush == PadState[ButtonA]) {
		isHold = true;
	}
	if (PadPush == PadState[ButtonB]) {
		isRotateR = true;
	}
	if (PadPush == PadState[ButtonX]) {
		isRotateL = true;
	}
	if (PadRelease == PadState[ButtonY]) {
		isDrop = true;
	}
	if (PadRelease == PadState[ButtonStart]) {
		PushEnter = true;
	}
	if (PadRelease == PadState[ButtonBack]) {
		if (g_scene == STATE_RESULT)
		{
			g_scene = STATE_TITLE;
		}
		else isPause = true;
	}
	if (!PadState[ButtonDOWN]) {
		isDown = true;
	}
	if (!PadState[ButtonLEFT]) {
		isLeft = true;
	}
	if (!PadState[ButtonRIGHT]) {
		isRight = true;
	}


	if (KeyOn == GetKeyBoardState(DIK_LEFT)) {
		isLeft = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_RIGHT)) {
		isRight = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_UP)) {
		isRotateR = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_DOWN)) {
		isDown = true;
	}
	if (KeyRelease == GetKeyBoardState(DIK_SPACE)) {
		isDrop = true;
	}
	if (KeyRelease == GetKeyBoardState(DIK_C)) {
		isHold = true;
	}
	if (KeyOn == GetKeyBoardState(DIK_Z)) {
		isRotateL = true;
	}
	if (KeyRelease == GetKeyBoardState(DIK_ESCAPE)) {
		if (g_scene == STATE_RESULT)
		{
			g_scene = STATE_TITLE;
		}
		if (g_scene == STATE_MAIN) {
			isPause = true;
		}
	}
	if ((KeyRelease == GetKeyBoardState(DIK_RETURN)) || (KeyRelease == GetKeyBoardState(DIK_NUMPADENTER))) {
		PushEnter = true;
	}
	if (PushEnter) {

		if (g_scene == STATE_TITLE) {
			g_scene = STATE_TUTORIAL;
			PushEnter = false;
		}
		if (g_scene == STATE_RESULT) {
			g_scene = STATE_MAIN;
			PushEnter = false;
		}
		if (g_scene == STATE_MAIN) {
			PushEnter = false;
			GameStop = false;
		}
	}
	else PushEnter = false;

	if (isPause) {

		GameStop = true;
		isPause = false;
	}
	if (GameOver) {
		static int GameOverCnt = 0;
		GameOverCnt++;
		if (GameOverCnt > SWITCH_RESULT) {
			memset(KeyOldState, KeyOff, 256);
			g_scene = STATE_RESULT;
			GameOverCnt = 0;
		}
	}
}

//Logo制御
void LogoControl() {
	static bool FalledLogo = true;
	if (!FalledLogo&&!(Logo.y >= 130.f)) {
		FalledLogo = true;
	}
	if (FalledLogo)
	{
		Logo.y += FALL_SPEED;
	}
	if (Logo.y >= 130.f)
	{
		FalledLogo = false;
	}
}
//リザルトキャラアニメ
void ResultControl(){
	static float x = 0;
	if (g_moveup){
		x = x + 0.1f;
		g_Girl.y -= cos(x);
	}	
	NowScorePoint += (g_ScorePoint / 180) + 1;
	if (g_ScorePoint <= NowScorePoint)
	{
		NowScorePoint = g_ScorePoint;
	}

}

int GetKeyBoardState(int KeyName) {
	HRESULT hr = g_pKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		BYTE diks[256];
		g_pKeyDevice->GetDeviceState(sizeof(diks), &diks);

		if (diks[KeyName] & 0x80)
		{
			if (KeyOldState[KeyName] == KeyOn)
			{
				KeyOldState[KeyName] = KeyOn;
				return KeyOn;
			}
			else
			{
				KeyOldState[KeyName] = KeyOn;
				return KeyPush;
			}
		}
		else
		{
			if (KeyOldState[KeyName] == KeyOn)
			{
				KeyOldState[KeyName] = KeyOff;
				return KeyRelease;
			}
			else
			{
				KeyOldState[KeyName] = KeyOff;
				return KeyOff;
			}			
		}
	}
}