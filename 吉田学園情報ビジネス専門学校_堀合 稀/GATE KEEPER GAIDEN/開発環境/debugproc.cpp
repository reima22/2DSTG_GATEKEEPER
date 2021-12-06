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
#include "game.h"

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
		pDevice, 18, 0, 0, 0, FALSE,
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

	int nCountFPS = GetFPSCnt();	// FPS�J�E���g�̎擾
	int nNumAll = CScene::GetNumAll();

	CMeshfield *pMeshfield = CGame::GetMeshfield();

	float fHeightWave = pMeshfield->GetHeightWave();
	float fDistanceWave = pMeshfield->GetDistanceWave();
	float fSpeedWave = pMeshfield->GetSpeedWave();

	Print("FPS:%d\n�I�u�W�F�N�g��:%d\n", nCountFPS, nNumAll);
	Print("�g�`�̍���:%f\n", fHeightWave);
	Print("�g�`�̊Ԋu:%f\n", fDistanceWave);
	Print("�g�`�̑���:%f\n", fSpeedWave);

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
	pDebugProc->Init();

	return pDebugProc;
}