#include "DX9.h"
#include "TetriminoState.h"


//�_�C���N�g3D�̏������֐�
HRESULT InitD3d(HWND hWnd)
{
	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (NULL == (g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}
	// �uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
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
		MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
		if (FAILED(g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &g_pD3Device)))
		{
			MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
			return E_FAIL;
		}
	}
	//�u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(g_pD3Device, "Texture/bg_main.png", 100, 100, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &g_pTexture[TEXMAX])))
	{
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	

	return S_OK;
}


//DirectInput
HRESULT InitDinput(HWND hWnd) {
	HRESULT hr;
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDinput, NULL)))
	{
		return hr;
	}
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(hr = g_pDinput->CreateDevice(GUID_SysKeyboard,
		&g_pKeyDevice, NULL)))
	{
		return hr;
	}
	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	// �������x���̐ݒ�
	if (FAILED(hr = g_pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	// �f�o�C�X���u�擾�v����
	g_pKeyDevice->Acquire();
	return S_OK;
}

//�}�E�X
HRESULT InitDinputMouse(HWND hWnd)
{
	HRESULT hr;
	if (FAILED(hr = g_pDinput->CreateDevice(
		GUID_SysMouse,
		&g_pDIMouse,
		NULL)))
	{
		return hr;
	}

	if (FAILED(hr = g_pDIMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	if (FAILED(hr = g_pDIMouse->SetCooperativeLevel(hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return hr;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(hr = g_pDIMouse->SetProperty(
		DIPROP_AXISMODE,
		&diprop.diph)))
	{
		return hr;
	}

	g_pDIMouse->Acquire();

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