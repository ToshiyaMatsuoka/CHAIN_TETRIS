#include "Main.h"
#include "Render.h"
#include "DX9.h"
#include "TetriminoState.h"
#include "Gamescene.h"

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
bool isRight, isLeft, isDown, isRotateR, isRotateL, isPause, isHold, isDrop;
bool GameStop = false;
bool g_moveup = true;
int NowScorePoint;
static BYTE KeyOldState[256];


void Control() {

	static int keyStroke[10];
	if (g_scene == STATE_TITLE || g_scene == STATE_RESULT) {
		keyStroke[0]++;
	}
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

	if (GetAnalogL(ANALOGRIGHT))
	{
		for (int i = 0; i < 4; i++)
		{
			isRight = true;
		}
	}

	if (GetAnalogL(ANALOGLEFT))
	{
		for (int i = 0; i < 4; i++)
		{
			isLeft = true;
		}
	}

	//ホームポジションで上下が入力され続ける
	if (!GetAnalogL(ANALOGDOWN))
	{
		for (int i = 0; i < 4; i++)
		{

		}
	}
	if (!GetAnalogL(ANALOGUP))
	{
		for (int i = 0; i < 4; i++)
		{
			isDown = true;
		}
	}

	if (!PadState[ButtonA]) {
		isHold = true;
	}
	if (!PadState[ButtonB]) {
		isRotateR = true;
	}
	if (!PadState[ButtonX]) {
		isRotateL = true;
	}
	if (!PadState[ButtonY]) {
		isDrop = true;
	}
	if (!PadState[ButtonStart]) {
		PushEnter = true;
	}
	if (!PadState[ButtonBack]) {
		if (g_scene == STATE_RESULT)
		{
			g_scene = STATE_TITLE;
		}
		else isPause = true;
	}
	if (!PadState[ButtonUP]) {

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
	if (!PadState[ButtonLB]) {

	}
	if (!PadState[ButtonRB]) {

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
	if (KeyOn == GetKeyBoardState(DIK_SPACE)) {
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
	if ((GetKeyBoardState(DIK_RETURN)) || (GetKeyBoardState(DIK_NUMPADENTER))) {
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
		keyStroke[0] = 0;
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