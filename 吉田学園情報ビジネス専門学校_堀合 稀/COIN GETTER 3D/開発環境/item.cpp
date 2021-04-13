//==============================================================================
//
// �A�C�e���̕`��kitem.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "item.h"
#include "game.h"
#include "shadow.h"
#include "score.h"
#include "particle.h"
#include "sound.h"
#include "player.h"
#include "object.h"
#include "time.h"
#include "stdlib.h"
#include <stdio.h>

//==============================================================================
// �}�N����`�@
//==============================================================================
#define ITEM_TURN	(0.05f)		// �A�C�e���̉�]���x

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
Item g_Item;					// �A�C�e���̏��

//==============================================================================
// �A�C�e���̏���������
//==============================================================================
HRESULT InitItem(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ItemInfo *pItemInfo = &g_Item.itemInfo[0];
	ItemType *pItemType = &g_Item.itemType[0];
	int nNumVtx[MAX_ITEM_TYPE];	// ���_��
	DWORD sizeFVF[MAX_ITEM_TYPE];	// ���_�t�H�[�}�b�g�̃T�C�Y

	// �����������猻���������O
	srand((unsigned int)time(0));

	// �ϐ�������
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItemInfo++)
	{
		pItemInfo->bUse = false;
		pItemInfo->bHiScore = false;
		pItemInfo->vtxMaxItem = VTX_MAX;
		pItemInfo->vtxMinItem = VTX_MIN;
	}
	g_Item.nSetItem = 0;
	g_Item.nCntAnim = 0;
	g_Item.nNumItem = 0;
	g_Item.nCntItem = 0;
	g_Item.matHigh.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	// �A�C�e�����̃e�L�X�g�ǂݍ���
	LoadItem();

	for (int nCntItem = 0; nCntItem < g_Item.nNumItem; nCntItem++,pItemType++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			pItemType->aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pItemType->pBuffMat,
			NULL,
			&pItemType->nNumMat,
			&pItemType->pMesh);

		// ���[�J���ϐ��錾
		BYTE *pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����擾
		nNumVtx[nCntItem] = pItemType->pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
		sizeFVF[nCntItem] = D3DXGetFVFVertexSize(pItemType->pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		pItemType->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx[nCntItem]; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

			// �e���W�̍ő�l�̔�r
			if (pItemType->vtxMaxType.x < vtx.x)
			{
				pItemType->vtxMaxType.x = vtx.x;
			}
			if (pItemType->vtxMaxType.y < vtx.y)
			{
				pItemType->vtxMaxType.y = vtx.y;
			}
			if (pItemType->vtxMaxType.z < vtx.z)
			{
				pItemType->vtxMaxType.z = vtx.z;
			}

			// �e���W�̍ŏ��l�̔�r
			if (pItemType->vtxMinType.x > vtx.x)
			{
				pItemType->vtxMinType.x = vtx.x;
			}
			if (pItemType->vtxMinType.y > vtx.y)
			{
				pItemType->vtxMinType.y = vtx.y;
			}
			if (pItemType->vtxMinType.z > vtx.z)
			{
				pItemType->vtxMinType.z = vtx.z;
			}

			pVtxBuff += sizeFVF[nCntItem];	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N
		pItemType->pMesh->UnlockVertexBuffer();
	}

	// �A�C�e���̔z�u
	pItemInfo = &g_Item.itemInfo[0];
	for (int nCntItem = 0; nCntItem < g_Item.nSetItem; nCntItem++, pItemInfo++)
	{
		SetItem(pItemInfo->pos, pItemInfo->rot, pItemInfo->nType);
	}

	// �����_���Ńn�C�X�R�A�A�C�e���̑I��
	g_Item.itemInfo[RandItem()].bHiScore = true;

	return S_OK;
}

//==============================================================================
// �A�C�e���̏I������
//==============================================================================
void UninitItem(void)
{
	// ���[�J���ϐ��錾
	ItemType *pItemType = &g_Item.itemType[0];

	for (int nCntItem = 0; nCntItem < g_Item.nNumItem; nCntItem++, pItemType++)
	{
		// ���b�V���̔j��
		if (pItemType->pMesh != NULL)
		{
			pItemType->pMesh->Release();
			pItemType->pMesh = NULL;
		}

		// �}�e���A���̔j��
		if (pItemType->pBuffMat != NULL)
		{
			pItemType->pBuffMat->Release();
			pItemType->pBuffMat = NULL;
		}
	}
}

