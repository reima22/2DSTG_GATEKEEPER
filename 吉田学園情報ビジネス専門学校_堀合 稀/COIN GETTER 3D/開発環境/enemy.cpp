//==============================================================================
//
// �G�̕`��kenemy.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "math.h"

// �}�N����`
#define COLLISION_PARTS	(4)	// �����蔻��̖ʂ̐�
#define MAX_ENEMY		(8)	// 

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPD3DXMESH g_pMeshEnemy = NULL;			// ���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatEnemy = NULL;	// �}�e���A��(�ގ����)�ւ̃|�C���^
DWORD g_nNumMatEnemy = 0;				// �}�e���A���̐�
Enemy g_enemy[MAX_ENEMY];				// �G�̍\����

//==============================================================================
// ����������
//==============================================================================
HRESULT InitEnemy(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data/MODEL/cube.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy,
		NULL,
		&g_nNumMatEnemy,
		&g_pMeshEnemy);

	// ���[�J���ϐ��錾
	int nNumVtx;	// ���_��
	DWORD sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = g_pMeshEnemy->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy->GetFVF());

	// �ϐ��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].vtxMinObject = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_enemy[nCntEnemy].vtxMaxObject = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_enemy[nCntEnemy].bUse = false;
	}

	// ���_���W�̔�r
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ���_�o�b�t�@�����b�N
		g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

			// �e���W�̍ő�l�̔�r
			if (g_enemy[nCntEnemy].vtxMaxObject.x < vtx.x)
			{
				g_enemy[nCntEnemy].vtxMaxObject.x = vtx.x;
			}
			if (g_enemy[nCntEnemy].vtxMaxObject.y < vtx.y)
			{
				g_enemy[nCntEnemy].vtxMaxObject.y = vtx.y;
			}
			if (g_enemy[nCntEnemy].vtxMaxObject.z < vtx.z)
			{
				g_enemy[nCntEnemy].vtxMaxObject.z = vtx.z;
			}

			// �e���W�̍ŏ��l�̔�r
			if (g_enemy[nCntEnemy].vtxMinObject.x > vtx.x)
			{
				g_enemy[nCntEnemy].vtxMinObject.x = vtx.x;
			}
			if (g_enemy[nCntEnemy].vtxMinObject.y > vtx.y)
			{
				g_enemy[nCntEnemy].vtxMinObject.y = vtx.y;
			}
			if (g_enemy[nCntEnemy].vtxMinObject.z > vtx.z)
			{
				g_enemy[nCntEnemy].vtxMinObject.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}
		// ���_�o�b�t�@���A�����b�N
		g_pMeshEnemy->UnlockVertexBuffer();
	}

	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXVECTOR3(2.0f, 0.0f, 0.0f), 0);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void UninitEnemy(void)
{
	// ���b�V���̔j��
	if (g_pMeshEnemy != NULL)
	{
		g_pMeshEnemy->Release();
		g_pMeshEnemy = NULL;
	}

	// �}�e���A���̔j��
	if (g_pBuffMatEnemy != NULL)
	{
		g_pBuffMatEnemy->Release();
		g_pBuffMatEnemy = NULL;
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void UpdateEnemy(void)
{
	// ���[�J���ϐ��錾
	Enemy *pEnemy = &g_enemy[0];
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 vec;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//vec = pPlayer->pos - pEnemy->pos;	// �x�N�g���Z�o
			//pEnemy->rot = vec;
			//pEnemy->move = (pPlayer->pos - pEnemy->pos) * 0.01f;
			// �ړ��͎Z�o
			//pEnemy->move.x = sinf(D3DX_PI * vec.x);
			//pEnemy->move.z = cosf(D3DX_PI * vec.z);

			switch (pEnemy->nMovePattern)
			{
			case 0:
				if (pEnemy->pos == D3DXVECTOR3(100.0f, 0.0f, 100.0f))
				{
					pEnemy->move.x = 0.0f;
					pEnemy->move.z = -2.0f;
				}
				else if (pEnemy->pos == D3DXVECTOR3(100.0f, 0.0f, -100.0f))
				{
					pEnemy->move.x = -2.0f;
					pEnemy->move.z = 0.0f;
				}
				else if (pEnemy->pos == D3DXVECTOR3(-100.0f, 0.0f, -100.0f))
				{
					pEnemy->move.x = 0.0f;
					pEnemy->move.z = 2.0f;
				}
				else if (pEnemy->pos == D3DXVECTOR3(-100.0f, 0.0f, 100.0f))
				{
					pEnemy->move.x = 2.0f;
					pEnemy->move.z = 0.0f;
				}
				break;

			default:
				break;
			}

			// �ړ��͂̉��Z
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.z += pEnemy->move.z;

			// �e�̒Ǐ]
			SetPositionShadow(pEnemy->nIdx, D3DXVECTOR3(pEnemy->pos.x, 0.0f, pEnemy->pos.z));

			// ���_�ʒu�̎Z�o
			pEnemy->posPoint[0] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMinObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMinObject.x);
			pEnemy->posPoint[1] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMinObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMaxObject.x);
			pEnemy->posPoint[2] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMaxObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMaxObject.x);
			pEnemy->posPoint[3] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMaxObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMinObject.x);

			// �l�Ӄx�N�g���̎Z�o
			pEnemy->vecPoint[0] = pEnemy->posPoint[1] - pEnemy->posPoint[0];
			pEnemy->vecPoint[1] = pEnemy->posPoint[2] - pEnemy->posPoint[1];
			pEnemy->vecPoint[2] = pEnemy->posPoint[3] - pEnemy->posPoint[2];
			pEnemy->vecPoint[3] = pEnemy->posPoint[0] - pEnemy->posPoint[3];
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void DrawEnemy(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^
	Enemy *pEnemy = &g_enemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pEnemy->mtxWorld);

			// �����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEnemy->rot.y, pEnemy->rot.x, pEnemy->rot.z);
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pEnemy->mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				// ���f��(�p�[�c)�̕`��
				g_pMeshEnemy->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==============================================================================
