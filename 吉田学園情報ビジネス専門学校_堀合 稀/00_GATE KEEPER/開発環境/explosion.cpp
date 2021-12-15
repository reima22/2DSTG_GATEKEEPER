//==============================================================================
//
// �����̕`��kexplosion.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_EXPLOSION	(256)	// �����̍ő�l
#define TEXTURE_CUT		(8)		// �e�N�X�`���[������
#define COUNTANIM		(4)		// ���j�A�j���[�V�����̃J�E���g
#define EXPLOSION_SIZE	(20)	// �����̃T�C�Y

//==============================================================================
// �����̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	int nSize;			// �傫��
	int nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int nPatternAnim;	// �A�j���[�V�����p�^�[����
	bool bUse;			// �g�p���Ă��邩�ǂ���
} EXPLOSION;

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// �����e�N�X�`���[�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// �����o�b�t�@�ւ̃|�C���^
EXPLOSION g_aExplosion[MAX_EXPLOSION];

//==============================================================================
// �����̏���������
//==============================================================================
HRESULT InitExplosion(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	// g_aExplosion�̏�����
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_aExplosion[nCntExplosion].nSize = EXPLOSION_SIZE;
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
	}
	

	// ���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �e�N�X�`�����_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_CUT, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_CUT, 0.0f);

		pVtx += 4;	// ���_�|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//==============================================================================
// �����̏I������
//==============================================================================
void UninitExplosion(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//==============================================================================
// �����̍X�V����
//==============================================================================
void UpdateExplosion(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	EXPLOSION *pExplosion;
	ENEMY *pEnemy;
	PLAYER *Player;
	
	// �����̎擾
	pExplosion = &g_aExplosion[0];

	// �G�̎擾
	pEnemy = GetEnemy();

	// �v���C���[�̎擾
	Player = GetPlayer();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx = pVtx + 4, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{ // �������g�p����Ă����ꍇ
			pExplosion->nCounterAnim++;	// �J�E���g�J�n

			// �J�E���g��4�̔{���ŕω�
			if ((pExplosion->nCounterAnim % COUNTANIM) == 0)
			{
				// �e�N�X�`���̃p�^�[���̍X�V
				pExplosion->nPatternAnim++;
				pExplosion->col.a += -0.1f;
				pExplosion->nSize += 3;

				// �e�N�X�`���̒��_���W�̍X�V
				pVtx[0].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT), 1.0f);
				pVtx[1].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT) + (1.0f / TEXTURE_CUT), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT) + (1.0f / TEXTURE_CUT), 0.0f);

				// ���_���W�̍X�V
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - pExplosion->nSize, pExplosion->pos.y + pExplosion->nSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - pExplosion->nSize, pExplosion->pos.y - pExplosion->nSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + pExplosion->nSize, pExplosion->pos.y + pExplosion->nSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + pExplosion->nSize, pExplosion->pos.y - pExplosion->nSize, 0.0f);

				// �e���_�J���[�̐ݒ�
				pVtx[0].col = pExplosion->col;
				pVtx[1].col = pExplosion->col;
				pVtx[2].col = pExplosion->col;
				pVtx[3].col = pExplosion->col;

				// �A�j���[�V�����I��
				if (pExplosion->nPatternAnim >= TEXTURE_CUT)
				{
					pExplosion->bUse = false;
					pExplosion->nCounterAnim = 0;
					pExplosion->nPatternAnim = 0;
					pExplosion->nSize = EXPLOSION_SIZE;
					pExplosion->col.a = 1.0f;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//==============================================================================
// �����̕`�揈��
//==============================================================================
void DrawExplosion(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffExplosion,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	// �|���S���̕`��
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCntExplosion * 4,		// �`����J�n���钸�_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u��
		}
	}
}

//==============================================================================
// �����̐ݒ�
//==============================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	EXPLOSION *pExplosion;

	// �����̎擾
	pExplosion = &g_aExplosion[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{ // �g�p����Ă��Ȃ��ꍇ
			// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_EXPLOSION000);

			// �ʒu��ݒ�
			pExplosion->pos = pos;

			// �A�j���[�V�����̗L����
			pExplosion->bUse = true;
			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}