//==============================================================================
// �A�C�e���̍X�V����
//==============================================================================
void UpdateItem(void)
{
	// ���[�J���ϐ��錾
	ItemInfo *pInfo = &g_Item.itemInfo[0];

	// �A�C�e���̉�]
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == true)
		{// �g�p��Ԃ�
			pInfo->rot.y += ITEM_TURN;	// �p�x�̉��Z

			if (pInfo->rot.y > D3DX_PI)
			{// �p�x�̐��l�P����
				pInfo->rot.y -= D3DX_PI * 2.0f;
			}

			//CollisionVec(&pItem->pos,NULL,NULL,pItem->vtxMaxObject.x, pItem->vtxMaxObject.y);
		}
	}

	g_Item.nCntAnim++;

	// �n�C�X�R�A�R�C���̐F�ω�
	if (g_Item.nCntAnim % 120 == 0)
	{
		g_Item.matHigh.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (g_Item.nCntAnim % 60 == 0 && g_Item.nCntAnim % 120 != 0)
	{
		g_Item.matHigh.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
}

//==============================================================================
// �A�C�e���̕`�揈��
//==============================================================================
void DrawItem(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^
	ItemInfo *pInfo = &g_Item.itemInfo[0];


	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pInfo->mtxWorld);

			// �����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pInfo->rot.y, pInfo->rot.x, pInfo->rot.z);
			D3DXMatrixMultiply(&pInfo->mtxWorld, &pInfo->mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pInfo->pos.x, pInfo->pos.y, pInfo->pos.z);
			D3DXMatrixMultiply(&pInfo->mtxWorld, &pInfo->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pInfo->mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Item.itemType[pInfo->nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Item.itemType[pInfo->nType].nNumMat; nCntMat++)
			{
				if (pInfo->bHiScore == true)
				{
					pDevice->SetMaterial(&g_Item.matHigh);
				}
				else if(pInfo->bHiScore == false)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				// ���f��(�p�[�c)�̕`��
				g_Item.itemType[pInfo->nType].pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==============================================================================
// �A�C�e���̐ݒ�
//==============================================================================
void SetItem(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int nType)
{
	// ���[�J���ϐ��錾
	ItemInfo *pInfo = &g_Item.itemInfo[0];
	ItemType *pType = &g_Item.itemType[0];

	// �A�C�e���̐ݒ�
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == false)
		{// �A�C�e�������g�p��
			pInfo->pos = pos;	// �ʒu

			pInfo->rot = rot;	// �p�x

			pType = &g_Item.itemType[0];

			pType += pInfo->nType;

			pInfo->vtxMaxItem = pType->vtxMaxType;
			pInfo->vtxMinItem = pType->vtxMinType;

			// �e���_�ʒu
			pInfo->posPoint[0] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMinItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMinItem.x);
			pInfo->posPoint[1] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMinItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMaxItem.x);
			pInfo->posPoint[2] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMaxItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMaxItem.x);
			pInfo->posPoint[3] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMaxItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMinItem.x);

			// 4�ӂ̃x�N�g��
			pInfo->vecPoint[0] = pInfo->posPoint[1] - pInfo->posPoint[0];
			pInfo->vecPoint[1] = pInfo->posPoint[2] - pInfo->posPoint[1];
			pInfo->vecPoint[2] = pInfo->posPoint[3] - pInfo->posPoint[2];
			pInfo->vecPoint[3] = pInfo->posPoint[0] - pInfo->posPoint[3];

			// �e�̐ݒ�
			pInfo->nIdx = SetShadow(D3DXVECTOR3(pInfo->pos.x, 0.0f, pInfo->pos.z), 10.0f, 10.0f);

			g_Item.nCntItem++;

			pInfo->bUse = true;	// �g�p��Ԃֈڍs

			break;
		}
	}
}

//==============================================================================
// �G�̎擾
//==============================================================================
Item *GetItem(void)
{
	return &g_Item;
}

//==============================================================================
// �����_���Ńn�C�X�R�A�A�C�e���̑I��
//==============================================================================
int RandItem(void)
{
	// ���[�J���ϐ��錾
	int nHigh = 0;

	nHigh = rand() % g_Item.nSetItem;

	return nHigh;
}

