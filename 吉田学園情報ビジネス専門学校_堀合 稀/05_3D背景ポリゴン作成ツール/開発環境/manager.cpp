//==============================================================================
//
// �}�l�[�W���kmanager.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "texture.h"
#include "input.h"
#include "textdata.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "exe.h"

//==============================================================================
// �ÓI�����o�ϐ�
//==============================================================================
CRenderer *CManager::m_pRenderer = NULL;			// �����_�����O
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// �L�[�{�[�h
CCamera *CManager::m_pCamera = NULL;				// �J�����|�C���^
CLight *CManager::m_pLight = NULL;					// ���C�g�̃|�C���^
CTextData *CManager::m_pTextData = NULL;			// �e�L�X�g�f�[�^�̃|�C���^
CTexture *CManager::m_pTexture = NULL;				// �e�N�X�`���|�C���^
CExe *CManager::m_pExe = NULL;						// ���s�N���X
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
		m_pRenderer = CRenderer::Create(hWnd, bWindow);
	}

	// �f�o�b�O�\���̐���
	CDebugProc::Create();

	// �L�[�{�[�h����
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	}

	// �e�L�X�g�f�[�^�̓ǂݍ��݃N���X�̐���
	if (m_pTextData == NULL)
	{
		m_pTextData = CTextData::Create();
	}
	
	// �e�N�X�`���̓ǂݍ���
	if (m_pTexture == NULL)
	{
		m_pTexture = CTexture::Create();
	}

	// �J�����̐���
	if (m_pCamera == NULL)
	{
		m_pCamera = CCamera::Create();
	}

	// ���C�g�̐���
	if (m_pLight == NULL)
	{
		m_pLight = CLight::Create();
	}

	// ���s�N���X�̐���
	if (m_pExe == NULL)
	{
		m_pExe = CExe::Create();
	}
	
	// �}�E�X����
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}

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
		m_pInputKeyboard = NULL;
	}

	// �}�E�X�̔j��
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		m_pInputMouse = NULL;
	}
	
	// �e�L�X�g�f�[�^�̔j��
	if (m_pTextData != NULL)
	{
		m_pTextData->UnloadText();
		m_pTextData = NULL;
	}
	
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->UnloadAll();
		m_pTexture = NULL;
	}

	// �J�����̊J��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		m_pCamera = NULL;
	}

	// ���C�g�̊J��
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		m_pLight = NULL;
	}

	// ���s�N���X�̊J��
	if (m_pExe != NULL)
	{
		m_pExe->Uninit();
		m_pExe = NULL;
	}

	// �S�j��
	CScene::ReleaseAll();

	// �|�C���^�̊J��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
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

	// ���C�g�̍X�V
	if (m_pLight != NULL)
	{
		m_pLight->Update();
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CManager::Draw(void)
{
	// �����_���[�̕`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}