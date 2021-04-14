//==============================================================================
//
// �`���[�g���A�����S�̕`��ktutorial.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "tutorial_logo.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define TUTOLOGO_POS0	(D3DXVECTOR3(1100.0f, 600.0f, 0.0f))
#define TUTOLOGO_POS1	(D3DXVECTOR3(950.0f, 680.0f, 0.0f))
#define TUTOLOGO_X		(140.0f)
#define TUTOLOGO_Y		(40.0f)
#define TUTOLOGO_COL	(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
//#define TUTOLOGO_NUM	(2)

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
TutoLogo g_TutoLogo;

//==============================================================================
// �`���[�g���A����ʂ̏���������
//==============================================================================
HRESULT InitTutoLogo(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutologo00.png",
		&g_TutoLogo.tutologoInfo[0].pTextureTutoLogo);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutologo01.png",
		&g_TutoLogo.tutologoInfo[1].pTextureTutoLogo);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTOLOGO_NUM,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_TutoLogo.pVtxBuffTutoLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// �ϐ��̏�����
	g_TutoLogo.tutologoInfo[0].pos = TUTOLOGO_POS0;
	g_TutoLogo.tutologoInfo[1].pos = TUTOLOGO_POS1;
	g_TutoLogo.tutologoInfo[0].bUse = true;
	g_TutoLogo.tutologoInfo[1].bUse = false;
	g_TutoLogo.bFadeTuto = false;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_TutoLogo.pVtxBuffTutoLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTOLOGO_NUM; nCnt++, pVtx += 4)
	{
		// ���U���g��ʂ̒��S���W
		
		g_TutoLogo.tutologoInfo[nCnt].col = TUTOLOGO_COL;

		// �|���S���̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x - TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y + TUTOLOGO_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x - TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y - TUTOLOGO_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x + TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y + TUTOLOGO_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x + TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y - TUTOLOGO_Y, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e���_�J���[�̐ݒ�
		pVtx[0].col = g_TutoLogo.tutologoInfo[nCnt].col;
		pVtx[1].col = g_TutoLogo.tutologoInfo[nCnt].col;
		pVtx[2].col = g_TutoLogo.tutologoInfo[nCnt].col;
		pVtx[3].col = g_TutoLogo.tutologoInfo[nCnt].col;

		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_TutoLogo.pVtxBuffTutoLogo->Unlock();

	return S_OK;
}

//==============================================================================
// �`���[�g���A����ʂ̏I������
//==============================================================================
void UninitTutoLogo(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_TutoLogo.pVtxBuffTutoLogo != NULL)
	{
		g_TutoLogo.pVtxBuffTutoLogo->Release();
		g_TutoLogo.pVtxBuffTutoLogo = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < TUTOLOGO_NUM; nCnt++)
	{
		if (g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo != NULL)
		{			   
			g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo->Release();
			g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo = NULL;
		}
	}
}

//==============================================================================
// �`���[�g���A����ʂ̍X�V����
//==============================================================================
void UpdateTutoLogo(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;

	g_TutoLogo.nCntAnim++;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_TutoLogo.pVtxBuffTutoLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < TUTOLOGO_NUM; nCntLogo++,pVtx += 4)
	{
		if (g_TutoLogo.tutologoInfo[nCntLogo].bUse == true)
		{// �g�p��Ԃɂ��郍�S����
			if (g_TutoLogo.bFadeTuto == true)
			{// ��ʑJ�ڎ�
				if (g_TutoLogo.nCntAnim % 3 == 0)
				{// �J�E���^�[�������̎�
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 1.0f;
				}
				else
				{
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 0.0f;
				}
			}
			else
			{// �ʏ펞
				if (g_TutoLogo.nCntAnim % 60 == 0)
				{// �J�E���^�[��60�̔{���̎�
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 1.0f;
				}
				else if (g_TutoLogo.nCntAnim % 30 == 0 && g_TutoLogo.nCntAnim % 60 != 0)
				{// �J�E���^�[��30�̔{���A����60�̔{���łȂ���
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 0.0f;
				}
			}

			// �e���_�J���[�̐ݒ�
			pVtx[0].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
			pVtx[1].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
			pVtx[2].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
			pVtx[3].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_TutoLogo.pVtxBuffTutoLogo->Unlock();
}

//==============================================================================
// �`���[�g���A�����S�̕`�揈��
//==============================================================================
void DrawTutoLogo(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_TutoLogo.pVtxBuffTutoLogo,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TUTOLOGO_NUM; nCnt++)
	{
		if (g_TutoLogo.tutologoInfo[nCnt].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u��
		}
	}
}

//==============================================================================
// �`���[�g���A�����S���̎擾
//==============================================================================
TutoLogo *GetTutoLogo(void)
{
	return &g_TutoLogo;
}