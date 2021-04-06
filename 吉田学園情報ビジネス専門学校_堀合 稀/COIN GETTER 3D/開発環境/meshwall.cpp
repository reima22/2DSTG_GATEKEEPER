//==============================================================================
//
// ���b�V���ǂ̕`��kmeshwall.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "meshwall.h"
#include <stdio.h>

//==============================================================================
// �}�N����`
//==============================================================================

#define WALL_POINT_MAX	(65535)		// ���_���̍ő�
#define WALL_IDX_MAX	(65535)		// �C���f�b�N�X�o�b�t�@�̍ő�m�ې�
#define WIDTH_WALL	(2)		// ��(��)
#define HEIGHT_WALL	(2)		// �s��(����)

//#define POINT	(14)	// ���_��
//#define POLY	(12)	// �|���S����

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshwall = NULL;	// �o�b�t�@�ւ̃|�C���^
//LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshwall = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
//LPDIRECT3DTEXTURE9 g_pTextureMeshwall = NULL;		// �e�N�X�`���ւ̃|�C���^
Meshwall g_Meshwall; 
//Meshwall g_aMeshwall[MAX_MWALL];
//int g_nAllPointWall;								// �����_��
//int g_nPolygonWall;									// �|���S����
//int g_nIdxPointWall;								// �C���f�b�N�X�o�b�t�@�̕K�v�Ȋm�ې�

//int g_nWidthWall = WIDTH_WALL + 1;			// �����̒��_��
//int g_nHeightWall = HEIGHT_WALL + 1;		// �����̒��_��

