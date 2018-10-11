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

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "Xinput.lib")
//画面サイズ
#define WIDTH 1280
#define HEIGHT 720

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
extern LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];	//	画像の情報を入れておく為のポインタ配列
extern IDirect3DDevice9*	  g_pD3Device;		//	Direct3Dのデバイス

extern LPDIRECTINPUT8 g_pDinput ;
extern LPDIRECTINPUTDEVICE8 g_pKeyDevice ;
extern LPD3DXFONT	g_pFont[FONTMAX];			//Font

#endif // !MAIN_H

