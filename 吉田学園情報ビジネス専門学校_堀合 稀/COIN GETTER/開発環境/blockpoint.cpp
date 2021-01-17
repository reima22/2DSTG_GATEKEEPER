//==============================================================================
//
// �u���b�N�ړ��_�̕`��kblockpoint.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "blockpoint.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBlockPoint = NULL;			// �u���b�N�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlockPoint = NULL;	// �o�b�t�@�ւ̃|�C���^
BLOCKPOINT g_aBlockPoint[POINT_MAX];					// �u���b�N�ړ��_�̏��

//==============================================================================
// �u���b�N�̏���������
//==============================================================================
HRESULT InitBlockPoint(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/point000.png",
		&g_pTextureBlockPoint);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POINT_MAX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlockPoint,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++)
	{
		g_aBlockPoint[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlockPoint[nCnt].fHeight = 0.0f;
		g_aBlockPoint[nCnt].fWidth = 0.0f;
		g_aBlockPoint[nCnt].bUse = false;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlockPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++, pVtx += 4)
	{
		// �u���b�N�̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x, g_aBlockPoint[nCnt].pos.y + g_aBlockPoint[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x, g_aBlockPoint[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x + BLOCK_SIZEX, g_aBlockPoint[nCnt].pos.y + BLOCK_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x + BLOCK_SIZEX, g_aBlockPoint[nCnt].pos.y, 0.0f);

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

		// �e�N�X�`�����_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlockPoint->Unlock();

	return S_OK;
}

//==============================================================================
// �u���b�N�̏I������
//==============================================================================
void UninitBlockPoint(void)
{
	// ���[�J���ϐ��錾
	BLOCKPOINT *pBlockPoint;

	// �u���b�N�̎擾
	pBlockPoint = &g_aBlockPoint[0];

	// �u���b�N�̈ړ�
	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++, pBlockPoint++)
	{
		if (pBlockPoint->bUse == true)
		{
			pBlockPoint->bUse = false;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlockPoint != NULL)
	{
		g_pVtxBuffBlockPoint->Release();
		g_pVtxBuffBlockPoint = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBlockPoint != NULL)
	{				   
		g_pTextureBlockPoint->Release();
		g_pTextureBlockPoint = NULL;
	}
}

//==============================================================================
// �u���b�N�̍X�V����
//==============================================================================
void UpdateBlockPoint(void)
{
	// ���[�J���ϐ��錾
	BLOCKPOINT *pBlockPoint;

	// �ړ��_�̎擾
	pBlockPoint = &g_aBlockPoint[0];

	// �ړ��_�̒��_���W�X�V
	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++)
	{
		if (pBlockPoint->bUse == true)
		{
			// �u���b�N�̒��_���W�擾
			SetVertexPoint(nCnt);
		}
	}
}

//==============================================================================
// �u���b�N�̕`�揈��
//==============================================================================
void DrawBlockPoint(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	BLOCKPOINT *pBlockPoint = &g_aBlockPoint[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBlockPoint,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++,pBlockPoint++)
	{
		if (pBlockPoint->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlockPoint);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u��
		}
	}
}

//==============================================================================
// �ړ��_�̎擾
//==============================================================================
BLOCKPOINT *GetPoint(void)
{
	return &g_aBlockPoint[0];
}

//==============================================================================
// �u���b�N�̐ݒ�
//==============================================================================
void SetPoint(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	BLOCKPOINT *pBlockPoint = &g_aBlockPoint[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlockPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++,pBlockPoint++)
	{
		if (pBlockPoint->bUse == false)
		{
			// �ʒu��ݒ�
			pBlockPoint->pos = pos;

			// ���̐ݒ�
			pBlockPoint->fWidth = fWidth;

			// �����̐ݒ�
			pBlockPoint->fHeight = fHeight;

			// ���_���W��ݒ�
			SetVertexBlock(nCnt);

			// �g�p��Ԃֈڍs
			pBlockPoint->bUse = true;

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlockPoint->Unlock();
}

//==============================================================================
// �u���b�N�̒��_���W�擾����
//==============================================================================
void SetVertexPoint(int nIdx)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	BLOCKPOINT *pBlockPoint;

	// �G�̎擾
	pBlockPoint = &g_aBlockPoint[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlockPoint->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�|�C���^��4���i�߂�
	pVtx += nIdx * 4;

	pBlockPoint += nIdx;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pBlockPoint->pos.x, pBlockPoint->pos.y + pBlockPoint->fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pBlockPoint->pos.x, pBlockPoint->pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pBlockPoint->pos.x + pBlockPoint->fWidth, pBlockPoint->pos.y + pBlockPoint->fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pBlockPoint->pos.x + pBlockPoint->fWidth, pBlockPoint->pos.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlockPoint->Unlock();
}