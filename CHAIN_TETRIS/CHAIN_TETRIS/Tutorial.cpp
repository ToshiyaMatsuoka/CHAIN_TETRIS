#include "Main.h"
#include "DX9.h"


bool LEFT = false;
bool RIGHT = false;
bool UP = false;
bool DOWN = false;
bool KEY_Z = false;
bool KEY_C = false;
bool SPACE = false;


void TutorialControl(){
	static bool SkipTTRL = false;
	static int keyCnt = 0;
	keyCnt++;
	//キーボードで押されているキーを調べ、対応する方向に移動させる
	HRESULT hr = g_pKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE))	{
		BYTE diks[256];
		g_pKeyDevice->GetDeviceState(sizeof(diks), &diks);


		if (diks[DIK_RIGHT] & 0x80){
			RIGHT = true;
		}
		if (diks[DIK_LEFT] & 0x80){
			LEFT = true;
		}
		if (diks[DIK_DOWN] & 0x80){
			DOWN = true;
		}
		if (diks[DIK_UP] & 0x80){
			UP = true;
		}
		if (diks[DIK_Z] & 0x80){
			KEY_Z = true;
		}
		if (diks[DIK_C] & 0x80){
			KEY_C = true;
		}
		if (diks[DIK_SPACE] & 0x80){
			SPACE = true;
		}
		static int firstTime = 0;
			firstTime++;
		if ((diks[DIK_RETURN] & 0x80)||(diks[DIK_NUMPADENTER] & 0x80)) {
			SkipTTRL = false;
			if (firstTime>5) {
				SkipTTRL = true;
				firstTime = 0;
			}
			
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

	if (GetAnalogL(ANALOGRIGHT))
	{
		for (int i = 0; i < 4; i++)
		{
			RIGHT = true;
		}
	}
	if (GetAnalogL(ANALOGLEFT))
	{
		for (int i = 0; i < 4; i++)
		{
			LEFT = true;
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
			DOWN = true;
		}
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
	if (!PadState[ButtonStart]&& keyCnt>25) {
		g_scene = STATE_MAIN;
		keyCnt = 0;
	}
	if (!PadState[ButtonBack]) {
		
	}
	if (!PadState[ButtonUP]) {

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
	if (!PadState[ButtonLB]) {

	}
	if (!PadState[ButtonRB]) {

	}

}
/**
*描画処理
*/
void TutorialRender(void)
{
	//チェック画像のy軸それぞれの設定の初期化
	static int g_r = 0;
	static int g_l = 0;
	static int g_u = 0;
	static int g_d = 0;
	static int g_c = 0;
	static int g_z = 0;
	static int g_s = 0;

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
	{ 275.f, 172.f + g_r, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f, 172.f + g_r , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f, 227.f + g_r, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f, 227.f + g_r, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX leftcheck[4]
	{
	{ 275.f,172.f + g_l, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + g_l , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + g_l, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + g_l, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX upcheck[4]
	{
	{ 275.f,172.f + g_u, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + g_u , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + g_u, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + g_u, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX downcheck[4]
	{
	{ 275.f, 172.f + g_d, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f, 172.f + g_d , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f, 227.f + g_d, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f, 227.f + g_d, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX key_ccheck[4]
	{
	{ 275.f, 172.f + g_c, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f, 172.f + g_c , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f, 227.f + g_c, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f, 227.f + g_c, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX key_zcheck[4]
	{
	{ 275.f,172.f + g_z, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + g_z , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + g_z, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + g_z, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};
	CUSTOMVERTEX spacecheck[4]
	{
	{ 275.f,172.f + g_s, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
	{ 340.f,172.f + g_s , 1.f,1.f, 0xFFFFFFFF, 1.f, 0.f },
	{ 340.f,227.f + g_s, 1.f,1.f, 0xFFFFFFFF, 1.f, 1.f },
	{ 275.f,227.f + g_s, 1.f,1.f, 0xFFFFFFFF, 0.f, 1.f }
	};




	//-----------------------------------------------------


	//画面の消去
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);


	//描画の開始
	g_pD3Device->BeginScene();

	g_pD3Device->SetTexture(0, g_pTexture[TUTORIAL_TEX]);
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, tutorial_bg, sizeof(CUSTOMVERTEX));

	if (RIGHT)
	{
		g_r = 0;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, rightcheck, sizeof(CUSTOMVERTEX));
	}
	if (LEFT)
	{
		g_l = 58;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, leftcheck, sizeof(CUSTOMVERTEX));
	}
	if (DOWN)
	{
		g_u = 116;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, upcheck, sizeof(CUSTOMVERTEX));
	}
	if (UP)
	{
		g_d = 174;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, downcheck, sizeof(CUSTOMVERTEX));

	}
	if (KEY_Z)
	{
		g_z = 232;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, key_zcheck, sizeof(CUSTOMVERTEX));

	}
	if (KEY_C)
	{
		g_c = 290;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, key_ccheck, sizeof(CUSTOMVERTEX));

	}
	if (SPACE)
	{
		g_s = 340;

		g_pD3Device->SetTexture(0, g_pTexture[CHECK_TEX]);
		g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, spacecheck, sizeof(CUSTOMVERTEX));

	}


	//描画の終了
	g_pD3Device->EndScene();

	//表示
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}
