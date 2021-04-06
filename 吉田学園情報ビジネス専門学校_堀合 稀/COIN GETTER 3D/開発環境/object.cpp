//==============================================================================
//
// 3D���f���̕`��kobject.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "object.h"
#include "input.h"
#include "gamepad.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "item.h"
#include "math.h"
#include <stdio.h>

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
Object g_object;

//==============================================================================
// ����������
//==============================================================================
HRESULT InitObject(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾
	ObjectType *pObjectType = &g_object.objectType[0];
	int nNumVtx[MAX_OBJECT_TYPE];	// ���_��
	DWORD sizeFVF[MAX_OBJECT_TYPE];	// ���_�t�H�[�}�b�g�̃T�C�Y

	g_object.nSetObject = 0;

	LoadObject();

	// X�t�@�C���̓ǂݍ���
	for (int nCntObject = 0; nCntObject < g_object.nNumObject; nCntObject++, pObjectType++)
	{
		D3DXLoadMeshFromX(
			pObjectType->aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pObjectType->pBuffMat,
			NULL,
			&pObjectType->nNumMat,
			&pObjectType->pMesh);

		// ���[�J���ϐ��錾
		BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
		D3DXMATERIAL *pMat;		// �}�e���A���ւ̃|�C���^

		// �ϐ�������
		pObjectType->vtxMinObject = VTX_MIN;
		pObjectType->vtxMaxObject = VTX_MAX;

		// �}�e���A�����ɑ΂���|�C���^���擾
		for (int nCnt = 0; nCnt < MAX_OBJECT_TYPE; nCnt++, pMat++)
		{
			pMat = (D3DXMATERIAL*)pObjectType->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pObjectType->nNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					D3DXCreateTextureFromFile(
						pDevice,
						pMat[nCntMat].pTextureFilename,
						&pObjectType->pTexture[nCntMat]);
				}
			}
		}

		// ���_�����擾
		nNumVtx[nCntObject] = pObjectType->pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
		sizeFVF[nCntObject] = D3DXGetFVFVertexSize(pObjectType->pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		pObjectType->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// ���_���W�̔�r
		for (int nCntVtx = 0; nCntVtx < (int)nNumVtx[nCntObject]; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

			// �e���W�̍ő�l�̔�r
			if (pObjectType->vtxMaxObject.x < vtx.x)
			{
				pObjectType->vtxMaxObject.x = vtx.x;
			}
			if (pObjectType->vtxMaxObject.y < vtx.y)
			{
				pObjectType->vtxMaxObject.y = vtx.y;
			}
			if (pObjectType->vtxMaxObject.z < vtx.z)
			{
				pObjectType->vtxMaxObject.z = vtx.z;
			}

			// �e���W�̍ŏ��l�̔�r
			if (pObjectType->vtxMinObject.x > vtx.x)
			{
				pObjectType->vtxMinObject.x = vtx.x;
			}
			if (pObjectType->vtxMinObject.y > vtx.y)
			{
				pObjectType->vtxMinObject.y = vtx.y;
			}
			if (pObjectType->vtxMinObject.z > vtx.z)
			{
				pObjectType->vtxMinObject.z = vtx.z;
			}

			pVtxBuff += sizeFVF[nCntObject];	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N
		pObjectType->pMesh->UnlockVertexBuffer();
	}

	for (int nCnt = 0; nCnt < g_object.nSetObject; nCnt++)
	{
		SetObject(g_object.objectInfo[nCnt].pos, g_object.objectInfo[nCnt].nType, g_object.objectInfo[nCnt].rot);
	}

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void UninitObject(void)
{
	// ���[�J���ϐ��錾
	ObjectType *pObjectType = &g_object.objectType[0];

	for (int nCntObject = 0; nCntObject < g_object.nNumObject; nCntObject++,pObjectType++)
	{
		// ���b�V���̔j��
		if (pObjectType->pMesh != NULL)
		{				
			pObjectType->pMesh->Release();
			pObjectType->pMesh = NULL;
		}

		// �}�e���A���̔j��
		if (pObjectType->pBuffMat != NULL)
		{	
			pObjectType->pBuffMat->Release();
			pObjectType->pBuffMat = NULL;
		}
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void UpdateObject(void)
{
	// ���[�J���ϐ��錾
	//Player *player = GetPlayer();
	D3DXVECTOR3 pos[2];
	D3DXVECTOR3 vecA;
	D3DXVECTOR3 vecB;
	float a;
	//aVec[];
	pos[0] = D3DXVECTOR3(-100.0f,0.0f,-50.0f);
	pos[1] = D3DXVECTOR3(100.0f,0.0f,-50.0f);
	vecA = pos[1] - pos[0];
	vecB = D3DXVECTOR3(0.0f,0.0f,50.0f) - pos[0];
	a = (vecA.z * vecB.x) - (vecA.x * vecB.z);

	/* �x�N�g��vecA�ɑ΂��ē_�����ɂ���Ƃ��ua�v�̓}�C�i�X������*/
}

//==============================================================================
// �`�揈��
//==============================================================================
void DrawObject(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^
	ObjectInfo *pInfo = &g_object.objectInfo[0];

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pInfo++)
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
			pMat = (D3DXMATERIAL*)g_object.objectType[pInfo->nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_object.objectType[pInfo->nType].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				if (pInfo->nType == 0)
				{
					pDevice->SetTexture(0, NULL);
				}
				else
				{
					pDevice->SetTexture(0, g_object.objectType[pInfo->nType].pTexture[nCntMat]);
				}

				// ���f��(�p�[�c)�̕`��
				g_object.objectType[pInfo->nType].pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
	//// ���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&object.mtxWorld);

	//// �����̔��f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, object.rot.y, object.rot.x, object.rot.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxRot);

	//// �ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, object.pos.x, object.pos.y, object.pos.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxTrans);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &object.mtxWorld);

	//// ���݂̃}�e���A�����擾
	//pDevice->GetMaterial(&matDef);

	//// �}�e���A���f�[�^�ւ̃|�C���^���擾
	//pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	//{
	//	// �}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, NULL);

	//	// ���f��(�p�[�c)�̕`��
	//	g_pMeshObject->DrawSubset(nCntMat);
	//}

	//// �ۑ����Ă����}�e���A����߂�
	//pDevice->SetMaterial(&matDef);
}

//==============================================================================
// �I�u�W�F�N�g�̎擾
//==============================================================================
Object *GetObject(void)
{
	return &g_object;
}

//==============================================================================
// �I�u�W�F�N�g�ݒ�
//==============================================================================
void SetObject(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 rot)
{
	// ���[�J���ϐ��錾
	ObjectInfo *pInfo = &g_object.objectInfo[0];

	// �I�u�W�F�N�g���̐ݒ�
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pInfo++)
	{
		if (pInfo->bUse == false)
		{
			pInfo->pos = pos;		// �ʒu
			pInfo->rot = rot;

			SetItem(D3DXVECTOR3(pos.x, 100.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

			pInfo->nType = nType;	// ���

			pInfo->vtxMinObject = g_object.objectType[nType].vtxMinObject;
			pInfo->vtxMaxObject = g_object.objectType[nType].vtxMaxObject;

			// ���_�ʒu
			pInfo->aPos[0] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMinObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMinObject.z);
			pInfo->aPos[1] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMinObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMaxObject.z);
			pInfo->aPos[2] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMaxObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMaxObject.z);
			pInfo->aPos[3] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMaxObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMinObject.z);

			// �l�Ӄx�N�g��
			pInfo->aVec[0] = pInfo->aPos[1] - pInfo->aPos[0];
			pInfo->aVec[1] = pInfo->aPos[2] - pInfo->aPos[1];
			pInfo->aVec[2] = pInfo->aPos[3] - pInfo->aPos[2];
			pInfo->aVec[3] = pInfo->aPos[0] - pInfo->aPos[3];
			
			// �e�̐ݒ�
			switch (nType)
			{
			case 0:
				pInfo->nIdx = SetShadow(D3DXVECTOR3(pInfo->pos.x, 0.0f, pInfo->pos.z), 10.0f, 10.0f);	// �e�̐ݒ�
				break;

			case 1:
				pInfo->nIdx = SetShadow(D3DXVECTOR3(pInfo->pos.x, 0.0f, pInfo->pos.z), 80.0f, 80.0f);	// �e�̐ݒ�
				break;
			
			default:
				break;
			}

			pInfo->bUse = true;

			break;
		}
	}
}

