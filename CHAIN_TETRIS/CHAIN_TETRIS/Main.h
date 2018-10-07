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



//��ʃT�C�Y
#define WIDTH 1280
#define HEIGHT 720



//�t�B�[���h�̏c���E����
#define FIELD_HEIGHT	24
#define FIELD_WIDTH		12
//�t�B�[���h���[
#define FieldInitPosX    500.f
#define FieldInitPosY    -64.f
//�T�u�{�[�h
#define SUBBOARD_LX 200.f
#define SUBBOARD_RX 1000.f
#define NEXTBOARD_Y 100.f
#define NEXT2BOARD_Y 250.f
#define NEXT3BOARD_Y  400.f
#define HOLDBOARD_Y 100.f
#define SCOREBOARD_Y 300.f
//�u���b�N
#define BLOCK_HARFWIDTH 16.f
//�J���[�Z�b�g
#define RED 0xffff0000
#define BLUE 0xff0000ff
#define LIME 0xff00ff00
#define YELLOW 0xffffff00
#define MAGENTA 0xffff00ff
#define CYAN 0xff00ffff
#define ORANGE 0xffffa500
#define BLACK 0x60000000
#define WHITE 0x00ffffff
#define INVISIBLE 0x00ffffff
#define BASEED 0xffffffff
//�e�g���~�m�ړ��֘A
#define EFFECTTIME 45
#define SWITCH_RESULT 80
//�L�[���͐���
#define CREATE_INTERVAL 5
#define MOVING_COUNTER 10
#define KEY_RESTAIN 5
//�^�C�g�����S������
#define FALL_SPEED 5.f

enum TEXTURE
{
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

enum FONT
{
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

struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

struct TEXTURE_STATE
{
	float x, y, scale;
};
struct STAGE_STATE
{
	float x, y, Xscale,Yscale;
};


extern LPDIRECT3D9 g_pDirect3D;		//	Direct3D�̃C���^�[�t�F�C�X
extern LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];	//	�摜�̏������Ă����ׂ̃|�C���^�z��
extern IDirect3DDevice9*	  g_pD3Device;		//	Direct3D�̃f�o�C�X

extern LPDIRECTINPUT8 g_pDinput ;
extern LPDIRECTINPUTDEVICE8 g_pKeyDevice ;
extern LPDIRECTINPUTDEVICE8 g_pDIMouse;
extern D3DPRESENT_PARAMETERS g_D3dPresentParameters;
extern LPD3DXFONT	g_pFont[FONTMAX];			//Font


extern LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern HRESULT InitD3d(HWND);
extern HRESULT InitDinput(HWND);

VOID FreeDx();


#endif // !MAIN_H