//==============================================================================
// �A�C�e���̎擾
//==============================================================================
void TouchItem(D3DXVECTOR3 *pPos, float fInRadius, float fHeight)
{
	// ���[�J���ϐ��錾
	ItemInfo *pInfo = &g_Item.itemInfo[0];
	Player *pPlayer = GetPlayer();	// �v���C���[�̎擾
	D3DXVECTOR3 aVec[FOUR_POINT];	// �e���_����v���C���[�ւ̃x�N�g��
	float fItemVec[FOUR_POINT];		// ����p�ϐ�
	D3DXVECTOR3 pos = *pPos;		// �v���C���[�̈ʒu

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == true)
		{// �A�C�e���̊O�ϓ����蔻��
			for (int nCnt = 0; nCnt < FOUR_POINT; nCnt++)
			{
				switch (nCnt)
				{
				case 0:	// ����
					aVec[nCnt] = pos + D3DXVECTOR3(fInRadius, 0.0f, 0.0f) - pInfo->posPoint[nCnt];
					break;

				case 1:	// ����
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, -fInRadius) - pInfo->posPoint[nCnt];
					break;

				case 2:	// �E��
					aVec[nCnt] = pos + D3DXVECTOR3(-fInRadius, 0.0f, 0.0f) - pInfo->posPoint[nCnt];
					break;

				case 3:	// ��O��
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fInRadius) - pInfo->posPoint[nCnt];
					break;
				}

				// ����p���l�Z�o
				fItemVec[nCnt] = (pInfo->vecPoint[nCnt].z * aVec[nCnt].x) - (pInfo->vecPoint[nCnt].x * aVec[nCnt].z);
			}

			// �A�C�e���Ƃ̐ڐG
			if (fItemVec[0] > 0.0f && fItemVec[1] > 0.0f && fItemVec[2] > 0.0f && fItemVec[3] > 0.0f && 
				pPlayer->pos.y <= (pInfo->pos.y + pInfo->vtxMaxItem.y) && (pPlayer->pos.y + pPlayer->fHeight) >= pInfo->pos.y)
			{// �A�C�e���̎擾
				if (pInfo->bHiScore == true && g_Item.matHigh.Diffuse.g == 0.0f)
				{
					// �G�t�F�N�g�̔���
					SetEffect(D3DXVECTOR3(pInfo->pos.x, pInfo->pos.y + 5.0f, pInfo->pos.z), 0.01f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 5.0f, 0.05f, 30);
					PlaySound(SOUND_LABEL_SE_HIGHSCORE);	// ���ʉ��̍Đ�
					AddScore(10000);						// �����X�R�A�̉��Z
				}
				else
				{
					// �G�t�F�N�g�̔���
					SetEffect(D3DXVECTOR3(pInfo->pos.x, pInfo->pos.y + 5.0f, pInfo->pos.z), 0.01f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 5.0f, 0.05f, 30);
					PlaySound(SOUND_LABEL_SE_COIN);	// ���ʉ��̍Đ�
					AddScore(1000);					// �X�R�A�̉��Z
				}
				
				Shadow *pShadow = GetShadow();	// �e�̎擾
				pShadow += pInfo->nIdx;			// �Ή�����e�ԍ�
				pShadow->bUse = false;			// �e�̏���
				pInfo->bUse = false;			// �A�C�e���̏���
				g_Item.nCntItem--;					// �A�C�e���J�E���g�̌��Z
				if (g_Item.nCntItem <= 0)
				{// �A�C�e���S�擾�����Ƃ�
					pPlayer->state = PLAYERSTATE_CLEAR;
				}
				break;
			}
		}
	}
}

//==============================================================================
// �A�C�e���̓ǂݍ���
//==============================================================================
void LoadItem(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	int nCntFile = 0;							// X�t�@�C���̐�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aItemSet[DATA_LENGTH] = { NULL };		// �L�����f�[�^�ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pItemSet = &aItemSet[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/item.txt", "r");
	if (pFile != NULL)
	{ //�t�@�C���W�J�\
		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// �R�����g�A�E�g���Ă��Ȃ�
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g
					bComment = true;
				}
				else
				{// �ʏ펞
					if (strcmp("NUM_ITEM", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s %d", &aEqual[0], &g_Item.nNumItem);
					}
					else if (strcmp("ITEM_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s %s", &aEqual[0], &g_Item.itemType[nCntFile].aFileName[0]);

						nCntFile++;
					}
					else if (strcmp("ITEMSET", pText) == 0)
					{// �L�����̏��
						aItemSet[0] = {};
						while (strcmp("END_ITEMSET", pItemSet) != 0)
						{// �L�������̓ǂݎ��
							fscanf(pFile, "%s", &aItemSet[0]);
							if (strcmp("TYPE", pItemSet) == 0)
							{// �����蔻��
								fscanf(pFile, "%s %d", &aEqual[0],&g_Item.itemInfo[g_Item.nSetItem].nType);

							}
							else if (strcmp("POS", pItemSet) == 0)
							{// �����蔻��
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_Item.itemInfo[g_Item.nSetItem].pos.x,
									&g_Item.itemInfo[g_Item.nSetItem].pos.y,
									&g_Item.itemInfo[g_Item.nSetItem].pos.z);
							}
							else if (strcmp("ROT", pItemSet) == 0)
							{// �p�[�c���
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_Item.itemInfo[g_Item.nSetItem].rot.x,
									&g_Item.itemInfo[g_Item.nSetItem].rot.y,
									&g_Item.itemInfo[g_Item.nSetItem].rot.z);
							}
						}
						g_Item.nSetItem++;
					}
				}
			}
			else if (bComment == true)
			{// �R�����g�A�E�g����
			 // ���[�J���ϐ��錾
				char a = NULL;
				char b = NULL;
				fscanf(pFile, "%c", &a);
				while (a != '\n' && b != '\n')
				{
					fscanf(pFile, "%s", &aText[0]);
					fscanf(pFile, "%c", &b);
				}
				bComment = false;
			}
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}