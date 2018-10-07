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



LPDIRECT3D9			  g_pDirect3D;		//	Direct3D�̃C���^�[�t�F�C�X
LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];	//	�摜�̏������Ă����ׂ̃|�C���^�z��
IDirect3DDevice9*	  g_pD3Device;		//	Direct3D�̃f�o�C�X
D3DDISPLAYMODE		  g_D3DdisplayMode;
LPDIRECTINPUT8		  g_pDinput = NULL;
LPDIRECTINPUTDEVICE8  g_pKeyDevice = NULL;
LPDIRECTINPUTDEVICE8  g_pDIMouse = NULL;
D3DPRESENT_PARAMETERS g_D3dPresentParameters;
LPD3DXFONT	g_pFont[FONTMAX];			//Font


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void FreeDx();
int EndGame();

//�E�B���h�E�v���V�[�W��
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
*�������J��
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

	//Windows���̐ݒ�
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInst;
	Wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	Wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = TEXT("CHAIN TETRIS");	//�N���X��

	RegisterClass(&Wndclass);//Window�̓o�^

	//Window�̐���
	hWnd = CreateWindow(
		TEXT("CHAIN TETRIS"),						//�E�B���h�E�̃N���X��
		TEXT("CHAIN TETLIS"),  				//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,						// �E�B���h�E�̉������̈ʒux
		CW_USEDEFAULT,						// �E�B���h�E�̏c�����̈ʒuy
		WIDTH,							// Width�i���j
		HEIGHT,							// Height�i�����j
		NULL,
		NULL,
		hInstance,							// �A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL
	);

	if (!hWnd) return 0;
	//�_�C���N�g�RD�̏������֐����Ă�
	if (FAILED(InitD3d(hWnd)))
	{
		return 0;
	}
	//�_�C���N�g�C���v�b�g�̏������֐����Ă�
	if (FAILED(InitDinput(hWnd)))
	{
		return 0;
	}

	//DirectX �I�u�W�F�N�g�̐���
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//�����`�F�b�N
	if (g_pDirect3D == NULL)
	{
		//�����Ɏ��s������I������
		return 0;
	}


	//Display Mode �̐ݒ�
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

	//�f�o�C�X�����
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_D3dPresentParameters, &g_pD3Device);


	int MAX_CONTROLLERS = 1;

	//�����`�F�b�N
	if (g_pD3Device == NULL)
	{
		//�����Ɏ��s������DirectX�I�u�W�F�N�g���J�����ďI������
		g_pDirect3D->Release();
		return 0;
	}

	//�`��ݒ�
	g_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRC�̐ݒ�
	g_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//���_�ɓ����f�[�^��ݒ�
	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);

	//�摜�̈��p
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

	//�����̐ݒ� 
	D3DXCreateFont(
		g_pD3Device,				// Direct3D�f�o�C�X
		100,						// ����
		50,							// ��
		FW_REGULAR,					// �t�H���g�̑��� ����
		NULL,						// ����
		FALSE,						// �Α�
		DEFAULT_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		DEFAULT_QUALITY,			// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("VENUS RISING"),		// �t�H���g��
		&g_pFont[MAIN_FONT]);					// ID3DXFont�|�C���^

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
		g_pD3Device,				// Direct3D�f�o�C�X
		45,						// ����
		25,							// ��
		FW_BLACK,					// �t�H���g�̑��� ����
		NULL,						// ����
		FALSE,						// �Α�
		DEFAULT_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		DEFAULT_QUALITY,			// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("venus rising"),		// �t�H���g��
		&g_pFont[Score_FONT]);					// ID3DXFont�|�C���^

	D3DXCreateFont(
		g_pD3Device,				// Direct3D�f�o�C�X
		150,						// ����
		100,							// ��
		FW_BLACK,					// �t�H���g�̑��� ����
		NULL,						// ����
		FALSE,						// �Α�
		DEFAULT_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		DEFAULT_QUALITY,			// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("VENUS RISING"),		// �t�H���g��
		&g_pFont[GO_FONT]);					// ID3DXFont�|�C���^
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
		g_pD3Device,				// Direct3D�f�o�C�X
		80,						// ����
		50,							// ��
		FW_BLACK,					// �t�H���g�̑��� ����
		NULL,						// ����
		FALSE,						// �Α�
		DEFAULT_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		DEFAULT_QUALITY,			// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("venus rising"),		// �t�H���g��
		&g_pFont[ResultScore_FONT]);

	DWORD SyncOld = timeGetTime();	//	�V�X�e�����Ԃ��擾
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
			if (SyncNow - SyncOld >= 1000 / 60) //	1�b�Ԃ�60�񂱂̒��ɓ���͂�
			{//60�t���[�����[�v����������
				
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