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


void Control() {

	static int keyStroke[10];
	if (g_scene == STATE_TITLE|| g_scene == STATE_RESULT) {
		keyStroke[0]++;
	}
	bool PushEnter=false;
	//XInput�f�o�C�X����
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

	//�z�[���|�W�V�����ŏ㉺�����͂��ꑱ����
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




	//�L�[�{�[�h�R���g���[��
	HRESULT hr = g_pKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		BYTE diks[256];
		g_pKeyDevice->GetDeviceState(sizeof(diks), &diks);

		if (diks[DIK_LEFT] & 0x80) {
			isLeft = true;
		}
		if (diks[DIK_RIGHT] & 0x80) {
			isRight = true;
		}
		if (diks[DIK_UP] & 0x80) {
			isRotateR = true;
		}
		if (diks[DIK_DOWN] & 0x80) {
			isDown = true;
		}
		if (diks[DIK_SPACE] & 0x80) {
			isDrop = true;
		}
		if (diks[DIK_C] & 0x80) {
			isHold = true;
		}
		if (diks[DIK_Z] & 0x80) {
			isRotateL = true;
		}
		if (diks[DIK_ESCAPE] & 0x80) {
			if (g_scene == STATE_RESULT)
			{
				g_scene = STATE_TITLE;
			}
			if (g_scene == STATE_MAIN) {
				isPause = true;
			}
		}
		if ((diks[DIK_RETURN] & 0x80) || (diks[DIK_NUMPADENTER] & 0x80)) {
			PushEnter = true;
			//diks[DIK_RETURN] = NULL;
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
	}
	if (!GameStop) {
		//�e�g���~�m����
		if (isRight) {
			
			keyStroke[RIGHT]++;
			if (keyStroke[RIGHT] > KEY_RESTAIN) {
				isRight = false;
				keyStroke[RIGHT] = 0;
			}
		}
		if (isLeft) {
			
			keyStroke[LEFT]++;
			if (keyStroke[LEFT] > KEY_RESTAIN) {
				isLeft = false;
				keyStroke[LEFT] = 0;
			}
		}
		if (isDown) {
			keyStroke[DOWN]++;
			if (keyStroke[DOWN] > KEY_RESTAIN) {
				isDown = false;
				keyStroke[DOWN] = 0;
			}
		}
		if (isRotateR&&isRotateL) {
			isRotateR = false;
			isRotateL = false;
		}
		if (isRotateR) {
			keyStroke[ROTATERIGHT]++;
			if (keyStroke[ROTATERIGHT] > KEY_RESTAIN) {
				isRotateR = false;
				keyStroke[ROTATERIGHT] = 0;
			}
		}
		if (isRotateL) {
			keyStroke[ROTATELEFT]++;
			if (keyStroke[ROTATELEFT] > KEY_RESTAIN) {
				isRotateL = false;
				keyStroke[ROTATELEFT] = 0;
			}
		}
		if (isDrop) {
			keyStroke[DROP]++;
			if (keyStroke[DROP] > KEY_RESTAIN) {
				isDrop = false;
				keyStroke[DROP] = 0;
				
			}
		}
		//if (isHold) {
		//	
		//}
	}

	if (isPause) {
	
		GameStop = true;
		isPause = false;
	}
	if (GameOver) {
		static int GameOverCnt = 0;
		GameOverCnt++;
		if(GameOverCnt>SWITCH_RESULT) {
			g_scene = STATE_RESULT;
			GameOverCnt = 0;
		}
	}
}


//Logo����
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
//���U���g�L�����A�j��
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