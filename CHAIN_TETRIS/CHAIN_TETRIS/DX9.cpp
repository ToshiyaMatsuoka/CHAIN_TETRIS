#include "DX9.h"
#include "TetriminoState.h"

//ダイレクト3Dの初期化関数
HRESULT InitD3d(HWND hWnd)
{
	// 「Direct3D」オブジェクトの作成
	if (NULL == (g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	// 「DIRECT3Dデバイス」オブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	if (FAILED(g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp, &g_pD3Device)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &g_pD3Device)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(g_pD3Device, "Texture/bg_main.png", 100, 100, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &g_pTexture["Test"])))
	{
		MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//DirectInput
HRESULT InitDinput(HWND hWnd) {
	HRESULT hr;
	// 「DirectInput」オブジェクトの作成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDinput, NULL)))
	{
		return hr;
	}
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(hr = g_pDinput->CreateDevice(GUID_SysKeyboard,
		&g_pKeyDevice, NULL)))
	{
		return hr;
	}
	// デバイスをキーボードに設定
	if (FAILED(hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	// 協調レベルの設定
	if (FAILED(hr = g_pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	// デバイスを「取得」する
	g_pKeyDevice->Acquire();
	return S_OK;
}

XINPUT_STATE g_Xinput;
PADSTATE PadState[buttomindexMAX];
PADSTATE PadOldState[buttomindexMAX] = { PadOff };

void GetControl()
{
	XInputGetState(0, &g_Xinput);
}

PADSTATE GetButton(ButtonIndex index)
{
	return PadState[index];
}

bool GetAnalogL(Analog analogstate)
{
	switch (analogstate)
	{
	case ANALOGUP:
		if (g_Xinput.Gamepad.sThumbLY > -65535 / 4)
		{
			return true;
		}
		break;
	case ANALOGDOWN:
		if (g_Xinput.Gamepad.sThumbLY < 65535 / 4)
		{
			return true;
		}
		break;
	case ANALOGLEFT:
		if (g_Xinput.Gamepad.sThumbLX <-65535 / 4)
		{
			return true;
		}
		break;
	case ANALOGRIGHT:
		if (g_Xinput.Gamepad.sThumbLX >65535 / 4)
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

void CheckButtonState(WORD buttomID, int buttomindex)
{
	if (g_Xinput.Gamepad.wButtons&buttomID)
	{
		if (PadOldState[buttomindex] == PadOn)
		{
			PadState[buttomindex] = PadOn;
		}
		else
		{
			PadState[buttomindex] = PadPush;
		}

		PadOldState[buttomindex] = PadOn;
	}
	else
	{
		if (PadOldState[buttomindex] == PadOn)
		{
			PadState[buttomindex] = PadRelease;
		}
		else
		{
			PadState[buttomindex] = PadOff;
		}
		PadOldState[buttomindex] = PadOff;
	}

}

void BuildDXDevice() {
	//ダイレクト３Dの初期化関数を呼ぶ
	if (FAILED(InitD3d(hWnd)))
	{
		return;
	}
	//ダイレクトインプットの初期化関数を呼ぶ
	if (FAILED(InitDinput(hWnd)))
	{
		return;
	}

	//DirectX オブジェクトの生成
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//成功チェック
	if (g_pDirect3D == NULL)
	{
		//生成に失敗したら終了する
		return;
	}
	g_D3dPresentParameters = (WinMode) ? d3dppWin : d3dppFull;
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&g_D3dPresentParameters, &g_pD3Device);
	//生成チェック
	if (g_pD3Device == NULL)
	{
		//生成に失敗したらDirectXオブジェクトを開放して終了する
		g_pDirect3D->Release();
		return;
	}
	//描画設定
	g_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	g_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//g_pD3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//頂点に入れるデータを設定
	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
}