//==============================================================================
// ����������
//==============================================================================
HRESULT InitMeshwall(void)
{
	// ���[�J���ϐ��錾
	//VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	//WORD *pIdx;
	int nCount = 0;
	WallInfo *pInfo = &g_Meshwall.wallInfo[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_MWALL; nCnt++, pInfo++)
	{
		pInfo->bUse = false;
	}
	g_Meshwall.nNumWall = 0;
	g_Meshwall.nNumTex = 0;

	LoadWall();

	for (int nCntWall = 0; nCntWall < g_Meshwall.nNumTex; nCntWall++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			g_Meshwall.wallType[nCntWall].aFileName,
			&g_Meshwall.wallType[nCntWall].pTextureMeshwall);
	}

	// �|�C���^���ŏ��ɖ߂�
	pInfo = &g_Meshwall.wallInfo[0];

	for (int nCntWall = 0; nCntWall < g_Meshwall.nNumWall; nCntWall++,pInfo++)
	{
		SetMeshwall(pInfo->pos, pInfo->rot, pInfo->fWidth, pInfo->fHeight, pInfo->nWidth, pInfo->nHeight);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * pInfo->nIdxPoint,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pInfo->pVtxBuffMeshwall,
			NULL);

		// �C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * pInfo->nAllPoint,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pInfo->pIdxBuffMeshwall,
			NULL);
	}




	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

	// ����������
	//for (int nCntWall = 0; nCntWall < MAX_MWALL; nCntWall++)
	//{
	//	for (int nCnt = 0; nCnt < g_aMeshwall[nCntWall].nHeight - 1; nCnt++)
	//	{
	//		for (int nCntA = 0; nCntA < g_aMeshwall[nCntWall].nWidth - 1; nCntA++,pVtx++)
	//		{
	//			// �|���S���̊e���_���W
	//			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//			// �@���x�N�g���̐ݒ�
	//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//			// �e���_�J���[�̐ݒ�
	//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//			// �e�N�X�`�����_���̐ݒ�
	//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		}
	//	}
	//}

	//for (int nCnt = 0; nCnt < MAX_MWALL; nCnt++, pVtx += 9)
	//{
	//	// �|���S���̊e���_���W
	//	pVtx[0].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x - 100.0f, g_aMeshwall[nCnt].pos.y + 200.0f, g_aMeshwall[nCnt].pos.z);
	//	pVtx[1].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x, g_aMeshwall[nCnt].pos.y + 200.0f, g_aMeshwall[nCnt].pos.z);
	//	pVtx[2].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x + 100.0f, g_aMeshwall[nCnt].pos.y + 200.0f, g_aMeshwall[nCnt].pos.z);
	//	pVtx[3].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x -100.0f, g_aMeshwall[nCnt].pos.y + 100.0f, g_aMeshwall[nCnt].pos.z);
	//	pVtx[4].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x, g_aMeshwall[nCnt].pos.y + 100.0f, g_aMeshwall[nCnt].pos.z);
	//	pVtx[5].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x + 100.0f, g_aMeshwall[nCnt].pos.y + 100.0f, g_aMeshwall[nCnt].pos.z);
	//	pVtx[6].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x - 100.0f, g_aMeshwall[nCnt].pos.y, g_aMeshwall[nCnt].pos.z);
	//	pVtx[7].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x, g_aMeshwall[nCnt].pos.y,	g_aMeshwall[nCnt].pos.z);
	//	pVtx[8].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x + 100.0f, g_aMeshwall[nCnt].pos.y, g_aMeshwall[nCnt].pos.z);

	//	// �@���x�N�g���̐ݒ�
	//	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[8].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//	// �e���_�J���[�̐ݒ�
	//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//	// �e�N�X�`�����_���̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(2.0f, 0.0f);
	//	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
	//	pVtx[5].tex = D3DXVECTOR2(2.0f, 1.0f);
	//	pVtx[6].tex = D3DXVECTOR2(0.0f, 2.0f);
	//	pVtx[7].tex = D3DXVECTOR2(1.0f, 2.0f);
	//	pVtx[8].tex = D3DXVECTOR2(2.0f, 2.0f);
	//}

	// ���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffMeshwall->Unlock();

	//// �C���f�b�N�X�o�b�t�@�̐���
	//pDevice->CreateIndexBuffer(
	//	sizeof(WORD) * WALL_POINT_MAX * MAX_MWALL,
	//	D3DUSAGE_WRITEONLY,
	//	D3DFMT_INDEX16,
	//	D3DPOOL_MANAGED,
	//	&g_pIdxBuffMeshwall,
	//	NULL);

	// �ǂ̐ݒ�
	//SetMeshwall(D3DXVECTOR3(0.0f, 150.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 50.0f, 20, 4);
	////SetMeshwall(D3DXVECTOR3(0.0f, 50.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 50.0f, 8, 2);
	//SetMeshwall(D3DXVECTOR3(500.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, 0.0f), 50.0f, 50.0f, 20, 4);
	//SetMeshwall(D3DXVECTOR3(0.0f, 100.0f, -500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 50.0f, 50.0f, 20, 4);
	//SetMeshwall(D3DXVECTOR3(-500.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / -2.0f, 0.0f), 50.0f, 50.0f, 20, 4);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void UninitMeshwall(void)
{
	// ���[�J���ϐ��錾
	WallInfo *pInfo = &g_Meshwall.wallInfo[0];
	WallType *pType = &g_Meshwall.wallType[0];

	for (int nCntInfo = 0; nCntInfo < g_Meshwall.nNumWall; nCntInfo++, pInfo++)
	{
		// ���_�o�b�t�@�̊J��
		if (pInfo->pVtxBuffMeshwall != NULL)
		{
			pInfo->pVtxBuffMeshwall->Release();
			pInfo->pVtxBuffMeshwall = NULL;
		}
		
		// �C���f�b�N�X�o�b�t�@�̊J��
		if (pInfo->pIdxBuffMeshwall != NULL)
		{
			pInfo->pIdxBuffMeshwall->Release();
			pInfo->pIdxBuffMeshwall = NULL;
		}
	}

	for (int nCntType = 0; nCntType < g_Meshwall.nNumTex; nCntType++, pType++)
	{
		// �e�N�X�`���̊J��
		if (pType->pTextureMeshwall != NULL)
		{
			pType->pTextureMeshwall->Release();
			pType->pTextureMeshwall = NULL;
		}
	}






}

