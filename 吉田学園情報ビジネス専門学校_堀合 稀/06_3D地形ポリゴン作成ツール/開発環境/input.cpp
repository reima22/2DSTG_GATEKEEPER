//==============================================================================
//
// ���͏����kinput.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "input.h"

//==============================================================================
// �ÓI�����o�ϐ�
//==============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInput::m_aKeyInfo[] =
{
	DIK_RETURN,
	DIK_W,			// �v���C���[�ړ�
	DIK_S,			// �v���C���[�ړ�
	DIK_A,			// �v���C���[�ړ�
	DIK_D,			// �v���C���[�ړ�
	DIK_Y,
	DIK_H,
	DIK_U,
	DIK_J,
	DIK_M,
	DIK_UP,			// �J�����ړ�
	DIK_DOWN,		// �J�����ړ�
	DIK_LEFT,		// �J�����ړ�
	DIK_RIGHT,		// �J�����ړ�
	DIK_SPACE,
	DIK_Z,			// �J��������
	DIK_C,			// �J��������
	DIK_X,			// �J�����̃��Z�b�g
	DIK_R,			// �J�����㏸
	DIK_F,			// �J�������~
	DIK_T,			// �Y�[���C��
	DIK_G,			// �Y�[���A�E�g
	DIK_B,			// �e�N�X�`���C���f�b�N�X���
	DIK_V,			// �e�N�X�`���C���f�b�N�X�i�s
	DIK_1,			// �e�N�X�`���̕���
	DIK_2,			// ���C���[�t���[������
	DIK_F1,			// �ۑ��R�}���h
	DIK_F2,			// �ҏW���[�h�̐؂�ւ�
	DIK_F3			// ���[�h�ؑ�
};
//DIMOUSESTATE2 CInputMouse::m_mouseState2;
//BYTE CInputMouse::m_aButton[MOUSEINFO_MAX] =
//{
//
//};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CInput::~CInput()
{
}

//==============================================================================
// ������
//==============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==============================================================================
// �I��
//==============================================================================
void CInput::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		// �A�N�Z�X���̔j��
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==============================================================================
// �L�[�{�[�h�̃R���X�g���N�^
//==============================================================================
CInputKeyboard::CInputKeyboard()
{
	// �L�[���̏�����
	memset(m_aKeyState,NULL,sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, NULL, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, NULL, sizeof(m_aKeyStateRelease));
}

// �L�[�{�[�h�̃f�X�g���N�^
CInputKeyboard::~CInputKeyboard()
{

}

//==============================================================================
// �L�[�{�[�h������
//==============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(
		GUID_SysKeyboard,
		&m_pDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �A�N�Z�X���̎擾
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================
// �L�[�{�[�h�I��
//==============================================================================
void CInputKeyboard::Uninit(void)
{
	// �I������
	CInput::Uninit();
}

//==============================================================================
// �L�[�{�[�h�X�V
//==============================================================================
void CInputKeyboard::Update(void)
{
	// ���[�J���ϐ��錾
	BYTE aKeyState[KEY_STATE];

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (int nKey = 0; nKey < KEY_STATE; nKey++)
		{
			m_aKeyStateTrigger[nKey] = (m_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];
			m_aKeyStateRelease[nKey] = (m_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];
			m_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		// �A�N�Z�X���̎擾
		m_pDevice->Acquire();
	}
}

// �L�[�{�[�h�̐���
CInputKeyboard *CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ���[�J���ϐ��錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = new CInputKeyboard;

	// ������
	if (pInputKeyboard != NULL)
	{
		pInputKeyboard->Init(hInstance, hWnd);
	}

	return pInputKeyboard;
}

//==============================================================================
// �v���X����
//==============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	// �L�[�{�[�h���̎擾
	return (m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

//==============================================================================
// �g���K�[����
//==============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	// �L�[�{�[�h���̎擾
	return (m_aKeyStateTrigger[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

//==============================================================================
// �����[�X����
//==============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	// �L�[�{�[�h���̎擾
	return (m_aKeyStateRelease[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

//==============================================================================
// ���s�[�g����
//==============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	if ((m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false == true)
	{
		m_nCountRepeat[nKey]++;
	}
	else
	{
		m_nCountRepeat[nKey] = 0;
	}

	// �L�[�{�[�h���̎擾
	if (m_nCountRepeat[nKey] == 1)
	{
		return (m_aKeyStateTrigger[m_aKeyInfo[nKey]] & 0x80) ? true : false;
	}
	else if (m_nCountRepeat[nKey] > REPEAT_CNT)
	{
		return (m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false;
	}
	else
	{
		return false;
	}
}

//==============================================================================
// �}�E�X�̃R���X�g���N�^
//==============================================================================
CInputMouse::CInputMouse()
{
	//// �L�[���̏�����
	//memset(m_aKeyState, NULL, sizeof(m_aKeyState));
	//memset(m_aKeyStateTrigger, NULL, sizeof(m_aKeyStateTrigger));
	//memset(m_aKeyStateRelease, NULL, sizeof(m_aKeyStateRelease));
}

// �}�E�X�̃f�X�g���N�^
CInputMouse::~CInputMouse()
{

}

//==============================================================================
// �}�E�X������
//==============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(
		GUID_SysMouse,
		&m_pDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(
		&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h�̐ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �A�N�Z�X���̎擾
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================
// �}�E�X�I��
//==============================================================================
void CInputMouse::Uninit(void)
{
	// �I������
	CInput::Uninit();
}

//==============================================================================
// �}�E�X�X�V
//==============================================================================
void CInputMouse::Update(void)
{
	// ���[�J���ϐ��錾
	//BYTE aMouseState[MOUSE_BOTTON_NUM];
	DIMOUSESTATE2 mouseState;

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(
		sizeof(mouseState),
		&mouseState)))
	{
		
		//DIMOUSESTATE m_mouseState;
		//DIMOUSESTATE m_mouseStateTrigger;
		//DIMOUSESTATE m_mouseStateRelease;
		//m_mouseStateTrigger = (m_mouseState & mouseState) ^ mouseState;
		//m_mouseStateRelease = (m_mouseState | mouseState) ^ mouseState;

		m_mouseState = mouseState;

		for (int nCntButton = 0; nCntButton < MOUSEINFO_MAX; nCntButton++)
		{
			m_aButton[nCntButton] = m_mouseState.rgbButtons[nCntButton];
		}
	}
	else
	{
		// �A�N�Z�X���̎擾
		m_pDevice->Acquire();
	}
}

//==============================================================================
// �}�E�X�̐���
//==============================================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ���[�J���ϐ��錾
	CInputMouse *pInputMouse;
	pInputMouse = new CInputMouse;

	// ������
	if (pInputMouse != NULL)
	{
		pInputMouse->Init(hInstance, hWnd);
	}

	return pInputMouse;
}

//==============================================================================
// �v���X����
//==============================================================================
bool CInputMouse::GetPress(int nKey)
{
	// �L�[�{�[�h���̎擾
	return (nKey & 0x80) ? true : false;
}

////==============================================================================
//// �g���K�[����
////==============================================================================
//bool CInputMouse::GetTrigger(int nKey)
//{
//	// �L�[�{�[�h���̎擾
//	return (nKey & 0x80) ? true : false;
//}
//
////==============================================================================
//// �����[�X����
////==============================================================================
//bool CInputMouse::GetRelease(int nKey)
//{
//	// �L�[�{�[�h���̎擾
//	return (nKey & 0x80) ? true : false;
//}