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
#include "meshfield.h"
#include "exe.h"

// �ÓI�����o�ϐ��錾
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[DEBUG_TEXT_NUM];

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

	CMeshfield *pMeshfield = CExe::GetMeshField();

	int nCountFPS = GetFPSCnt();	// FPS�J�E���g�̎擾

	char *pWaveType = pMeshfield->GetWaveTypeChar();
	char *pSyntheticType = pMeshfield->GetSyntheticTypeChar();
	char *pTex = pMeshfield->GetTexChar();
	char *pWireONOFF = pMeshfield->GetWireONOFF();

	float fHeightWave = pMeshfield->GetHeightWave();
	float fDistanceWave = pMeshfield->GetDistanceWave();
	float fSpeedWave = pMeshfield->GetSpeedWave();
	bool bCutTex = pMeshfield->GetCutTex();
	bool bWireFrame = pMeshfield->GetWireFrame();
	int nTexInfo = pMeshfield->GetTex();
	float fTexMoveRot = pMeshfield->GetTexMoveRot(nTexInfo);
	float fTexMove = pMeshfield->GetTexMove(nTexInfo);
	int nTexIdx = pMeshfield->GetTexIdx(nTexInfo);

	Print("FPS:%d\n", nCountFPS);
	Print("�J�����̈ړ��F[WASD]\n");
	Print("���_�̉�]�@��:[Z] �E:[C]\n");
	Print("�Y�[���C��:[T] �Y�[���A�E�g:[G]\n");
	Print("���b�V�����̏����o��: [F1]\n");
	Print("�g�̐؂�ւ�:              [1] �g�̎��:%s\n", pWaveType);
	Print("�����؂�ւ�:              [2] �������:%s\n", pSyntheticType);
	Print("�����؂�ւ�:              [3] �����L��:%d\n", (int)bCutTex);
	Print("����e�N�X�`���؂�ւ�:    [4]\n");
	Print("���C���[�t���[���̐؂�ւ�:[5] %s\n", pWireONOFF);
	Print("�g�̍��� +:[Y] -:[H]\n");
	Print("�g�`�Ԋu +:[U] -:[J]\n");
	Print("�g�̑��� +:[I] -:[K]\n");
	Print("�e�N�X�`���C���f�b�N�X�̑���: -:[N] +:[M]\n");
	Print("�e�N�X�`���̗�������: +:[Q] -:[E]\n");
	Print("�e�N�X�`���̗���鑬��: +:[V] -:[B]\n");
	Print("�g�`�̍���:%f\n", fHeightWave);
	Print("�g�`�̊Ԋu:%f\n", fDistanceWave);
	Print("�g�`�̑���:%f\n", fSpeedWave);
	Print("���쒆�̃e�N�X�`��:%s\n", pTex);
	Print("���݂̃e�N�X�`���C���f�b�N�X:%d\n", nTexIdx);
	Print("�e�N�X�`���̗�������:%f\n", fTexMoveRot);
	Print("�e�N�X�`���̗���鑬��:%f\n", fTexMove);

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