//==============================================================================
// �O�ς𗘗p���������蔻��
//==============================================================================
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fInRadius ,float fHeight)
{
	// ���[�J���ϐ��錾
	bool bLand;
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// ��`���_���画��Ώۂւ̃x�N�g��
	D3DXVECTOR3 pos = *pPos;			// ����Ώۂ̈ʒu
	Player *pPlayer = GetPlayer();
	ObjectInfo *pObject = &g_object.objectInfo[0];
	

	if (pPlayer->bOnBlock == false)
	{
		bLand = false;
	}
	else
	{
		bLand = true;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{
		if (pObject->bUse == true)
		{
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			for (int nCnt = 0; nCnt < COLLISION_PARTS; nCnt++)
			{
				if (nCnt == 0)
				{// ����
					aVec[nCnt] = pos + D3DXVECTOR3(fInRadius, 0.0f, 0.0f) - pObject->aPos[nCnt];
				}
				else if (nCnt == 1)
				{// ����
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, -fInRadius) - pObject->aPos[nCnt];
				}
				else if (nCnt == 2)
				{// �E��
					aVec[nCnt] = pos + D3DXVECTOR3(-fInRadius, 0.0f, 0.0f) - pObject->aPos[nCnt];
				}
				else if (nCnt == 3)
				{// ��O
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fInRadius) - pObject->aPos[nCnt];
				}

				// ���l���u+�v�̎��A�����蔻�肪�L��
				pObject->fPlayerVec[nCnt] = (pObject->aVec[nCnt].z * aVec[nCnt].x) - (pObject->aVec[nCnt].x * aVec[nCnt].z);
			}

			// �����蔻��
			if (pObject->fPlayerVec[0] > 0.0f && pObject->fPlayerVec[1] > 0.0f && pObject->fPlayerVec[2] > 0.0f && pObject->fPlayerVec[3] > 0.0f)
			{
				SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, pObject->pos.y + pObject->vtxMaxObject.y, pPlayer->pos.z));
				if (pPos->y <= (pObject->pos.y + pObject->vtxMaxObject.y) && pPos->y > pObject->pos.y + pObject->vtxMinObject.y - fHeight)
				{
					if (pPos->y <= (pObject->pos.y + pObject->vtxMaxObject.y) &&
						pPosOld->y >= pObject->pos.y + pObject->vtxMaxObject.y)
					{// �㑤
						if (pPos->y < pObject->pos.y + pObject->vtxMaxObject.y)
						{
							pPlayer->nKey = 0;
							pPlayer->nCounterMotion = 0;
							pPlayer->motionType = MOTIONTYPE_LANDING;
						}
						pPos->y = pObject->pos.y + pObject->vtxMaxObject.y;
						bLand = true;
						if (GetKeyboardTrigger(KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
						{
							bLand = false;
						}
					}
					else if (pPos->x > pPosOld->x && pPosOld->x < pObject->pos.x + pObject->vtxMinObject.x)
					{// ����	
						pPos->x = pObject->pos.x + pObject->vtxMinObject.x - fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->x < pPosOld->x && pPosOld->x > pObject->pos.x + pObject->vtxMaxObject.x)
					{// �E��	
						pPos->x = pObject->pos.x + pObject->vtxMaxObject.x + fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->z <= pPosOld->z && pPos->z > pObject->pos.z)
					{// ����
						pPos->z = pObject->pos.z + pObject->vtxMaxObject.z + fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->z >= pPosOld->z && pPos->z < pObject->pos.z)
					{// ��O
						pPos->z = pObject->pos.z + pObject->vtxMinObject.z - fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}

					break;
				}
			}
			else if (pObject->fPlayerVec[0] < 0.0f || pObject->fPlayerVec[1] < 0.0f || pObject->fPlayerVec[2] < 0.0f || pObject->fPlayerVec[3] < 0.0f)
			{
				if (pPlayer->bOnBlock == true && pos.y > 0.0f)
				{
					bLand = false;
				}
			}
			
		}
	}
	return bLand;
}

