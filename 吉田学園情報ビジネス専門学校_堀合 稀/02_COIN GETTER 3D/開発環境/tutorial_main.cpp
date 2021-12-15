//==============================================================================
//
// �`���[�g���A����ʂ̕`��ktutorial.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "tutorial_main.h"
#include "tutorial_logo.h"
#include "input.h"
#include "fade.h"
#include "gamepad.h"

// �}�N����`
//#define TUTORIAL_NUM	(2)	// �`���[�g���A���̐�

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
TutoMain g_TutoMain;

//==============================================================================
// �`���[�g���A����ʂ̏���������
//==============================================================================
HRESULT InitTutoMain(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutorial000.jpg",
		&g_TutoMain.tutoMainInfo[0].pTextureTutoMain);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutorial001.jpg",
		&g_TutoMain.tutoMainInfo[1].pTextureTutoMain);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTOMAIN_NUM,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_TutoMain.pVtxBuffTutoMain,
		NULL)))
	{
		return E_FAIL;
	}

	g_TutoMain.tutoMainInfo[0].bUse = true;
	g_TutoMain.tutoMainInfo[1].bUse = false;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_TutoMain.pVtxBuffTutoMain->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTOMAIN_NUM; nCnt++, pVtx += 4)
	{
		// ���U���g��ʂ̒��S���W
		g_TutoMain.tutoMainInfo[nCnt].pos = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);

		// �|���S���̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x - SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y + SCREEN_CENTERY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x - SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y - SCREEN_CENTERY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x + SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y + SCREEN_CENTERY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x + SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y - SCREEN_CENTERY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_TutoMain.pVtxBuffTutoMain->Unlock();

	return S_OK;
}

//==============================================================================
// �`���[�g���A����ʂ̏I������
//==============================================================================
void UninitTutoMain(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_TutoMain.pVtxBuffTutoMain != NULL)
	{
		g_TutoMain.pVtxBuffTutoMain->Release();
		g_TutoMain.pVtxBuffTutoMain = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < TUTOMAIN_NUM; nCnt++)
	{
		if (g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain != NULL)
		{
			g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain->Release();
			g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain = NULL;
		}
	}
}

//==============================================================================
// �`���[�g���A����ʂ̍X�V����
//==============================================================================
void UpdateTutoMain(void)
{
	// ���[�J���ϐ��錾
	FADE fade;
	TutoLogo *Logo = GetTutoLogo();

	// �t�F�[�h�̎擾
	fade = GetFade();

	// �`���[�g���A����ʂ̑J��
	if (fade != FADE_IN)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			if (g_TutoMain.tutoMainInfo[0].bUse == true)
			{// �`���[�g���A���ꖇ�ڂ̐؂�ւ�
				Logo->tutologoInfo[0].bUse = false;
				Logo->tutologoInfo[1].bUse = true;

				g_TutoMain.tutoMainInfo[0].bUse = false;
				g_TutoMain.tutoMainInfo[1].bUse = true;
			}
			else
			{// �t�F�[�h�A�E�g�����ʑJ�ڏ�����
				Logo->bFadeTuto = true;
				SetFade(FADE_OUT, MODE_GAME);
			}
		}
	}
	//else if (fade == FADE_NONE)
	//{
	//	if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	//	{
	//		
	//	}
	//}
}

//==============================================================================
// �`���[�g���A����ʂ̕`�揈��
//==============================================================================
void DrawTutoMain(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_TutoMain.pVtxBuffTutoMain,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_TutoMain.tutoMainInfo[nCnt].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u��
		}
	}
}