//==============================================================================
// �X�V����
//==============================================================================
void UpdateMeshwall(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;
	WORD *pIdx;
	D3DXVECTOR3 p[100];

	WallInfo *pInfo = &g_Meshwall.wallInfo[0];
	//WallType *pType = &g_Meshwall.wallType[0];



	for (int nCntMax = 0; nCntMax < MAX_MWALL; nCntMax++, pInfo++)
	{
		if (pInfo->bUse == true)
		{
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			pInfo->pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

			// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
			pInfo->pIdxBuffMeshwall->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntWall = 0; nCntWall < pInfo->nHeightPoint; nCntWall++)
			{
				for (int nCnt = 0; nCnt < pInfo->nWidthPoint; nCnt++, pVtx++)
				{
					// �|���S���̊e���_���W
					//pVtx[0].pos = D3DXVECTOR3(
					//	g_aMeshwall[nCntMax].pos.x - g_aMeshwall[nCntMax].fWidth + g_aMeshwall[nCntMax].fWidth * nCnt * 2 / WIDTH_WALL,
					//	g_aMeshwall[nCntMax].pos.y + g_aMeshwall[nCntMax].fHeight * 2 - g_aMeshwall[nCntMax].fHeight * nCntWall * 2 / HEIGHT_WALL,
					//	g_aMeshwall[nCntMax].pos.z);
					pVtx[0].pos = D3DXVECTOR3(
						-pInfo->fWidthMax / 2.0f + (float)nCnt * pInfo->fWidth,
						pInfo->fHeightMax / 2.0f - (float)nCntWall * pInfo->fHeight,
						0.0f);

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// �e���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`�����_���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCnt, 1.0f * nCntWall);
				}
			}

			for (int nCntA = 0; nCntA < pInfo->nHeight; nCntA++)
			{
				for (int nCnt = 0; nCnt < (pInfo->nWidthPoint + 1); nCnt++)
				{
					if (nCnt != 0 && nCnt == pInfo->nWidthPoint && nCntA != pInfo->nHeight - 1)
					{// �E�[����܂�Ԃ���
						pIdx[0] = pIdx[-1];
						pIdx[1] = pIdx[-2] + 1;
					}
					else if (nCntA == pInfo->nHeight - 1 && nCnt == pInfo->nWidthPoint)
					{// �I�����ɖ�������
						break;
					}
					else
					{// �ʏ�z�u
						pIdx[0] = pInfo->nWidthPoint + (pInfo->nWidthPoint * nCntA) + nCnt;
						pIdx[1] = pIdx[0] - (pInfo->nWidthPoint);
					}
					pIdx += 2;
				}
			}

			// �C���f�b�N�X�o�b�t�@���A�����b�N����
			pInfo->pIdxBuffMeshwall->Unlock();

			// ���_�o�b�t�@���A�����b�N����
			pInfo->pVtxBuffMeshwall->Unlock();
		}
	}

	//// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);
	//// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	//g_pIdxBuffMeshwall->Lock(0, 0, (void**)&pIdx, 0);
	////int a[120];
	//D3DXVECTOR3 b[1000];
	//int nMax = 0;
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	nMax += g_aMeshwall[nCnt].nAllPoint;
	//}
	//for (int nCnt = 0; nCnt < nMax; nCnt++/*,pIdx++*/,pVtx++)
	//{
	//	b[nCnt] = pVtx[0].pos;
	//	//a[nCnt] = pIdx[0];
	//}
	//// ���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffMeshwall->Unlock();
	//// �C���f�b�N�X�o�b�t�@���A�����b�N����
	//g_pIdxBuffMeshwall->Unlock();
}

//==============================================================================
// �`�揈��
//==============================================================================
void DrawMeshwall(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	WallInfo *pInfo = &g_Meshwall.wallInfo[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_MWALL; nCnt++,pInfo++)
	{
		if (pInfo->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pInfo->mtxWorld);

			// �����̔��f
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				pInfo->rot.y,
				pInfo->rot.x,
				pInfo->rot.z);

			D3DXMatrixMultiply(
				&pInfo->mtxWorld,
				&pInfo->mtxWorld,
				&mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(
				&mtxTrans,
				pInfo->pos.x,
				pInfo->pos.y,
				pInfo->pos.z);

			D3DXMatrixMultiply(
				&pInfo->mtxWorld,
				&pInfo->mtxWorld,
				&mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pInfo->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, pInfo->pVtxBuffMeshwall, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(pInfo->pIdxBuffMeshwall);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Meshwall.wallType[pInfo->nType].pTextureMeshwall);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				0,
				0,
				pInfo->nIdxPoint/*g_nIdxPointWall*/,				// ���_��
				0,
				pInfo->nPolygon/*g_nPolygonWall*/);			// �v���~�e�B�u��
		}
	}
}

