#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <D3dx9core.h>
#include "Main.h"
#include "Render.h"
#include "Control.h"
#include "DX9.h"
#include "TetriminoState.h"
#include "Gamescene.h"
#include "Tutorial.h"
#include "resource.h"
#include "teamlogo.h"
#include <crtdbg.h>
#include "SoundsManager.h"


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



LPDIRECT3D9			  g_pDirect3D;		//	Direct3Dのインターフェイス
LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];	//	画像の情報を入れておく為のポインタ配列
IDirect3DDevice9*	  g_pD3Device;		//	Direct3Dのデバイス
D3DDISPLAYMODE		  g_D3DdisplayMode;
LPDIRECTINPUT8		  g_pDinput = NULL;
LPDIRECTINPUTDEVICE8  g_pKeyDevice = NULL;
LPDIRECTINPUTDEVICE8  g_pDIMouse = NULL;
D3DPRESENT_PARAMETERS g_D3dPresentParameters;
LPD3DXFONT	g_pFont[FONTMAX];			//Font


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void FreeDx();
int EndGame();

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

/**
*メモリ開放
*/
void FreeDx()
{
	for (int i = 0; i <= TEXMAX; i++)
	{

		SAFE_RELEASE(g_pTexture[i]);
		
	}
	for (int i = 0; i < FONTMAX; i++) {
		SAFE_RELEASE(g_pFont[i]);
	}
	if (g_pKeyDevice)
	{
		g_pKeyDevice->Unacquire();
	}
	SAFE_RELEASE(g_pKeyDevice);
	SAFE_RELEASE(g_pDIMouse);
	SAFE_RELEASE(g_pDinput);

	SAFE_RELEASE(g_pDirect3D);
	SAFE_RELEASE(g_pD3Device);

}



INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstance, LPSTR szStr, INT iCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HWND hWnd = NULL;
	MSG msg;
	WNDCLASS Wndclass;
	static bool GAMEOPEN = true;

	//Windows情報の設定
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInst;
	Wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	Wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = TEXT("CHAIN TETRIS");	//クラス名

	RegisterClass(&Wndclass);//Windowの登録

	//Windowの生成
	hWnd = CreateWindow(
		TEXT("CHAIN TETRIS"),						//ウィンドウのクラス名
		TEXT("CHAIN TETLIS"),  				//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//ウィンドウスタイル
		CW_USEDEFAULT,						// ウィンドウの横方向の位置x
		CW_USEDEFAULT,						// ウィンドウの縦方向の位置y
		WIDTH,							// Width（幅）
		HEIGHT,							// Height（高さ）
		NULL,
		NULL,
		hInstance,							// アプリケーションインスタンスのハンドル
		NULL
	);

	if (!hWnd) return 0;
	//ダイレクト３Dの初期化関数を呼ぶ
	if (FAILED(InitD3d(hWnd)))
	{
		return 0;
	}
	//ダイレクトインプットの初期化関数を呼ぶ
	if (FAILED(InitDinput(hWnd)))
	{
		return 0;
	}

	//DirectX オブジェクトの生成
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//成功チェック
	if (g_pDirect3D == NULL)
	{
		//生成に失敗したら終了する
		return 0;
	}


	//Display Mode の設定
	g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&g_D3DdisplayMode);

	ZeroMemory(&g_D3dPresentParameters,
		sizeof(D3DPRESENT_PARAMETERS));
	g_D3dPresentParameters.BackBufferFormat = g_D3DdisplayMode.Format;
	g_D3dPresentParameters.BackBufferCount = 1;
	g_D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3dPresentParameters.Windowed = TRUE;

	//ZeroMemory(&g_D3dPresentParameters, sizeof(D3DPRESENT_PARAMETERS));
	//g_D3dPresentParameters.BackBufferWidth = WIDTH;
	//g_D3dPresentParameters.BackBufferHeight = HEIGHT;
	//g_D3dPresentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	//g_D3dPresentParameters.BackBufferCount = 1;
	//g_D3dPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	//g_D3dPresentParameters.MultiSampleQuality = 0;
	//g_D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//g_D3dPresentParameters.hDeviceWindow = hWnd;
	//g_D3dPresentParameters.Windowed = FALSE;
	//g_D3dPresentParameters.EnableAutoDepthStencil = FALSE;
	//g_D3dPresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	//g_D3dPresentParameters.Flags = 0;
	//g_D3dPresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//g_D3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//デバイスを作る
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_D3dPresentParameters, &g_pD3Device);


	int MAX_CONTROLLERS = 1;

	//生成チェック
	if (g_pD3Device == NULL)
	{
		//生成に失敗したらDirectXオブジェクトを開放して終了する
		g_pDirect3D->Release();
		return 0;
	}

	//描画設定
	g_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	g_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//頂点に入れるデータを設定
	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);

	//画像の引用
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/bg_main.png",
		&g_pTexture[BG_MAIN_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/flame_main.png",
		&g_pTexture[STAGE_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Empty_block.png",
		&g_pTexture[UNITS_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/tetliminoBLOCK.png",
		&g_pTexture[BORD_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Team_logo.png",
		&g_pTexture[TEAM_LOGO_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Red_block.png",
		&g_pTexture[BLOCK_RED_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Blue_block.png",
		&g_pTexture[BLOCK_BLUE_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Green_block.png",
		&g_pTexture[BLOCK_GREEN_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Yellow_block.png",
		&g_pTexture[BLOCK_YELLOW_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Origin_block.png",
		&g_pTexture[BLOCK_BRUNK_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/title.png",
		&g_pTexture[BG_TITLE_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/bg_main.png",
		&g_pTexture[STATE_MAIN]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/bg_result.png",
		&g_pTexture[BG_RESULT_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/title_chocolate.png",
		&g_pTexture[LOGO_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/PRESS_ENTER_KEY.png",
		&g_pTexture[START_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/chain_art.png",
		&g_pTexture[CHAIN_COLOR_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/tutorial.png",
		&g_pTexture[TUTORIAL_TEX]);

	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/check.png",
		&g_pTexture[CHECK_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/result_girl.png",
		&g_pTexture[RESULTGIRL_TEX]);
	D3DXCreateTextureFromFile(
		g_pD3Device,
		"Texture/Effect_Erese.png",
		&g_pTexture[EFFECT_TEX]);

	//文字の設定 
	D3DXCreateFont(
		g_pD3Device,				// Direct3Dデバイス
		100,						// 高さ
		50,							// 幅
		FW_REGULAR,					// フォントの太さ 普通
		NULL,						// 下線
		FALSE,						// 斜体
		DEFAULT_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		DEFAULT_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("VENUS RISING"),		// フォント名
		&g_pFont[MAIN_FONT]);					// ID3DXFontポインタ

	D3DXCreateFont(
		g_pD3Device,
		18,
		12,
		FW_DEMIBOLD,
		NULL,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_SCRIPT,
		TEXT("VENUS RISING"),
		&g_pFont[SUBBOARD_FONT]);

	D3DXCreateFont(
		g_pD3Device,				// Direct3Dデバイス
		45,						// 高さ
		25,							// 幅
		FW_BLACK,					// フォントの太さ 普通
		NULL,						// 下線
		FALSE,						// 斜体
		DEFAULT_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		DEFAULT_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("venus rising"),		// フォント名
		&g_pFont[Score_FONT]);					// ID3DXFontポインタ

	D3DXCreateFont(
		g_pD3Device,				// Direct3Dデバイス
		150,						// 高さ
		100,							// 幅
		FW_BLACK,					// フォントの太さ 普通
		NULL,						// 下線
		FALSE,						// 斜体
		DEFAULT_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		DEFAULT_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("VENUS RISING"),		// フォント名
		&g_pFont[GO_FONT]);					// ID3DXFontポインタ
	D3DXCreateFont(
		g_pD3Device,
		150,
		100,
		FW_DEMIBOLD,
		NULL,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_SCRIPT,
		TEXT("VENUS RISING"),
		&g_pFont[PAUSE_FONT]);

	D3DXCreateFont(
		g_pD3Device,
		30,
		30,
		FW_DEMIBOLD,
		NULL,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_SCRIPT,
		TEXT("VENUS RISING"),
		&g_pFont[PAUSE2_FONT]);
	D3DXCreateFont(
		g_pD3Device,				// Direct3Dデバイス
		80,						// 高さ
		50,							// 幅
		FW_BLACK,					// フォントの太さ 普通
		NULL,						// 下線
		FALSE,						// 斜体
		DEFAULT_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		DEFAULT_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("venus rising"),		// フォント名
		&g_pFont[ResultScore_FONT]);

	DWORD SyncOld = timeGetTime();	//	システム時間を取得
	DWORD SyncNow;

	srand((unsigned int)time(NULL));
	teamlogorandam = rand() % 3 + 1;


	timeBeginPeriod(1);
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / 60) //	1秒間に60回この中に入るはず
			{//60フレームループしたい処理
				
				switch (g_scene){
				case STATE_TEAMLOGO:
					TeamLogoControl();
					TeamLogoRender();
					break;
				case STATE_TITLE:
					msg.message = EndGame();
					Control();
					Render();
					break;
				case STATE_MAIN:
					if (!GameOver) {
						Gamescene();
					}
					Control();
					Render();
					break;
				case STATE_RESULT:
					Control();
					ResultControl();
					Render();
					SelectTetrimino();
					CreateField();
					CreateBoard();
					GameOver = false;
					break;
				case STATE_TUTORIAL:
					Logo = { 320.f, -300.f, 100.f };
					TutorialControl();
					TutorialRender();
					break;
				}
				
				SyncOld = SyncNow;
			}
		}
	}
	timeEndPeriod(1);

	FreeDx();
	return (int)msg.wParam;
}

int EndGame() {

	if (KeyRelease == GetKeyBoardState(DIK_ESCAPE)) {
		return WM_QUIT;
	}
	return WM_NULL;

}