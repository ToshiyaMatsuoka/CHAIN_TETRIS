#ifndef MAIN_H
#define MAIN_H
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <D3dx9core.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<Xinput.h>
#include <map>
#include "SoundsManager.h"


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib,"SoundLib.lib")

//画面サイズ
#define WIDTH 1280
#define HEIGHT 720

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

using std::map;
using std::string;

enum TEXTURE{
	BG_MAIN_TEX,
	BG_TITLE_TEX,
	BG_RESULT_TEX,	
	LOGO_TEX,
	START_TEX,
	OPTION_TEX,
	UNITS_TEX,
	STAGE_TEX,
	BORD_TEX,
	TEAM_LOGO_TEX,
	BLOCK_RED_TEX,
	BLOCK_BLUE_TEX,
	BLOCK_GREEN_TEX,
	BLOCK_YELLOW_TEX,
	BLOCK_BRUNK_TEX,
	CHAIN_COLOR_TEX,
	PAUSE_TEX,
	TUTORIAL_TEX,
	CHECK_TEX,
	RESULTGIRL_TEX,
	EFFECT_TEX,
	TEXMAX
};

enum FONT{
	MAIN_FONT,
	SUBBOARD_FONT,
	Score_FONT,
	GO_FONT,
	PAUSE_FONT,
	PAUSE2_FONT,
	ResultScore_FONT,
	FONTMAX
};
enum SCENE {
	STATE_TEAMLOGO,
	STATE_TITLE,
	STATE_TUTORIAL,
	STATE_MAIN,
	STATE_RESULT,
};

extern LPDIRECT3D9 g_pDirect3D;		//	Direct3Dのインターフェイス
extern map<string, LPDIRECT3DTEXTURE9>	  g_pTexture;	//	画像の情報を入れておく為のポインタ配列
extern IDirect3DDevice9*	  g_pD3Device;		//	Direct3Dのデバイス
extern D3DPRESENT_PARAMETERS g_D3dPresentParameters;

extern LPDIRECTINPUT8 g_pDinput ;
extern LPDIRECTINPUTDEVICE8 g_pKeyDevice ;
extern map<string, LPD3DXFONT> g_pFont;	//Font
//extern SoundLib::SoundsManager soundsManager;

extern HWND hWnd;
extern HANDLE th;
//フルスクリーン関連
extern D3DPRESENT_PARAMETERS d3dppWin, d3dppFull;
extern bool WinMode;	//true:Window　false:Full

#endif // !MAIN_H

