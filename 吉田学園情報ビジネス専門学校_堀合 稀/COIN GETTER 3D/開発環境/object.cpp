//==============================================================================
//
// 3Dモデルの描画〔object.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "object.h"
#include "input.h"
#include "gamepad.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "item.h"
#include "math.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureObject[MAX_TEX_OBJECT] = {};	// テクスチャへのポインタ
LPD3DXMESH g_pMeshObject[OBJECTTYPE_MAX] = {};				// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatObject[OBJECTTYPE_MAX] = {};			// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatObject[OBJECTTYPE_MAX] = {};					// マテリアルの数
Object object[MAX_OBJECT];
Type type[OBJECTTYPE_MAX];

//D3DXVECTOR3 g_aPos[4];					// 当たり判定頂点情報
//D3DXVECTOR3 vec;
//D3DXVECTOR3 g_aVec[4];					// 当たり判定ベクト
//D3DXVECTOR3 g_posModel;				// 位置
//D3DXVECTOR3 g_rotModel;				// 向き
//D3DXMATRIX g_mtxWorldModel;			// ワールドマトリックス
//D3DXVECTOR3 g_moveModelX;				// X軸の移動量
//D3DXVECTOR3 g_moveModelZ;				// Z軸の移動量
//int g_nIdx;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitObject(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/baloon01.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject[OBJECTTYPE_BALOON],
		NULL,
		&g_nNumMatObject[OBJECTTYPE_BALOON],
		&g_pMeshObject[OBJECTTYPE_BALOON]);

	D3DXLoadMeshFromX(
		"data/MODEL/block.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject[OBJECTTYPE_BLOCK],
		NULL,
		&g_nNumMatObject[OBJECTTYPE_BLOCK],
		&g_pMeshObject[OBJECTTYPE_BLOCK]);

	// ローカル変数宣言
	int nNumVtx[OBJECTTYPE_MAX];	// 頂点数
	DWORD sizeFVF[OBJECTTYPE_MAX];	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	for (int nCnt = 0; nCnt < OBJECTTYPE_MAX; nCnt++,pMat++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMatObject[nCnt]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject[nCnt]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// ファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(
					pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_apTextureObject[nCntMat]);
			}
		}
	}

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		object[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCnt].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCnt].bUse = false;
		object[nCnt].nType = 0;
	}

	for (int nCnt = 0; nCnt < OBJECTTYPE_MAX; nCnt++)
	{
		// 頂点数を取得
		nNumVtx[nCnt] = g_pMeshObject[nCnt]->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF[nCnt] = D3DXGetFVFVertexSize(g_pMeshObject[nCnt]->GetFVF());

		// 頂点バッファをロック
		g_pMeshObject[nCnt]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// 頂点座標の比較
		for (int nCntVtx = 0; nCntVtx < (int)nNumVtx[nCnt]; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

														// 各座標の最大値の比較
			if (type[nCnt].vtxMaxObject.x < vtx.x)
			{
				type[nCnt].vtxMaxObject.x = vtx.x;
			}
			if (type[nCnt].vtxMaxObject.y < vtx.y)
			{
				type[nCnt].vtxMaxObject.y = vtx.y;
			}
			if (type[nCnt].vtxMaxObject.z < vtx.z)
			{
				type[nCnt].vtxMaxObject.z = vtx.z;
			}

			// 各座標の最小値の比較
			if (type[nCnt].vtxMinObject.x > vtx.x)
			{
				type[nCnt].vtxMinObject.x = vtx.x;
			}
			if (type[nCnt].vtxMinObject.y > vtx.y)
			{
				type[nCnt].vtxMinObject.y = vtx.y;
			}
			if (type[nCnt].vtxMinObject.z > vtx.z)
			{
				type[nCnt].vtxMinObject.z = vtx.z;
			}

			pVtxBuff += sizeFVF[nCnt];	// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		g_pMeshObject[nCnt]->UnlockVertexBuffer();
	}

	// 風船
	SetObject(D3DXVECTOR3(300.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-300.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(0.0f, 0.0f, 300.0f), 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(0.0f, 0.0f, -300.0f), 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ブロック
	SetObject(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-200.0f, 0.0f, 200.0f), 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(200.0f, 0.0f, -200.0f), 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-200.0f, 0.0f, -200.0f), 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitObject(void)
{
	for (int nCnt = 0; nCnt < OBJECTTYPE_MAX; nCnt++)
	{
		// メッシュの破棄
		if (g_pMeshObject[nCnt] != NULL)
		{				
			g_pMeshObject[nCnt]->Release();
			g_pMeshObject[nCnt] = NULL;
		}

		// マテリアルの破棄
		if (g_pBuffMatObject[nCnt] != NULL)
		{
			g_pBuffMatObject[nCnt]->Release();
			g_pBuffMatObject[nCnt] = NULL;
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
	Object *pObject = &object[0];


	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++,pObject++)
	{
		if (pObject->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pObject->mtxWorld);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pObject->rot.y, pObject->rot.x, pObject->rot.z);
			D3DXMatrixMultiply(&pObject->mtxWorld, &pObject->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pObject->pos.x, pObject->pos.y, pObject->pos.z);
			D3DXMatrixMultiply(&pObject->mtxWorld, &pObject->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pObject->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObject[pObject->nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject[pObject->nType]; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				if (pObject->nType == 0)
				{
					pDevice->SetTexture(0, NULL);
				}
				else
				{
					pDevice->SetTexture(0, g_apTextureObject[nCntMat]);
				}

				// モデル(パーツ)の描画
				g_pMeshObject[pObject->nType]->DrawSubset(nCntMat);
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
	return &object[0];
}

//==============================================================================
// オブジェクト設定
//==============================================================================
void SetObject(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 rot)
{
	// ローカル変数宣言
	Object *pObject = &object[0];

	// オブジェクト情報の設定
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{
		if (pObject->bUse == false)
		{
			pObject->pos = pos;		// 位置
			pObject->rot = rot;

			SetItem(D3DXVECTOR3(pos.x, 100.0f, pos.z));

			pObject->nType = nType;	// 種類

			pObject->vtxMinObject = type[nType].vtxMinObject;
			pObject->vtxMaxObject = type[nType].vtxMaxObject;

			// 頂点位置
			pObject->aPos[0] = D3DXVECTOR3(pObject->pos.x + type[nType].vtxMinObject.x ,pObject->pos.y ,pObject->pos.z + type[nType].vtxMinObject.z);
			pObject->aPos[1] = D3DXVECTOR3(pObject->pos.x + type[nType].vtxMinObject.x ,pObject->pos.y ,pObject->pos.z + type[nType].vtxMaxObject.z);
			pObject->aPos[2] = D3DXVECTOR3(pObject->pos.x + type[nType].vtxMaxObject.x ,pObject->pos.y ,pObject->pos.z + type[nType].vtxMaxObject.z);
			pObject->aPos[3] = D3DXVECTOR3(pObject->pos.x + type[nType].vtxMaxObject.x ,pObject->pos.y ,pObject->pos.z + type[nType].vtxMinObject.z);

			// 四辺ベクトル
			pObject->aVec[0] = pObject->aPos[1] - pObject->aPos[0];
			pObject->aVec[1] = pObject->aPos[2] - pObject->aPos[1];
			pObject->aVec[2] = pObject->aPos[3] - pObject->aPos[2];
			pObject->aVec[3] = pObject->aPos[0] - pObject->aPos[3];

			// 影の設定
			switch (nType)
			{
			case 0:
				pObject->nIdx = SetShadow(D3DXVECTOR3(pObject->pos.x, 0.0f, pObject->pos.z), 10.0f, 10.0f);	// 影の設定
				break;

			case 1:
				pObject->nIdx = SetShadow(D3DXVECTOR3(pObject->pos.x, 0.0f, pObject->pos.z), 80.0f, 80.0f);	// 影の設定
				break;
			
			default:
				break;
			}

			pObject->bUse = true;

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
	Object *pObject = &object[0];
	

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