//==============================================================================
// �I�u�W�F�N�g�e�L�X�g�̓ǂݍ���
//==============================================================================
void LoadObject(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	int nCntFile = 0;							// X�t�@�C���̐�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aObjectSet[DATA_LENGTH] = { NULL };	// �L�����f�[�^�ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pObjectSet = &aObjectSet[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/object.txt", "r");
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
					if (strcmp("NUM_OBJECT", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s %d", &aEqual[0], &g_object.nNumObject);
					}
					else if (strcmp("OBJECT_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s %s", &aEqual[0], &g_object.objectType[nCntFile].aFileName[0]);
						nCntFile++;
					}
					else if (strcmp("OBJECTSET", pText) == 0)
					{// �L�����̏��
						aObjectSet[0] = {};
						while (strcmp("END_OBJECTSET", pObjectSet) != 0)
						{// �L�������̓ǂݎ��
							fscanf(pFile, "%s", &aObjectSet[0]);
							if (strcmp("TYPE", pObjectSet) == 0)
							{// �����蔻��
								fscanf(pFile, "%s %d", &aEqual[0], &g_object.objectInfo[g_object.nSetObject].nType);
							}
							else if (strcmp("POS", pObjectSet) == 0)
							{// �ʒu
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_object.objectInfo[g_object.nSetObject].pos.x,
									&g_object.objectInfo[g_object.nSetObject].pos.y,
									&g_object.objectInfo[g_object.nSetObject].pos.z);

							}
							else if (strcmp("ROT", pObjectSet) == 0)
							{// �p�x
								fscanf(pFile, "%s %f %f %f", &aEqual[0], 
									&g_object.objectInfo[g_object.nSetObject].rot.x,
									&g_object.objectInfo[g_object.nSetObject].rot.y,
									&g_object.objectInfo[g_object.nSetObject].rot.z);
							}
						}
						g_object.nSetObject++;
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