// �I�u�W�F�N�g�̎擾
//==============================================================================
Enemy *GetEnemy(void)
{
	return &g_enemy[0];
}
//==============================================================================
// �G�̐ݒ�
//==============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nMove)
{
	Enemy *pEnemy = &g_enemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			pEnemy->pos = pos;
			pEnemy->move = move;
			pEnemy->nMovePattern = nMove;
			pEnemy->nIdx = SetShadow(D3DXVECTOR3(pEnemy->pos.x, 0.0f, pEnemy->pos.z), 15.0f, 15.0f);

			pEnemy->bUse = true;
		}
	}
}

//==============================================================================
// �G�Ƃ̐ڐG
//==============================================================================
bool TouchEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ���[�J���ϐ��錾
	Enemy *pEnemy = &g_enemy[0];
	D3DXVECTOR3 aVec[FOUR_POINT];
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 pos = *pPos;
	bool bOnBlock;
	if (pPlayer->bOnBlock == false)
	{
		bOnBlock = false;
	}
	else
	{
		bOnBlock = true;
	}

	for (int nCntItem = 0; nCntItem < MAX_ENEMY; nCntItem++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// �A�C�e���̊O�ϓ����蔻��
			for (int nCnt = 0; nCnt < FOUR_POINT; nCnt++)
			{
				switch (nCnt)
				{
				case 0:	// ���ӂ̃x�N�g��
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - pEnemy->posPoint[nCnt];
					break;

				case 1:	// �����̃x�N�g��
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - pEnemy->posPoint[nCnt];
					break;

				case 2:	// �E�ӂ̃x�N�g��
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - pEnemy->posPoint[nCnt];
					break;

				case 3:	// ��O���̃x�N�g��
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - pEnemy->posPoint[nCnt];
					break;
				}

				// ����p���l�̎Z�o
				pEnemy->fVec[nCnt] = (pEnemy->vecPoint[nCnt].z * aVec[nCnt].x) - (pEnemy->vecPoint[nCnt].x * aVec[nCnt].z);
			}

			if (pPos->y <= pEnemy->pos.y + pEnemy->vtxMaxObject.y - fHeightMin && pPos->y > pEnemy->pos.y + pEnemy->vtxMinObject.y - fHeightMax)
			{
				if (pEnemy->fVec[0] > 0.0f && pEnemy->fVec[1] > 0.0f && pEnemy->fVec[2] > 0.0f && pEnemy->fVec[3] > 0.0f)
				{
					if (pPos->y <= pPosOld->y && 
						pPos->y <= (pEnemy->pos.y + pEnemy->vtxMaxObject.y) && 
						pPosOld->y >= (pEnemy->pos.y + pEnemy->vtxMaxObject.y) &&
						pEnemy->vtxMaxObject.y >= pPos->y)
					{// �㑤
						pPos->y = pEnemy->pos.y + pEnemy->vtxMaxObject.y;
						bOnBlock = true;
					}
					else if (pPos->x > pPosOld->x && pPosOld->x < pEnemy->pos.x + pEnemy->vtxMinObject.x && pPlayer->state == PLAYERSTATE_NORMAL)
					{// ����	
						pPos->x = pEnemy->pos.x + pEnemy->vtxMinObject.x - fWidthMax;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
						PlaySound(SOUND_LABEL_SE_HIT000);
					}
					else if (pPos->x < pPosOld->x && pPosOld->x > pEnemy->pos.x + pEnemy->vtxMaxObject.x && pPlayer->state == PLAYERSTATE_NORMAL)
					{// �E��	
						pPos->x = pEnemy->pos.x + pEnemy->vtxMaxObject.x - fWidthMin;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
						PlaySound(SOUND_LABEL_SE_HIT000);
					}
					else if (pPos->z <= pPosOld->z && pPos->z > pEnemy->pos.z && pPlayer->state == PLAYERSTATE_NORMAL)
					{// ����
						pPos->z = pEnemy->pos.z + pEnemy->vtxMaxObject.z - fDepthMin;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
						PlaySound(SOUND_LABEL_SE_HIT000);
					}
					else if (pPos->z >= pPosOld->z && pPos->z < pEnemy->pos.z && pPlayer->state == PLAYERSTATE_NORMAL)
					{// ��O
						pPos->z = pEnemy->pos.z + pEnemy->vtxMinObject.z - fDepthMax;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
						PlaySound(SOUND_LABEL_SE_HIT000);
					}
				}
			}

			//if (fEnemyVec[0] > 0.0f && fEnemyVec[1] > 0.0f && fEnemyVec[2] > 0.0f && fEnemyVec[3] > 0.0f)
			//{// �A�C�e���̎擾
			//	//SetEffect(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + 5.0f, pEnemy->pos.z), 0.01f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 5.0f, 0.05f, 30);
			//	PlaySound(SOUND_LABEL_SE_HIT000);
			//	//AddScore(100);
			//	break;
			//}
		}
	}

	return bOnBlock;
}