//==============================================================================
//	���b�V���ǂ̐ݒ�
//==============================================================================
void SetMeshwall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight,int nWidth,int nHeight)
{
	// ���[�J���ϐ��錾
	WallInfo *pInfo;
	pInfo = &g_Meshwall.wallInfo[0];
	//VERTEX_3D *pVtx;

	// �ǂ̐ݒ�
	for (int nCntWall = 0; nCntWall < MAX_MWALL; nCntWall++, pInfo++)
	{
		if (pInfo->bUse == false)
		{
			pInfo->pos = pos;				// �ʒu

			pInfo->rot = rot;				// ����

			pInfo->fWidth = fWidth;			// ��

			pInfo->fHeight = fHeight;		// ����

			pInfo->nWidth = nWidth;			// ��̐�

			pInfo->nHeight = nHeight;		// �s�̐�

			pInfo->nWidthPoint = nWidth + 1;		// ���̒��_��

			pInfo->nHeightPoint = nHeight + 1;		// �����̒��_��

			pInfo->fWidthMax = pInfo->fWidth * (float)nWidth;

			pInfo->fHeightMax = pInfo->fHeight * (float)nHeight;

			pInfo->nAllPoint = nHeight * 2 * (nWidth + 2) - 2;
			pInfo->nPolygon = nWidth * nHeight * 2 + (4 * (nHeight - 1));
			pInfo->nIdxPoint = (nWidth + 1) * (nHeight + 1);

			pInfo->bUse = true;			// �g�p���

			break;
		}
	}
}

void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 move, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{

}

// �ǂ̃e�L�X�g�ǂݍ���
void LoadWall(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	int nCntFile = 0;							// X�t�@�C���̐�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aWallSet[DATA_LENGTH] = { NULL };	// �L�����f�[�^�ǂݎ��p�ϐ�

												// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pWallSet = &aWallSet[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/wall.txt", "r");
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
					if (strcmp("NUM_WALL", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s %d", &aEqual[0], &g_Meshwall.nNumTex);
					}
					else if (strcmp("WALL_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s %s", &aEqual[0], &g_Meshwall.wallType[nCntFile].aFileName[0]);
						nCntFile++;
					}
					else if (strcmp("WALLSET", pText) == 0)
					{// �L�����̏��
						aWallSet[0] = {};
						while (strcmp("END_WALLSET", pWallSet) != 0)
						{// �L�������̓ǂݎ��
							fscanf(pFile, "%s", &aWallSet[0]);
							if (strcmp("TYPE", pWallSet) == 0)
							{// �����蔻��
								fscanf(pFile, "%s %d", &aEqual[0], &g_Meshwall.wallInfo[g_Meshwall.nNumWall].nType);
							}
							else if (strcmp("SIZE", pWallSet) == 0)
							{// �ǂ̑傫��(�c���̃}�X��)
								fscanf(pFile, "%s %d %d", &aEqual[0], 
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].nWidth,
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].nHeight);
							}
							else if (strcmp("WIDTH", pWallSet) == 0)
							{// ��
								fscanf(pFile, "%s %f", &aEqual[0],
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].fWidth);
							}
							else if (strcmp("HEIGHT", pWallSet) == 0)
							{// ����
								fscanf(pFile, "%s %f", &aEqual[0],
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].fHeight);
							}
							else if (strcmp("POS", pWallSet) == 0)
							{// �ʒu
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].pos.x,
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].pos.y,
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].pos.z);

							}
							else if (strcmp("ROT", pWallSet) == 0)
							{// �p�x
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].rot.x,
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].rot.y,
									&g_Meshwall.wallInfo[g_Meshwall.nNumWall].rot.z);
							}
						}D3DX_PI;
						g_Meshwall.nNumWall++;
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