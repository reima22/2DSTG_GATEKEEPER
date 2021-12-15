//==============================================================================
//
// �u���b�N�̕`��kblock.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "block.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "gamepad.h"
#include "blockpoint.h"

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBlock = NULL;			// �u���b�N�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		// �o�b�t�@�ւ̃|�C���^
BLOCK g_aBlock[BLOCK_MAX];							// �u���b�N�̏��

//==============================================================================
// �u���b�N�̏���������
//==============================================================================
HRESULT InitBlock(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/block000.jpg",
		&g_pTextureBlock);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BLOCK_MAX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++)
	{
		g_aBlock[nCnt].bUse = false;
		g_aBlock[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCnt].type = ONTYPE_NONE;
		g_aBlock[nCnt].nMoveCnt = 0;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pVtx += 4)
	{
		// �u���b�N�̒��S���W
		g_aBlock[nCnt].pos = D3DXVECTOR3(0.0f, -30.0f, 0.0f);

		// �u���b�N�̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x, g_aBlock[nCnt].pos.y + BLOCK_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x, g_aBlock[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + BLOCK_SIZEX, g_aBlock[nCnt].pos.y + BLOCK_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + BLOCK_SIZEX, g_aBlock[nCnt].pos.y, 0.0f);

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
	g_pVtxBuffBlock->Unlock();

	return S_OK;
}

//==============================================================================
// �u���b�N�̏I������
//==============================================================================
void UninitBlock(void)
{
	// ���[�J���ϐ��錾
	BLOCK *pBlock;

	// �u���b�N�̎擾
	pBlock = &g_aBlock[0];

	// �u���b�N�̈ړ�
	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->bUse = false;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}
}

//==============================================================================
// �u���b�N�̍X�V����
//==============================================================================
void UpdateBlock(void)
{
	// ���[�J���ϐ��錾
	BLOCK *pBlock;

	// �u���b�N�̎擾
	pBlock = &g_aBlock[0];

	// �u���b�N�̉��ړ�
	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	{
		pBlock->nMoveCnt++;

		if (pBlock->bUse == true)
		{
			if (pBlock->move.x != 0)
			{ // ���ړ��̊Ԋu
				if (pBlock->nMoveCnt % pBlock->nMaxCnt == 0)
				{
					pBlock->move.x *= -1;
				}
			}

			// �u���b�N�̈ړ�����
			pBlock->pos += pBlock->move;
			
			// �u���b�N�̒��_���W�擾
			SetVertexBlock(nCnt);
		}
	}

	// �u���b�N�̎擾
	pBlock = &g_aBlock[0];

	// �u���b�N�̏c�ړ�
	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			if (pBlock->move.y != 0.0f)
			{ // �c�ړ��̊Ԋu
				if (pBlock->nMoveCnt % pBlock->nMaxCnt == 0)
				{
					pBlock->move.y *= -1;
				}
			}

			// �u���b�N�̒��_���W�擾
			SetVertexBlock(nCnt);
		}
	}
}

//==============================================================================
// �u���b�N�̕`�揈��
//==============================================================================
void DrawBlock(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBlock);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			nCnt * 4,				// �`����J�n���钸�_�C���f�b�N�X
			2);						// �`�悷��v���~�e�B�u��
	}
}

