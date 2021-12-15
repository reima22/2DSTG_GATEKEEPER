//==============================================================================
//
// �f�o�b�O�\�������kdebugproc.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "main.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "gamepad.h"
#include "meshfield.h"
#include "exe.h"
#include "camera.h"

// �ÓI�����o�ϐ��錾
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[DEBUG_TEXT_NUM];
char *CDebugProc::m_apOnOff[2] =
{
	"OFF",
	"ON"
};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CDebugProc::CDebugProc()
{
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CDebugProc::~CDebugProc()
{
}

//==============================================================================
// ����������
//==============================================================================
void CDebugProc::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �t�H���g�̍쐬
	D3DXCreateFont(
		pDevice, 20, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	// ������o�b�t�@�̏�����
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//==============================================================================
// �I������
//==============================================================================
void CDebugProc::Uninit(void)
{
	// �t�H���g���̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//==============================================================================
// �v�����g����
//==============================================================================
void CDebugProc::Print(const char *fmt,...)
{
	// ���[�J���ϐ��錾
	const char *pText = fmt;
	char aStr[DEBUG_TEXT_NUM];
	memset(&aStr[0], 0, sizeof(aStr));

	// �ό�����
	va_list args;

	// �J�n
	va_start(args, fmt);

	for (int nCnt = 0; *pText != '\0'; pText++) 
	{
		// �؂�o���Ə���
		switch (*pText)
		{
		case '%':

			pText++;

			switch (*pText)
			{
			case 'd':	// ����
				nCnt += wsprintf(&aStr[nCnt], "%d", va_arg(args, int));
				break;

			case 'f':	// ��������
				nCnt += sprintf(&aStr[nCnt], "%.3f", va_arg(args, double));
				break;

			case 'c':	// ����
				nCnt += wsprintf(&aStr[nCnt], "%c", va_arg(args, char));
				break;

			case 's':	// ������
				nCnt += wsprintf(&aStr[nCnt], "%s", va_arg(args, const char*));
				break;
				
			default:
				break;
			}
			break;

		default:	// ��O
			nCnt += wsprintf(&aStr[nCnt], "%c", (char*)pText[0]);
			break;
		}
	}

	va_end(args);

	// ������̘A��
	strcat(m_aStr,aStr);
}

//==============================================================================
// �`�揈��
//==============================================================================
void CDebugProc::Draw(void)
{
	// ���[�J���ϐ��錾
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	CPlayer *pPlayer = CExe::GetPlayer();

	// �L�[�{�[�h
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CGamepad *gamepad = CManager::GetInputGamepad();

	// �}�E�X�擾
	CInputMouse *pMouse = CManager::GetInputMouse();

	// �J����
	CCamera *pCamera = CManager::GetCamera();

	CMeshfield *pMeshfield = CExe::GetMeshField();

	CExe *pExe = CManager::GetExe();
	CExe::EDITMODE mode = CExe::GetEditMode();

	char *pMode = pExe->GetModeChar();

	int nCountFPS = GetFPSCnt();			// FPS�J�E���g�̎擾
	int nNumAll = CScene::GetNumAll();

	bool bTexCut = pMeshfield->GetTexCut();
	bool bWireFrame = pMeshfield->GetWireFrame();
	int nTexIdx = pMeshfield->GetTexIdx();
	float fEditRadius = pMeshfield->GetEditArea();
	int nCntRepeat = keyboard->GetCntRepeat(CInput::KEYINFO_MESH_UP);

	D3DXVECTOR3 movePlayer;
	D3DXVECTOR3 posPlayer;

	D3DXVECTOR3 posCameraR = pCamera->GetPositionR();
	D3DXVECTOR3 posCameraV = pCamera->GetPositionV();
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();
	float fLength =  pCamera->GetLength();
	float fGroundLength = pCamera->GetGroundLength();


	// NULL�`�F�b�N
	if (pPlayer != NULL)
	{
		posPlayer = pPlayer->GetPosition();
		movePlayer = pPlayer->GetMove();
	}

	// �ėp
	Print("FPS:%d\n�I�u�W�F�N�g��:%d\n", nCountFPS, nNumAll);
	Print("�f�[�^�̕ۑ��@�@  :[F1]\n");
	Print("�f�[�^�̓ǂݍ��݁@:[F2]\n");
	Print("�ҏW���[�h�؂�ւ�:[F3] %s\n", pMode);
	Print("�v���C���[�̑���:[WASD]\n");
	Print("�v���C���[�̈ʒu�@�FX %f, Y %f, Z %f\n", posPlayer.x, posPlayer.y, posPlayer.z);
	Print("�J�����̒����_�ʒu�FX %f, Y %f, Z %f\n", posCameraR.x, posCameraR.y, posCameraR.z);
	Print("�J�����̎��_�ʒu�@�FX %f, Y %f, Z %f\n", posCameraV.x, posCameraV.y, posCameraV.z);
	Print("�J�����̊p�x    �@�FX %f, Y %f, Z %f\n", rotCamera.x, rotCamera.y, rotCamera.z);
	Print("���_�Ԃ̋����@�@�@�F%f\n", fLength);
	Print("�n��̋����@�@�@�@�F%f\n", fGroundLength);
	Print("�J�����̏㏸:[R] ���~:[F]\n");
	Print("�J�����̃��Z�b�g:[X]\n");

	Print("\n�}�E�X�@lX:%d lY:%d lZ:%d \n", 
		pMouse->GetMouseState());

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_LEFT)) == true)
	{
		Print("\n�u���N���b�N�v\n");
	}
	
	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_RIGHT)) == true)
	{
		Print("\n�u�E�N���b�N�v\n");
	}

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_WHEEL)) == true)
	{
		Print("\n�u�z�C�|���v\n");
	}

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_BBACK)) == true)
	{
		Print("\n�u�u���E�U�o�b�N�v\n");
	}

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_MOVEON)) == true)
	{
		Print("\n�u�i�ށv\n");
	}

	// ���[�h��
	switch (mode)
	{
	case CExe::EDITMODE_EDIT:
		Print("\n�J�����̈ړ�:[��������]\n");
		Print("�ҏW�G���A �g��:[Y] �k��:[H]\n");
		Print("�ҏW�G���A�̔��a:%f\n", fEditRadius);
		Print("\n���_���W����\n�㏸:[U] ���~:[J]\n");
		Print("���Z�b�g:[M]\n");
		Print("\n�e�N�X�`���C���f�b�N�X:%d\n���:[V] �O�i:[B]\n", nTexIdx);
		Print("�e�N�X�`���̕���:      [1] %s\n", m_apOnOff[(int)bTexCut]);
		Print("���C���[�t���[���̕\��:[2] %s\n", m_apOnOff[(int)bWireFrame]);
		break;

	case CExe::EDITMODE_PREVIEW:

	
		break;
	}

	// �e�L�X�g�̕`��
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// ������o�b�t�@�̏�����
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//==============================================================================
// ��������
//==============================================================================
CDebugProc *CDebugProc::Create(void)
{
	// �C���X�^���X����
	CDebugProc *pDebugProc;
	pDebugProc = new CDebugProc;

	// ������
	if (pDebugProc != NULL)
	{
		pDebugProc->Init();
	}

	return pDebugProc;
}