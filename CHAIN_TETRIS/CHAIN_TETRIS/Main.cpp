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


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

using std::map;
using std::string;

LPDIRECT3D9			  g_pDirect3D;		//	Direct3Dのインターフェイス
map<string, LPDIRECT3DTEXTURE9>	  g_pTexture;	//	画像の情報を入れておく為のポインタ配列
IDirect3DDevice9*	  g_pD3Device;		//	Direct3Dのデバイス
D3DDISPLAYMODE		  g_D3DdisplayMode;
LPDIRECTINPUT8		  g_pDinput = NULL;
LPDIRECTINPUTDEVICE8  g_pKeyDevice = NULL;
D3DPRESENT_PARAMETERS g_D3dPresentParameters;
map<string, LPD3DXFONT> g_pFont;			//Font
//SoundLib::SoundsManager soundsManager;
static const char* FONTPASS = "./venus rising.TTF";
static const char* FONTNAME = "venus rising";

//フルスクリーン関連
D3DPRESENT_PARAMETERS d3dppWin, d3dppFull;
RECT WinRect;			//Window Mode での位置大きさ
bool WinAct = false;
bool WinMode = true;	//true:Window　false:Full
bool DeviceLost = false;
HWND hWnd = NULL;
HANDLE th;
DWORD result;

void FreeDx();
void SetFont(int height, int width, string FontKey, LPCSTR fonttype= FONTNAME);
void ReadTexture(string TextureKey, LPCSTR FileName);
int EndGame();
void LoadTexture();
void LoadFont();

//DWORD WINAPI Thread(LPVOID *data)
//{
//	//LoadTexture();
//	//LoadFont();
//	static int a = 0;
//	char buf[1000];
//	while (a<20) {
//		//カウントをウィンドウタイトルに表示
//		a++;
//			sprintf_s(buf,1000, "%d", a);
//			SetWindowText(hWnd, buf);
//			//1000ミリ秒（1秒）おきにループ
//			Sleep(1000);
//	}
//	ExitThread(0);
//}
// 画面モードの変更
void ChangeDisplayMode(void)
{
	HRESULT hr = NULL;
	WinMode = !WinMode;
	//画面の消去
	g_pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	//描画の終了
	g_pD3Device->EndScene();
	//表示
	g_pD3Device->Present(NULL, NULL, NULL, NULL);

	if (WinMode) {
		g_D3dPresentParameters = d3dppWin;
	}
	else {
		g_D3dPresentParameters = d3dppFull;
		GetWindowRect(hWnd, &WinRect);
	}
	hr = g_pD3Device->Reset(&g_D3dPresentParameters);
	FreeDx();
	BuildDXDevice();
	//ファイルの引用
	LoadTexture();
	LoadFont();
	//ResumeThread(th);
	//soundsManager.Start("A", true);
	//GetExitCodeThread(th, &result);
	//if (result != STILL_ACTIVE) {
	//	soundsManager.Stop("A");
	//}
	if (FAILED(hr)&& result != STILL_ACTIVE) {
		if (hr == D3DERR_DEVICELOST) {
			DeviceLost = true;
		}
		if (hr == D3DERR_DRIVERINTERNALERROR) {
			DestroyWindow(hWnd);
		}
		return;
	}

	if (WinMode) {
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		SetWindowPos(hWnd, HWND_NOTOPMOST,
			WinRect.left, WinRect.top,
			WinRect.right - WinRect.left,
			WinRect.bottom - WinRect.top,
			SWP_SHOWWINDOW);
	}
	else {
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}
}
// ウインドウ・サイズの変更
HRESULT ChangeWindowSize(void)
{
	// ウインドウのクライアント領域に合わせる

	HRESULT hr = g_pD3Device->Reset(&g_D3dPresentParameters);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST) {
			DeviceLost = true;
		}
	}
	else {
		DestroyWindow(hWnd);
	}

	// ビューポートの設定
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = g_D3dPresentParameters.BackBufferWidth;
	vp.Height = g_D3dPresentParameters.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	hr = g_pD3Device->SetViewport(&vp);
	if (FAILED(hr)) {
		DestroyWindow(hWnd);
	}
	return hr;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SYSKEYDOWN:     // Alt + 特殊キーの処理に使う
		switch (wp) {
		case VK_RETURN:     // Alt + Enterを押すと切り替え
			ChangeDisplayMode();
			return 0;
		case VK_F4:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		default:
			return 0;
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

/**
*メモリ開放
*/
void FreeDx()
{
	RemoveFontResourceEx(FONTPASS, FR_PRIVATE, NULL);
	g_pTexture.clear();
	map<string, LPDIRECT3DTEXTURE9>().swap(g_pTexture);
	g_pFont.clear();
	map<string, LPD3DXFONT>().swap(g_pFont);

	if (g_pKeyDevice)
	{
		g_pKeyDevice->Unacquire();
	}
	SAFE_RELEASE(g_pKeyDevice);
	SAFE_RELEASE(g_pDinput);

	SAFE_RELEASE(g_pDirect3D);
	SAFE_RELEASE(g_pD3Device);

}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstance, LPSTR szStr, INT iCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MSG msg;
	WNDCLASS Wndclass;
	
	//soundsManager.Initialize();

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
	//Window
	ZeroMemory(&d3dppWin, sizeof(D3DPRESENT_PARAMETERS));
	d3dppWin.BackBufferWidth = 0;
	d3dppWin.BackBufferHeight = 0;
	d3dppWin.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dppWin.BackBufferCount = 1;
	d3dppWin.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dppWin.MultiSampleQuality = 0;
	d3dppWin.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dppWin.hDeviceWindow = hWnd;
	d3dppWin.Windowed = TRUE;
	d3dppWin.EnableAutoDepthStencil = FALSE;
	d3dppWin.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	d3dppWin.Flags = 0;
	d3dppWin.FullScreen_RefreshRateInHz = 0;
	d3dppWin.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//Full
	ZeroMemory(&d3dppFull, sizeof(D3DPRESENT_PARAMETERS));
	d3dppFull.BackBufferWidth = WIDTH;
	d3dppFull.BackBufferHeight = HEIGHT;
	d3dppFull.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dppFull.BackBufferCount = 1;
	d3dppFull.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dppFull.MultiSampleQuality = 0;
	d3dppFull.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dppFull.hDeviceWindow = hWnd;
	d3dppFull.Windowed = FALSE;
	d3dppFull.EnableAutoDepthStencil = FALSE;
	d3dppFull.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	d3dppFull.Flags = 0;
	d3dppFull.FullScreen_RefreshRateInHz = 0;
	d3dppFull.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread, NULL, 0, NULL);
	if (!hWnd) return 0;
	BuildDXDevice();
	//画像の引用
	LoadTexture();

	//文字の設定 
	LoadFont();
	//soundsManager.AddFile("Sound/思春期を殺した少年の翼.wav", "A");


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
			if (SyncNow - SyncOld >= 1000 / 60)
			{//60フレームループしたい処理
				//soundsManager.SetVolume("A", 50);
				//soundsManager.Start("A", true);
				//ResumeThread(th);
				//GetExitCodeThread(th, &result);
				//if (result != STILL_ACTIVE&&SoundLib::Playing==soundsManager.GetStatus("A")) {
				//	soundsManager.Stop("A");
				//}

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
	//CloseHandle(th);
	return (int)msg.wParam;
}