//==============================================================================
// �u���b�N�̐ݒ�
//==============================================================================
void SetBlock(D3DXVECTOR3 pos,D3DXVECTOR3 move, float fWidth, float fHeight,int nMaxCnt)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == false)
		{
			// �ʒu��ݒ�
			g_aBlock[nCnt].pos = pos;

			// �ړ��͂�ݒ�
			g_aBlock[nCnt].move = move;

			// ���̐ݒ�
			g_aBlock[nCnt].fWidth = fWidth;

			// �����̐ݒ�
			g_aBlock[nCnt].fHeight = fHeight;

			// �ړ��J�E���g�̐ݒ�
			g_aBlock[nCnt].nMaxCnt = nMaxCnt;

			// ���_���W��ݒ�
			SetVertexBlock(nCnt);

			// �g�p��Ԃֈڍs
			g_aBlock[nCnt].bUse = true;

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//==============================================================================
// �u���b�N�̒��_���W�擾����
//==============================================================================
void SetVertexBlock(int nIdx)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	BLOCK *pBlock;

	// �G�̎擾
	pBlock = &g_aBlock[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�|�C���^��4���i�߂�
	pVtx += nIdx * 4;

	pBlock += nIdx;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x, g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x, g_aBlock[nIdx].pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//==============================================================================
// �u���b�N�̏��擾
//==============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//==============================================================================
// �u���b�N�̓����蔻��̐ݒ�
//==============================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, ONTYPE type)
{
	// ���[�J���ϐ��錾
	BLOCK *pBlock;
	bool bLand = false;		// ���n�������ǂ���
	pBlock = &g_aBlock[0];	// �u���b�N�̎擾

	// �����蔻��
	for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->type = type;	// �G��Ă�����̂̎��

			if (pPos->y >= pPosOld->y && pPos->y >= pBlock->pos.y && pPosOld->y - 2 <= pBlock->pos.y)
			{ // �����̔���
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + fWidth > pBlock->pos.x &&
					pPos->x - fWidth < pBlock->pos.x + BLOCK_SIZEX)
				{ // �u���b�N�̓����蔻��
					pPos->y = pBlock->pos.y;	// ����̍X�V
					bLand = true;				// ���n����
					//pPosOld = pPos;				// �O��̈ʒu�̍X�V

					if (pBlock->type == ONTYPE_PLAYER)
					{ // �v���C���[�̔���
						if (GetKeyboardPress(KEYINFO_LEFT) == false &&
							GetKeyboardPress(KEYINFO_RIGHT) == false &&
							IsButtonPush(KEYINFO::KEYINFO_LEFT) == false &&
							IsButtonPush(KEYINFO::KEYINFO_RIGHT) == false)
						{ // ���E�ړ�������s��Ȃ�
							pMove->x = pBlock->move.x;
							pMove->y = pBlock->move.y;
						}
					}
					else if (pBlock->type == ONTYPE_ENEMY)
					{
						if (pBlock->move.y != 0)
						{
							pMove->y = pBlock->move.y;
						}
					}

					pMove->y = 0.0f;	// ����ւ̒蒅

					if (pBlock->move.y > 0.0f)
					{ // ���ꉺ�~��
						pMove->y = pBlock->move.y;
					}

					if (pBlock->move.y < 0.0f)
					{
						pPos->y += pBlock->move.y;
					}
				}
			}
			else if (pPos->y <= pPosOld->y && pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY && pPosOld->y - fHeight >= pBlock->pos.y + BLOCK_SIZEY)
			{ // ����̔���
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + (fWidth + pMove->x) > pBlock->pos.x &&
					pPos->x - (fWidth + pMove->x) < pBlock->pos.x + BLOCK_SIZEX)
				{ // �u���b�N�̓����蔻��
					pPos->y = pBlock->pos.y + BLOCK_SIZEY + fHeight;
					pMove->y = 0.1f;
					//pPosOld = pPos;
				}
			}
			else if (pPos->x >= pPosOld->x && pPos->x < pBlock->pos.x)
			{ // �u���b�N�����ʂ̔���
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + fWidth >= pBlock->pos.x &&
					pPos->x - fWidth <= pBlock->pos.x + BLOCK_SIZEX)
				{ // �u���b�N�̓����蔻��
					pPos->x = pBlock->pos.x - fWidth - 1;

					if (pBlock->type == ONTYPE_PLAYER)
					{ // �v���C���[�̔���
						pMove->x = 0.0f;
					}
					else if (pBlock->type == ONTYPE_ENEMY)
					{ // �G�̔���
						pMove->x *= -1.0f;
					}

					//pPosOld = pPos;
				}
			}
			else if (pPos->x <= pPosOld->x && pPos->x > pBlock->pos.x)
			{ // �u���b�N�E���ʂ̔���
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + fWidth >= pBlock->pos.x &&
					pPos->x - fWidth <= pBlock->pos.x + BLOCK_SIZEX)
				{ // �u���b�N�̓����蔻��
 					pPos->x = pBlock->pos.x + BLOCK_SIZEX + fWidth + 1;

					if (pBlock->type == ONTYPE_PLAYER)
					{ // �v���C���[�̔���
						pMove->x = 0.0f;
					}
					else if (pBlock->type == ONTYPE_ENEMY)
					{
						pMove->x *= -1.0f;
					}

					//pPosOld = pPos;
				}
			}

		}
	}

	return bLand;
}