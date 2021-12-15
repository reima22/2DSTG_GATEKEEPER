//==============================================================================
//
// �}�l�[�W���kmanager.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "gamepad.h"
#include "textdata_player.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "collision.h"
#include "exe.h"

//==============================================================================
// �ÓI�����o�ϐ�
//==============================================================================
CRenderer *CManager::m_pRenderer = NULL;			// �����_�����O
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// �L�[�{�[�h
CGamepad *CManager::m_pGamepad = NULL;				// �Q�[���p�b�h
CCamera *CManager::m_pCamera = NULL;				// �J�����|�C���^
CLight *CManager::m_pLight;							// ���C�g�̃|�C���^
CTextData *CManager::m_pTextData = NULL;			// �e�L�X�g�f�[�^�̃|�C���^
CTexture *CManager::m_pTexture = NULL;				// �e�N�X�`���|�C���^
CCollision *CManager::m_pCollision = NULL;			// �����蔻��̃|�C���^
CExe *CManager::m_pExe = NULL;						// ���s�N���X�|�C���^
CInputMouse *CManager::m_pInputMouse = NULL;		// �}�E�X

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CManager::CManager()
{
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CManager::~CManager()
{
}

//==============================================================================
// ������
//==============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �C���X�^���X�Ə�����
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, bWindow);
	}



	// �f�o�b�O�\���̐���
	CDebugProc::Create();

	// �J�����̐���
	m_pCamera = CCamera::Create();

	// ���C�g�̐���
	m_pLight = CLight::Create();

	// �L�[�{�[�h����
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// �Q�[���p�b�h����
	m_pGamepad = new CGamepad;
	m_pGamepad->Init(hInstance, hWnd);

	// �}�E�X����
	m_pInputMouse = CInputMouse::Create(hInstance, hWnd);

	// �e�L�X�g�f�[�^�̓ǂݍ��݃N���X�̐���
	m_pTextData = CTextData::Create();

	// �e�N�X�`���̓ǂݍ���
	m_pTexture = CTexture::Create();

	// �����蔻��̐���
	m_pCollision = CCollision::Create();
	
	// ���s�N���X
	m_pExe = CExe::Create();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CManager::Uninit(void)
{
	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �Q�[���p�b�h�̔j��
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Uninit();
		delete m_pGamepad;
		m_pGamepad = NULL;
	}
	
	// �e�L�X�g�f�[�^�̔j��
	if (m_pTextData != NULL)
	{
		m_pTextData->UninitAll();
		m_pTextData = NULL;
	}
	
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->UnloadAll();
		//delete m_pTexture;
		m_pTexture = NULL;
	}

	// �J�����̊J��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		//delete m_pCamera;
		m_pCamera = NULL;
	}

	// ���C�g�̊J��
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		//delete m_pLight;
		m_pLight = NULL;
	}

	// �����蔻��̔j��
	if(m_pCollision != NULL)
	{
		m_pCollision = NULL;
	}

	// �S�j��
	CScene::ReleaseAll();

	// �|�C���^�̊J��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h�̍X�V
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �Q�[���p�b�h�̍X�V
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Update();
	}

	// �}�E�X�̍X�V
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	// �����_���[�̍X�V
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// �J�����̍X�V
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}