int EndGame() {

	if (KeyRelease == GetKeyBoardState(DIK_ESCAPE)) {
		return WM_QUIT;
	}
	return WM_NULL;

}

void SetFont(int height,int width, string FontKey,LPCSTR fonttype) {
	D3DXCreateFont(
		g_pD3Device,
		height,
		width,
		FW_DEMIBOLD,
		NULL,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_SCRIPT,
		fonttype,
		&g_pFont[FontKey]);
}

void ReadTexture(string TextureKey, LPCSTR FileName) {

	D3DXCreateTextureFromFile(
		g_pD3Device,
		FileName,
		&g_pTexture[TextureKey]);

}

void LoadTexture() {
	ReadTexture("BG_MAIN_TEX", "Texture/bg_main.png");
	ReadTexture("STAGE_TEX", "Texture/flame_main.png");
	ReadTexture("UNITS_TEX", "Texture/Empty_block.png");
	ReadTexture("BORD_TEX", "Texture/tetliminoBLOCK.png");
	ReadTexture("TEAM_LOGO_TEX", "Texture/Team_logo.png");
	ReadTexture("BLOCK_RED_TEX", "Texture/Red_block.png");
	ReadTexture("BLOCK_BLUE_TEX", "Texture/Blue_block.png");
	ReadTexture("BLOCK_GREEN_TEX", "Texture/Green_block.png");
	ReadTexture("BLOCK_YELLOW_TEX", "Texture/Yellow_block.png");
	ReadTexture("BLOCK_BRUNK_TEX", "Texture/Origin_block.png");
	ReadTexture("BG_TITLE_TEX", "Texture/title.png");
	ReadTexture("STATE_MAIN", "Texture/bg_main.png");
	ReadTexture("BG_RESULT_TEX", "Texture/bg_result.png");
	ReadTexture("LOGO_TEX", "Texture/title_chocolate.png");
	ReadTexture("START_TEX", "Texture/PRESS_ENTER_KEY.png");
	ReadTexture("CHAIN_COLOR_TEX", "Texture/chain_art.png");
	ReadTexture("TUTORIAL_TEX", "Texture/tutorial.png");
	ReadTexture("CHECK_TEX", "Texture/check.png");
	ReadTexture("RESULTGIRL_TEX", "Texture/result_girl.png");
	ReadTexture("EFFECT_TEX", "Texture/Effect_Erese.png");
}
void LoadFont() {
	AddFontResourceEx(FONTPASS, FR_PRIVATE, NULL);
	SetFont(100, 50, "MAIN_FONT");
	SetFont(18, 12, "SUBBOARD_FONT");
	SetFont(45, 25, "Score_FONT");
	SetFont(150, 100, "GO_FONT");
	SetFont(150, 100, "PAUSE_FONT");
	SetFont(30, 30, "PAUSE2_FONT");
	SetFont(80, 50, "ResultScore_FONT");

}