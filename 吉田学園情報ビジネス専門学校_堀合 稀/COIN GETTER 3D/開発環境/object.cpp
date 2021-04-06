//==============================================================================
//
// 3Dモデルの描画〔object.cpp〕
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
// グローバル変数
//==============================================================================
Object g_object;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitObject(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	ObjectType *pObjectType = &g_object.objectType[0];
	int nNumVtx[MAX_OBJECT_TYPE];	// 頂点数
	DWORD sizeFVF[MAX_OBJECT_TYPE];	// 頂点フォーマットのサイズ

	g_object.nSetObject = 0;

	LoadObject();

	// Xファイルの読み込み
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

		// ローカル変数宣言
		BYTE *pVtxBuff;			// 頂点バッファへのポインタ
		D3DXMATERIAL *pMat;		// マテリアルへのポインタ

		// 変数初期化
		pObjectType->vtxMinObject = VTX_MIN;
		pObjectType->vtxMaxObject = VTX_MAX;

		// マテリアル情報に対するポインタを取得
		for (int nCnt = 0; nCnt < MAX_OBJECT_TYPE; nCnt++, pMat++)
		{
			pMat = (D3DXMATERIAL*)pObjectType->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pObjectType->nNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// ファイル名を使用してテクスチャを読み込む
					D3DXCreateTextureFromFile(
						pDevice,
						pMat[nCntMat].pTextureFilename,
						&pObjectType->pTexture[nCntMat]);
				}
			}
		}

		// 頂点数を取得
		nNumVtx[nCntObject] = pObjectType->pMesh->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF[nCntObject] = D3DXGetFVFVertexSize(pObjectType->pMesh->GetFVF());

		// 頂点バッファをロック
		pObjectType->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// 頂点座標の比較
		for (int nCntVtx = 0; nCntVtx < (int)nNumVtx[nCntObject]; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
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

			// 各座標の最小値の比較
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

			pVtxBuff += sizeFVF[nCntObject];	// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		pObjectType->pMesh->UnlockVertexBuffer();
	}

	for (int nCnt = 0; nCnt < g_object.nSetObject; nCnt++)
	{
		SetObject(g_object.objectInfo[nCnt].pos, g_object.objectInfo[nCnt].nType, g_object.objectInfo[nCnt].rot);
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitObject(void)
{
	// ローカル変数宣言
	ObjectType *pObjectType = &g_object.objectType[0];

	for (int nCntObject = 0; nCntObject < g_object.nNumObject; nCntObject++,pObjectType++)
	{
		// メッシュの破棄
		if (pObjectType->pMesh != NULL)
		{				
			pObjectType->pMesh->Release();
			pObjectType->pMesh = NULL;
		}

		// マテリアルの破棄
		if (pObjectType->pBuffMat != NULL)
		{	
			pObjectType->pBuffMat->Release();
			pObjectType->pBuffMat = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateObject(void)
{
	// ローカル変数宣言
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

	/* ベクトルvecAに対して点が左にあるとき「a」はマイナスを示す*/
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawObject(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
	ObjectInfo *pInfo = &g_object.objectInfo[0];

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pInfo++)
	{
		if (pInfo->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pInfo->mtxWorld);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pInfo->rot.y, pInfo->rot.x, pInfo->rot.z);
			D3DXMatrixMultiply(&pInfo->mtxWorld, &pInfo->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pInfo->pos.x, pInfo->pos.y, pInfo->pos.z);
			D3DXMatrixMultiply(&pInfo->mtxWorld, &pInfo->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pInfo->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_object.objectType[pInfo->nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_object.objectType[pInfo->nType].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				if (pInfo->nType == 0)
				{
					pDevice->SetTexture(0, NULL);
				}
				else
				{
					pDevice->SetTexture(0, g_object.objectType[pInfo->nType].pTexture[nCntMat]);
				}

				// モデル(パーツ)の描画
				g_object.objectType[pInfo->nType].pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&object.mtxWorld);

	//// 向きの反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, object.rot.y, object.rot.x, object.rot.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxRot);

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, object.pos.x, object.pos.y, object.pos.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxTrans);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &object.mtxWorld);

	//// 現在のマテリアルを取得
	//pDevice->GetMaterial(&matDef);

	//// マテリアルデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	//{
	//	// マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// テクスチャの設定
	//	pDevice->SetTexture(0, NULL);

	//	// モデル(パーツ)の描画
	//	g_pMeshObject->DrawSubset(nCntMat);
	//}

	//// 保存していたマテリアルを戻す
	//pDevice->SetMaterial(&matDef);
}

//==============================================================================
// オブジェクトの取得
//==============================================================================
Object *GetObject(void)
{
	return &g_object;
}

//==============================================================================
// オブジェクト設定
//==============================================================================
void SetObject(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 rot)
{
	// ローカル変数宣言
	ObjectInfo *pInfo = &g_object.objectInfo[0];

	// オブジェクト情報の設定
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pInfo++)
	{
		if (pInfo->bUse == false)
		{
			pInfo->pos = pos;		// 位置
			pInfo->rot = rot;

			SetItem(D3DXVECTOR3(pos.x, 100.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

			pInfo->nType = nType;	// 種類

			pInfo->vtxMinObject = g_object.objectType[nType].vtxMinObject;
			pInfo->vtxMaxObject = g_object.objectType[nType].vtxMaxObject;

			// 頂点位置
			pInfo->aPos[0] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMinObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMinObject.z);
			pInfo->aPos[1] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMinObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMaxObject.z);
			pInfo->aPos[2] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMaxObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMaxObject.z);
			pInfo->aPos[3] = D3DXVECTOR3(pInfo->pos.x + g_object.objectType[nType].vtxMaxObject.x ,pInfo->pos.y ,pInfo->pos.z + g_object.objectType[nType].vtxMinObject.z);

			// 四辺ベクトル
			pInfo->aVec[0] = pInfo->aPos[1] - pInfo->aPos[0];
			pInfo->aVec[1] = pInfo->aPos[2] - pInfo->aPos[1];
			pInfo->aVec[2] = pInfo->aPos[3] - pInfo->aPos[2];
			pInfo->aVec[3] = pInfo->aPos[0] - pInfo->aPos[3];
			
			// 影の設定
			switch (nType)
			{
			case 0:
				pInfo->nIdx = SetShadow(D3DXVECTOR3(pInfo->pos.x, 0.0f, pInfo->pos.z), 10.0f, 10.0f);	// 影の設定
				break;

			case 1:
				pInfo->nIdx = SetShadow(D3DXVECTOR3(pInfo->pos.x, 0.0f, pInfo->pos.z), 80.0f, 80.0f);	// 影の設定
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
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fInRadius ,float fHeight)
{
	// ローカル変数宣言
	bool bLand;
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル
	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
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
			// オブジェクトとの当たり判定
			for (int nCnt = 0; nCnt < COLLISION_PARTS; nCnt++)
			{
				if (nCnt == 0)
				{// 左面
					aVec[nCnt] = pos + D3DXVECTOR3(fInRadius, 0.0f, 0.0f) - pObject->aPos[nCnt];
				}
				else if (nCnt == 1)
				{// 奥側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, -fInRadius) - pObject->aPos[nCnt];
				}
				else if (nCnt == 2)
				{// 右面
					aVec[nCnt] = pos + D3DXVECTOR3(-fInRadius, 0.0f, 0.0f) - pObject->aPos[nCnt];
				}
				else if (nCnt == 3)
				{// 手前
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fInRadius) - pObject->aPos[nCnt];
				}

				// 数値が「+」の時、当たり判定が有効
				pObject->fPlayerVec[nCnt] = (pObject->aVec[nCnt].z * aVec[nCnt].x) - (pObject->aVec[nCnt].x * aVec[nCnt].z);
			}

			// 当たり判定
			if (pObject->fPlayerVec[0] > 0.0f && pObject->fPlayerVec[1] > 0.0f && pObject->fPlayerVec[2] > 0.0f && pObject->fPlayerVec[3] > 0.0f)
			{
				SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, pObject->pos.y + pObject->vtxMaxObject.y, pPlayer->pos.z));
				if (pPos->y <= (pObject->pos.y + pObject->vtxMaxObject.y) && pPos->y > pObject->pos.y + pObject->vtxMinObject.y - fHeight)
				{
					if (pPos->y <= (pObject->pos.y + pObject->vtxMaxObject.y) &&
						pPosOld->y >= pObject->pos.y + pObject->vtxMaxObject.y)
					{// 上側
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
					{// 左側	
						pPos->x = pObject->pos.x + pObject->vtxMinObject.x - fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->x < pPosOld->x && pPosOld->x > pObject->pos.x + pObject->vtxMaxObject.x)
					{// 右側	
						pPos->x = pObject->pos.x + pObject->vtxMaxObject.x + fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->z <= pPosOld->z && pPos->z > pObject->pos.z)
					{// 奥側
						pPos->z = pObject->pos.z + pObject->vtxMaxObject.z + fInRadius;
						SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->z >= pPosOld->z && pPos->z < pObject->pos.z)
					{// 手前
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
// オブジェクトテキストの読み込み
//==============================================================================
void LoadObject(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	int nCntFile = 0;							// Xファイルの数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aObjectSet[DATA_LENGTH] = { NULL };	// キャラデータ読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pObjectSet = &aObjectSet[0];

	// データの読み込み
	pFile = fopen("data/TEXT/object.txt", "r");
	if (pFile != NULL)
	{ //ファイル展開可能
		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// コメントアウトしていない
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// 文字列の先頭が「#」ならばコメントアウト
					bComment = true;
				}
				else
				{// 通常時
					if (strcmp("NUM_OBJECT", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s %d", &aEqual[0], &g_object.nNumObject);
					}
					else if (strcmp("OBJECT_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s %s", &aEqual[0], &g_object.objectType[nCntFile].aFileName[0]);
						nCntFile++;
					}
					else if (strcmp("OBJECTSET", pText) == 0)
					{// キャラの情報
						aObjectSet[0] = {};
						while (strcmp("END_OBJECTSET", pObjectSet) != 0)
						{// キャラ情報の読み取り
							fscanf(pFile, "%s", &aObjectSet[0]);
							if (strcmp("TYPE", pObjectSet) == 0)
							{// 当たり判定
								fscanf(pFile, "%s %d", &aEqual[0], &g_object.objectInfo[g_object.nSetObject].nType);
							}
							else if (strcmp("POS", pObjectSet) == 0)
							{// 位置
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_object.objectInfo[g_object.nSetObject].pos.x,
									&g_object.objectInfo[g_object.nSetObject].pos.y,
									&g_object.objectInfo[g_object.nSetObject].pos.z);

							}
							else if (strcmp("ROT", pObjectSet) == 0)
							{// 角度
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
			{// コメントアウト処理
			 // ローカル変数宣言
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
	{ // ファイル展開不可
	}
}