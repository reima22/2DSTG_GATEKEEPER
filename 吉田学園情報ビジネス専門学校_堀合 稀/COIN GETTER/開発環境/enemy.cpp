//==============================================================================
//
// �G�̕`��kenemy.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "enemy.h"
#include "player.h"
#include "block.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define ENEMY_LEFT_REMIT	(-20)		// �G�z�u�̃J�E���g�
#define ENEMY_RIGHT_REMIT	(1300)		// �X�R�A�{����̍���

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	// �G�e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// �G�o�b�t�@�ւ̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];									// �G�̏��

//==============================================================================
// �G�̏���������
//==============================================================================
HRESULT InitEnemy(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	// �����Z�o���猻���������O
	srand((unsigned int)time(0));

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[4]);

	// �ϐ��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->fWidth = ENEMY_SIZEX;
		pEnemy->fHeight = ENEMY_SIZEY;
		pEnemy->nType = NULL;
		pEnemy->bUse = false;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->nCounterState = 0;
		pEnemy->nLife = 0;
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->nAnimCnt = 0;
		pEnemy->fTex = 0.0f;
	}

	// ���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �G�̒��S���W�̐ݒ�
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �G�̒��_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);

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

		pVtx += 4;	// ���_�|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//==============================================================================
// �G�̏I������
//==============================================================================
void UninitEnemy(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TYPE_ENEMY; nCnt++)
	{
		if (g_apTextureEnemy[nCnt] != NULL)
		{
			g_apTextureEnemy[nCnt]->Release();
			g_apTextureEnemy[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �G�̍X�V����
//==============================================================================
void UpdateEnemy(void)
{
	AnimEnemy();
	MoveEnemy();
}

//==============================================================================
// �G�̕`�揈��
//==============================================================================
void DrawEnemy(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffEnemy,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCntEnemy * 4,			// �`����J�n���钸�_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u��
		}
	}
}

//==============================================================================
// �G�̐ݒ�
//==============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fWidth, float fHeight, int nType, int nLife)
{
	// ���[�J���ϐ��錾
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			// �ʒu��ݒ�
			pEnemy->pos = pos;

			// �傫����ݒ�
			pEnemy->fWidth = fWidth;
			pEnemy->fHeight = fHeight;

			// ���_���W��ݒ�
			SetVertexEnemy(nCntEnemy);

			// �ړ��ʂ̐ݒ�
			pEnemy->move = move;

			// ��ނ̐ݒ�
			pEnemy->nType = nType;

			// �G�̗̑�
			pEnemy->nLife = nLife;

			pEnemy->bUse = true;
			break;
		}
	}
}

//==============================================================================
// �G�̒��_���W�擾����
//==============================================================================
void SetVertexEnemy(int nIdx)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�|�C���^��4���i�߂�
	pVtx += nIdx * 4;

	pEnemy += nIdx;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y - g_aEnemy[nIdx].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y - g_aEnemy[nIdx].fHeight, 0.0f);
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// �G�̈ړ�
//==============================================================================
void MoveEnemy(void)
{
	// ���[�J���ϐ��錾
	//VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		//�O��̈ʒu��ۑ�
		pEnemy->posOld = pEnemy->pos;
	}

	// �G�̎擾
	pEnemy = &g_aEnemy[0];

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->move.y < GRAVITY_ENEMY && pEnemy->nType != 4 && pEnemy->nType != 5)
			{
				pEnemy->move.y += DROP_SPEEDUP;
			}

			// �ړ��ʂ̑��
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.y += pEnemy->move.y;

			if (CollisionBlock(&pEnemy->pos, &pEnemy->posOld, &pEnemy->move, ENEMY_SIZEX, ENEMY_SIZEY, ONTYPE_ENEMY) == false && pEnemy->nType == 1)
			{
				pEnemy->move.x *= -1;
				pEnemy->pos.y -= DROP_SPEEDUP;
			}
			//CollisionBlock(&pEnemy->pos, &pEnemy->posOld, &pEnemy->move, ENEMY_SIZEX, ENEMY_SIZEY, ONTYPE_ENEMY);

			// ��ʍ��Ń��[�v(�E���ֈړ�)
			if (pEnemy->pos.x < ENEMY_LEFT_REMIT)
			{
				pEnemy->pos.x = ENEMY_RIGHT_REMIT;
				if (pEnemy->pos.y >= 690.0f)
				{
					pEnemy->pos.y = 130.0f;
				}
			}

			// ��ʉE�Ń��[�v(�����ֈړ�)
			if (pEnemy->pos.x > ENEMY_RIGHT_REMIT)
			{
				pEnemy->pos.x = ENEMY_LEFT_REMIT;
				if (pEnemy->pos.y >= 690.0f)
				{
					pEnemy->pos.y = 130.0f;
				}
			}

			SetVertexEnemy(nCnt);
		}
	}
}

//==============================================================================
// �G�̎擾
//==============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==============================================================================
// �G�̃A�j���[�V����
//==============================================================================
void AnimEnemy(void)
{
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->nType == 4)
			{
				pEnemy->nAnimCnt++;
				if (pEnemy->nAnimCnt % 20 == 0)
				{
					pEnemy->fTex += 0.5f;
					if (pEnemy->fTex >= 1.0f)
					{
						pEnemy->fTex = 0.0f;
					}
				}

				// �e�N�X�`�����_���̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + pEnemy->fTex, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + pEnemy->fTex, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.5f + pEnemy->fTex, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + pEnemy->fTex, 0.0f);

				SetVertexEnemy(nCnt);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}