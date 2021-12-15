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
#include "select.h"
#include "light.h"
#include <stdio.h>

//==============================================================================
// グローバル変数
//==============================================================================
Object object;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitObject(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	Select *pSelect = GetSelect();
	ObjectType *pObjectType = &object.type[0];
	ObjectInfo *pObjectInfo = &object.objectInfo[0];

	// ローカル変数宣言
	int nNumVtx[MAX_TYPE_OBJECT];	// 頂点数
	DWORD sizeFVF[MAX_TYPE_OBJECT];	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	LoadObject();

	// Xファイルの読み込み
	for (int nCntObject = 0; nCntObject < object.nNumType; nCntObject++, pObjectType++, pMat++)
	{
		D3DXLoadMeshFromX(
			pObjectType->aFileName, 
			D3DXMESH_SYSTEMMEM, pDevice, 
			NULL, 
			&pObjectType->pBuffMatObject,
			NULL, 
			&pObjectType->nNumMatObject,
			&pObjectType->pMeshObject);

		pMat = (D3DXMATERIAL*)pObjectType->pBuffMatObject->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pObjectType->nNumMatObject; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// ファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(
					pDevice,
					pMat[nCntMat].pTextureFilename,
					&pObjectType->pTextureObject[nCntMat]);
			}
		}

		// 頂点数を取得
		nNumVtx[nCntObject] = pObjectType->pMeshObject->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF[nCntObject] = D3DXGetFVFVertexSize(pObjectType->pMeshObject->GetFVF());

		// 頂点バッファをロック
		pObjectType->pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

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

		if (nCntObject == 11)
		{
			pObjectType->vtxMaxObject.x = 5;
			pObjectType->vtxMaxObject.z = 5;
			pObjectType->vtxMinObject.x = -5;
			pObjectType->vtxMinObject.z = -5;
		}
		else if (nCntObject == 13)
		{
			pObjectType->vtxMaxObject.x = 30;
			pObjectType->vtxMaxObject.z = 30;
			pObjectType->vtxMinObject.x = -30;
			pObjectType->vtxMinObject.z = -30;
		}

		// 頂点バッファをアンロック
		pObjectType->pMeshObject->UnlockVertexBuffer();
	}

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++,pObjectInfo++)
	{
		pObjectInfo->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pObjectInfo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pObjectInfo->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pObjectInfo->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pObjectInfo->bUse = false;
		pObjectInfo->nType = 0;
	}

	StageObject(pSelect->stage);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitObject(void)
{
	for (int nCnt = 0; nCnt < object.nNumType; nCnt++)
	{
		// メッシュの破棄
		if (object.type[nCnt].pMeshObject != NULL)
		{				
			object.type[nCnt].pMeshObject->Release();
			object.type[nCnt].pMeshObject = NULL;
		}

		// マテリアルの破棄
		if (object.type[nCnt].pBuffMatObject != NULL)
		{
			object.type[nCnt].pBuffMatObject->Release();
			object.type[nCnt].pBuffMatObject = NULL;
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
	ObjectInfo *pInfo = &object.objectInfo[0];
	D3DLIGHT9 *pLight = GetLight();
	D3DXMATRIX mtxShadow;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	D3DXPLANE plane;
	D3DMATERIAL9 matShadow;

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// ライトベクトルの設定
	vecLight = D3DXVECTOR4(pLight->Direction.x, pLight->Direction.y, pLight->Direction.z, 0.0f);

	// 平面の生成
	pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

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


			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matShadow);

			// 減算合成の生成
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			for (int nCntMat = 0; nCntMat < (int)object.type[pInfo->nType].nNumMatObject; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&matShadow);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// シャドウマトリックスの生成
				D3DXMatrixShadow(&mtxShadow, &vecLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &pInfo->mtxWorld, &mtxShadow);

				// シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				// モデル(パーツ)の描画
				object.type[pInfo->nType].pMeshObject->DrawSubset(nCntMat);
			}

			// 通常の合成に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matShadow);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pInfo->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)object.type[pInfo->nType].pBuffMatObject->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)object.type[pInfo->nType].nNumMatObject; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// モデル(パーツ)の描画
				object.type[pInfo->nType].pMeshObject->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==============================================================================
// オブジェクトの取得
//==============================================================================
Object *GetObject(void)
{
	return &object;
}

// ステージごとのオブジェクト配置
void StageObject(int nStage)
{
	switch (nStage)
	{
	case 0:
		SetObject(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1,false);
		SetObject(D3DXVECTOR3(-12.0f, 0.0f, 1272.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, false);
		SetObject(D3DXVECTOR3(1256.3f, 0.0f, -1284.4f), D3DXVECTOR3(0.0f, -3.14f, 0.0f), 3, false);
		SetObject(D3DXVECTOR3(-1182.7f, 0.0f, -1159.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, false);
		SetObject(D3DXVECTOR3(1200.5f, 0.0f, 1092.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4, false);
		SetObject(D3DXVECTOR3(-1245.4f, 0.0f, 1235.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3, false);
		SetObject(D3DXVECTOR3(-7.1f, 0.0f, -281.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7, false);
		SetObject(D3DXVECTOR3(-7.1f, 0.0f, 259.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 7, false);
		SetObject(D3DXVECTOR3(280.9f, 0.0f, -5.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), 7, false);
		SetObject(D3DXVECTOR3(-295.0f, 0.0f, -5.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 7, false);
		SetObject(D3DXVECTOR3(76.9f, 0.0f, -269.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-91.1f, 0.0f, -269.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-91.1f, 0.0f, 259.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(77.0f, 0.0f, 259.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(268.9f, 0.0f, 91.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(268.9f, 0.0f, -89.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-295.0f, 0.0f, -89.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-295.1f, 0.0f, 67.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-139.1f, 0.0f, 1003.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(-31.1f, 0.0f, 1003.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(76.8f, 0.0f, 1003.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(232.9f, 0.0f, 1342.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(232.9f, 0.0f, 1218.9f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(-271.1f, 0.0f, 1375.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(-271.1f, 0.0f, 1248.8f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 8, false);
		SetObject(D3DXVECTOR3(-439.1f, 0.0f, 379.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(388.9f, 0.0f, 379.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(388.9f, 0.0f, 726.9f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(388.9f, 0.0f, 1002.4f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(388.9f, 0.0f, 1302.9f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-451.1f, 0.0f, 1303.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-451.1f, 0.0f, 1027.9f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-451.1f, 0.0f, 703.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-679.1f, 0.0f, 1015.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-895.1f, 0.0f, 1015.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(640.9f, 0.0f, 1015.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(856.7f, 0.0f, 1015.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(856.9f, 0.0f, 1327.3f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(631.5f, 0.0f, 1327.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-655.3f, 0.0f, 1327.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-895.1f, 0.0f, 1327.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-787.6f, 0.0f, -1397.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-835.1f, 0.0f, -1026.0f), D3DXVECTOR3(0.0f, 4.71f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-547.1f, 0.0f, -1025.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-355.5f, 0.0f, -1361.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-319.0f, 0.0f, -1025.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-91.1f, 0.0f, -1373.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-91.1f, 0.0f, -1018.7f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 12, false);
		SetObject(D3DXVECTOR3(-1315.1f, 0.0f, -688.8f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0, false);
		SetObject(D3DXVECTOR3(239.6f, 0.0f, -1345.5f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0, false);
		SetObject(D3DXVECTOR3(551.8f, 0.0f, -1333.5f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0, false);
		SetObject(D3DXVECTOR3(-1248.9f, 0.0f, -371.5f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 0, false);
		SetObject(D3DXVECTOR3(-1214.6f, 0.0f, 100.4f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 4, false);
		SetObject(D3DXVECTOR3(1230.2f, 0.0f, 378.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 4, false);
		SetObject(D3DXVECTOR3(406.8f, 0.0f, 169.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(406.8f, 0.0f, -214.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(406.8f, 0.0f, -442.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(214.0f, 0.0f, -442.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-240.3f, 0.0f, -442.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-457.1f, 0.0f, -442.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-457.2f, 0.0f, -226.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(-457.2f, 0.0f, 205.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 6, false);
		SetObject(D3DXVECTOR3(153.4f, 0.0f, -923.8f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(416.6f, 0.0f, -924.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(669.3f, 0.0f, -924.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(681.4f, 0.0f, -685.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(681.4f, 0.0f, -444.8f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(681.4f, 0.0f, -216.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1372.7f, 0.0f, -43.4f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1372.7f, 0.0f, -235.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1372.7f, 0.0f, -413.3f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1372.7f, 0.0f, -605.7f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1372.7f, 0.0f, -817.9f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(809.3f, 0.0f, -1423.3f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(962.8f, 0.0f, -1423.3f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1359.6f, 0.0f, 553.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1359.6f, 0.0f, 706.5f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1359.6f, 0.0f, 894.7f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 11,false);
		break;

	case 1:
		SetObject(D3DXVECTOR3(-1349.0f, 0.0f, -1349.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1133.1f, 0.0f, -1269.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-964.1f, 0.0f, -1396.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-789.1f, 0.0f, -1222.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-668.5f, 0.0f, -1352.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-452.5f, 0.0f, -1328.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-237.8f, 0.0f, -1377.1f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-308.4f, 0.0f, -1204.2f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(91.0f, 0.0f, -1430.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(168.2f, 0.0f, -1220.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(419.6f, 0.0f, -1310.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(961.8f, 0.0f, -1420.4f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(992.4f, 0.0f, -1239.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1227.4f, 0.0f, -1245.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1374.4f, 0.0f, -1444.4f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1412.5f, 0.0f, -1156.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1149.1f, 0.0f, -925.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1300.3f, 0.0f, -789.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1337.6f, 0.0f, -536.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1121.2f, 0.0f, -428.8f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1025.7f, 0.0f, -164.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1332.0f, 0.0f, 547.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1343.6f, 0.0f, 345.1f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1007.6f, 0.0f, 237.6f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(923.7f, 0.0f, 525.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(1390.1f, 0.0f, 1356.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1390.1f, 0.0f, 1147.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1390.1f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(1073.1f, 0.0f, 1360.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(844.8f, 0.0f, 1369.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(377.1f, 0.0f, 1369.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-186.9f, 0.0f, 1369.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-611.4f, 0.0f, 1322.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1337.8f, 0.0f, 1360.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1152.3f, 0.0f, 1212.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-867.1f, 0.0f, 1322.2f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1238.5f, 0.0f, 1014.1f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-1144.3f, 0.0f, 730.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1272.0f, 0.0f, 509.2f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1150.7f, 0.0f, 297.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1253.2f, 0.0f, 38.5f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-1065.1f, 0.0f, -86.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-1285.5f, 0.0f, -267.2f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-1053.3f, 0.0f, -390.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-1246.0f, 0.0f, -598.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1058.0f, 0.0f, -751.6f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1282.6f, 0.0f, -900.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1122.8f, 0.0f, -1009.2f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-75.7f, 0.0f, -1320.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(262.6f, 0.0f, -1401.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(555.3f, 0.0f, -1422.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(783.2f, 0.0f, -1422.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1164.8f, 0.0f, -1422.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1358.5f, 0.0f, -1000.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1467.3f, 0.0f, -832.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1332.4f, 0.0f, -387.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1428.1f, 0.0f, -206.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1243.5f, 0.0f, -159.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1320.2f, 0.0f, 14.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(1416.2f, 0.0f, 205.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(1227.6f, 0.0f, 224.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1423.4f, 0.0f, 656.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1428.2f, 0.0f, 477.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1428.2f, 0.0f, 954.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(1242.3f, 0.0f, 1317.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(672.7f, 0.0f, 1389.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(497.6f, 0.0f, 1389.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(202.7f, 0.0f, 1389.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(43.8f, 0.0f, 1389.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-383.2f, 0.0f, 1389.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-756.7f, 0.0f, 1389.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1057.3f, 0.0f, 1389.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11, false);
		SetObject(D3DXVECTOR3(-1344.9f, 0.0f, 898.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1369.1f, 0.0f, 740.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1369.2f, 0.0f, 393.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1067.1f, 0.0f, 157.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1335.0f, 0.0f, 199.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1169.4f, 0.0f, -125.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1168.7f, 0.0f, -435.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1382.7f, 0.0f, -766.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1208.6f, 0.0f, -1086.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1041.6f, 0.0f, -1087.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1004.2f, 0.0f, -889.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-1369.1f, 0.0f, 1138.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11,false);
		SetObject(D3DXVECTOR3(-759.4f, 0.0f, -972.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, false);
		SetObject(D3DXVECTOR3(1142.4f, 0.0f, 528.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, false);
		SetObject(D3DXVECTOR3(-561.3f, 0.0f, -1165.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(-285.7f, 0.0f, -1045.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(-831.5f, 0.0f, -687.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(-1033.9f, 0.0f, -601.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(-793.9f, 0.0f, -408.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(967.3f, 0.0f, 679.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(967.3f, 0.0f, 377.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(1191.8f, 0.0f, 820.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(1191.8f, 0.0f, 958.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(1191.5f, 0.0f, 1122.9f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(1323.6f, 0.0f, 1053.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(-727.3f, 0.0f, 1167.7f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(-917.6f, 0.0f, 1045.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9,false);
		SetObject(D3DXVECTOR3(-1101.8f, 0.0f, 855.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(-868.0f, 0.0f, 899.1f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(940.1f, 0.0f, -962.3f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(754.1f, 0.0f, -1154.8f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(578.9f, 0.0f, -1195.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(674.2f, 0.0f, -991.9f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		SetObject(D3DXVECTOR3(1067.4f, 0.0f, -594.7f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 9, false);
		break;

	case 2:
		SetObject(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 13, false);
		SetObject(D3DXVECTOR3(-1141.3f, 0.0f, 900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14,false);
		SetObject(D3DXVECTOR3(-835.0f, 0.0f, 228.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 14,false);
		SetObject(D3DXVECTOR3(-490.9f, 0.0f, -1044.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 14, false);
		SetObject(D3DXVECTOR3(748.6f, 0.0f, -305.4f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), 14, false);
		SetObject(D3DXVECTOR3(757.9f, 0.0f, 901.8f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 14, false);
		SetObject(D3DXVECTOR3(397.9f, 0.0f, 541.9f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 15, false);
		SetObject(D3DXVECTOR3(554.1f, 0.0f, -171.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 15,false);
		SetObject(D3DXVECTOR3(885.8f, 0.0f, -503.7f), D3DXVECTOR3(0.0f, 0.00f, 0.0f), 15,false);
		SetObject(D3DXVECTOR3(1017.1f, 0.0f, 872.6f), D3DXVECTOR3(0.0f, 0.00f, 0.0f), 15,false);
		SetObject(D3DXVECTOR3(597.4f, 0.0f, 1049.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15, false);
		SetObject(D3DXVECTOR3(-1098.6f, 0.0f, 687.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15, false);
		SetObject(D3DXVECTOR3(-1307.7f, 0.0f, 761.2f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 15, false);
		SetObject(D3DXVECTOR3(-986.2f, 0.0f, 109.6f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 15, false);
		SetObject(D3DXVECTOR3(-690.0f, 0.0f, -29.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15, false);
		SetObject(D3DXVECTOR3(-498.0f, 0.0f, -1214.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15,false);
		SetObject(D3DXVECTOR3(-342.1f, 0.0f, -1179.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15,false);
		SetObject(D3DXVECTOR3(-641.7f, 0.0f, -1335.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(-377.9f, 0.0f, -1283.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(-174.1f, 0.0f, -1061.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(761.7f, 0.0f, -477.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(560.3f, 0.0f, -471.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(469.1f, 0.0f, -315.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(798.3f, 0.0f, -110.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(1032.2f, 0.0f, -441.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, false);
		SetObject(D3DXVECTOR3(997.0f, 0.0f, 758.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(913.9f, 0.0f, 985.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(576.8f, 0.0f, 973.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(457.0f, 0.0f, 458.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(528.5f, 0.0f, 546.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(-609.1f, 0.0f, 37.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(-684.6f, 0.0f, 301.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(-975.7f, 0.0f, 709.2f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16,false);
		SetObject(D3DXVECTOR3(-1310.9f, 0.0f, 1034.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, false);
		SetObject(D3DXVECTOR3(-143.9f, 0.0f, -62.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, false);
		SetObject(D3DXVECTOR3(159.1f, 0.0f, -110.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, false);
		SetObject(D3DXVECTOR3(92.6f, 0.0f, 141.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, false);
		SetObject(D3DXVECTOR3(-45.9f, 0.0f, -291.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(691.8f, 0.0f, -606.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(-302.1f, 0.0f, -963.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17,false);
		SetObject(D3DXVECTOR3(-762.3f, 0.0f, -963.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17,false);
		SetObject(D3DXVECTOR3(-948.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(-1214.3f, 0.0f, 1071.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(605.8f, 0.0f, 753.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(-112.6f, 0.0f, 83.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(894.4f, 0.0f, 673.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(-1215.7f, 0.0f, 690.3f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		SetObject(D3DXVECTOR3(-879.1f, 0.0f, 737.8f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17, false);
		break;

	default:
		break;
	}
}

//==============================================================================
// オブジェクト設定
//==============================================================================
void SetObject(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType,bool bBreak)
{
	// ローカル変数宣言
	ObjectInfo *pInfo = &object.objectInfo[0];

	// オブジェクト情報の設定
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pInfo++)
	{
		if (pInfo->bUse == false)
		{
			pInfo->pos = pos;		// 位置
			pInfo->rot = rot;

			//SetItem(D3DXVECTOR3(pos.x, 100.0f, pos.z));

			pInfo->nType = nType;	// 種類

			pInfo->vtxMinObject = object.type[nType].vtxMinObject;
			pInfo->vtxMaxObject = object.type[nType].vtxMaxObject;

			// 頂点位置
			pInfo->aPos[0] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMinObject.x , pInfo->pos.y , pInfo->pos.z + pInfo->vtxMinObject.z);
			pInfo->aPos[1] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMinObject.x , pInfo->pos.y , pInfo->pos.z + pInfo->vtxMaxObject.z);
			pInfo->aPos[2] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMaxObject.x , pInfo->pos.y , pInfo->pos.z + pInfo->vtxMaxObject.z);
			pInfo->aPos[3] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMaxObject.x , pInfo->pos.y , pInfo->pos.z + pInfo->vtxMinObject.z);

			// 四辺ベクトル
			pInfo->aVec[0] = pInfo->aPos[1] - pInfo->aPos[0];
			pInfo->aVec[1] = pInfo->aPos[2] - pInfo->aPos[1];
			pInfo->aVec[2] = pInfo->aPos[3] - pInfo->aPos[2];
			pInfo->aVec[3] = pInfo->aPos[0] - pInfo->aPos[3];

			// 影の設定
			//switch (nType)
			//{
			//case 0:
			//	pObject->nIdx = SetShadow(D3DXVECTOR3(pObject->pos.x, 0.0f, pObject->pos.z), 10.0f, 10.0f);	// 影の設定
			//	break;

			//case 1:
			//	pObject->nIdx = SetShadow(D3DXVECTOR3(pObject->pos.x, 0.0f, pObject->pos.z), 80.0f, 80.0f);	// 影の設定
			//	break;
			//
			//default:
			//	break;
			//}

			pInfo->bUse = true;

			break;
		}
	}
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fInRadius ,float fHeight,int nCntPlayer)
{
	// ローカル変数宣言
	bool bLand;
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル
	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
	Player *pPlayer = GetPlayer();
	ObjectInfo *pInfo = &object.objectInfo[0];
	
	pPlayer += nCntPlayer;

	if (pPlayer->bOnBlock == false)
	{
		bLand = false;
	}
	else
	{
		bLand = true;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pInfo++)
	{
		if (pInfo->bUse == true)
		{
			// オブジェクトとの当たり判定
			for (int nCnt = 0; nCnt < COLLISION_PARTS; nCnt++)
			{
				if (nCnt == 0)
				{// 左面
					aVec[nCnt] = pos + D3DXVECTOR3(fInRadius, 0.0f, 0.0f) - pInfo->aPos[nCnt];
				}
				else if (nCnt == 1)
				{// 奥側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, -fInRadius) - pInfo->aPos[nCnt];
				}
				else if (nCnt == 2)
				{// 右面
					aVec[nCnt] = pos + D3DXVECTOR3(-fInRadius, 0.0f, 0.0f) - pInfo->aPos[nCnt];
				}
				else if (nCnt == 3)
				{// 手前
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fInRadius) - pInfo->aPos[nCnt];
				}

				// 数値が「+」の時、当たり判定が有効
				pInfo->fPlayerVec[nCnt] = (pInfo->aVec[nCnt].z * aVec[nCnt].x) - (pInfo->aVec[nCnt].x * aVec[nCnt].z);
			}

			// 当たり判定
			if (pInfo->fPlayerVec[0] > 0.0f && pInfo->fPlayerVec[1] > 0.0f && pInfo->fPlayerVec[2] > 0.0f && pInfo->fPlayerVec[3] > 0.0f)
			{

				if (pPos->y <= (pInfo->pos.y + pInfo->vtxMaxObject.y) && pPos->y > pInfo->pos.y + pInfo->vtxMinObject.y - fHeight)
				{
					if (pPos->y <= (pInfo->pos.y + pInfo->vtxMaxObject.y) &&
						pPosOld->y >= pInfo->pos.y + pInfo->vtxMaxObject.y)
					{// 上側
						if (pPos->y < pInfo->pos.y + pInfo->vtxMaxObject.y)
						{
							pPlayer->nKey = 0;
							pPlayer->nCounterMotion = 0;
							pPlayer->motionType = MOTIONTYPE_LANDING;
						}
						pPos->y = pInfo->pos.y + pInfo->vtxMaxObject.y;
						SetPosShadow(nCntPlayer, D3DXVECTOR3(0.0f, pInfo->pos.y + pInfo->vtxMaxObject.y, 0.0f));

						bLand = true;
						if (GetKeyboardTrigger(KEYINFO_JUMP) == true || BuDo(KEYINFO::KEYINFO_OK,nCntPlayer) == true)
						{
							bLand = false;
						}
					}
					else if (pPos->x > pPosOld->x && pPosOld->x < pInfo->pos.x + pInfo->vtxMinObject.x)
					{// 左側	
						pPos->x = pInfo->pos.x + pInfo->vtxMinObject.x - fInRadius;
						//SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->x < pPosOld->x && pPosOld->x > pInfo->pos.x + pInfo->vtxMaxObject.x)
					{// 右側	
						pPos->x = pInfo->pos.x + pInfo->vtxMaxObject.x + fInRadius;
						//SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->z <= pPosOld->z && pPos->z > pInfo->pos.z)
					{// 奥側
						pPos->z = pInfo->pos.z + pInfo->vtxMaxObject.z + fInRadius;
						//SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}
					else if (pPos->z >= pPosOld->z && pPos->z < pInfo->pos.z)
					{// 手前
						pPos->z = pInfo->pos.z + pInfo->vtxMinObject.z - fInRadius;
						//SetPositionShadow(pPlayer->nIdx, D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z));
					}

					break;
				}
			}
			else if (pInfo->fPlayerVec[0] < 0.0f || pInfo->fPlayerVec[1] < 0.0f || pInfo->fPlayerVec[2] < 0.0f || pInfo->fPlayerVec[3] < 0.0f)
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

void LoadObject(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aEqual[2] = { NULL };				// 「=」読み取り用変数
	int nCntFile = 0;						// Xファイルの数
	int nIdx = 0;							// パーツ数
	bool bComment = false;					// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };		// テキスト読み取り用変数
	char aSelect[TEXT_LENGTH] = { NULL };	// ポリゴン情報読み込み用変数
	char aState[TEXT_LENGTH] = { NULL };	// 詳細情報読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSelect = &aSelect[0];
	char *pState = &aState[0];

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
				{// 文字列の先頭が「#」ならばコメントアウトへ
					bComment = true;
				}
				else
				{// 通常時
					if (strcmp("NUM_OBJECT", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &object.nNumType);
					}
					else if (strcmp("OBJECT_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &object.type[nCntFile].aFileName[0]);
						nCntFile++;
					}
					//else if (strcmp("SELECT_SET", pText) == 0)
					//{// 配置情報
					//	aSelect[0] = {};
					//	while (strcmp("END_SELECT_SET", pSelect) != 0)
					//	{// 配置情報の読み取り
					//		fscanf(pFile, "%s", &aSelect[0]);
					//		if (strcmp("POLYGONSET", pSelect) == 0)
					//		{
					//			aState[0] = {};
					//			while (strcmp("END_POLYGONSET", pState) != 0)
					//			{
					//				fscanf(pFile, "%s", &aState[0]);
					//				if (strcmp("POS", pState) == 0)
					//				{// 位置情報
					//					fscanf(pFile, "%s", &aEqual[0]);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].pos.x);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].pos.y);
					//					if (g_select.selectInfo[nIdx].st > 1)
					//					{
					//						//g_select.selectInfo[nIdx].pos.x += SCREEN_HEIGHT * g_select.selectInfo[nIdx].st;
					//						g_select.selectInfo[nIdx].pos.y += SCREEN_HEIGHT * (g_select.selectInfo[nIdx].st - 1);
					//					}
					//				}
					//				else if (strcmp("COL", pState) == 0)
					//				{// 色情報
					//					fscanf(pFile, "%s", &aEqual[0]);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.r);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.g);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.b);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.a);
					//				}
					//				else if (strcmp("WIDTH", pState) == 0)
					//				{// 幅
					//					fscanf(pFile, "%s", &aEqual[0]);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].fWidth);
					//				}
					//				else if (strcmp("HEIGHT", pState) == 0)
					//				{// 高さ
					//					fscanf(pFile, "%s", &aEqual[0]);
					//					fscanf(pFile, "%f", &g_select.selectInfo[nIdx].fHeight);
					//				}
					//				else if (strcmp("TEXTYPE", pState) == 0)
					//				{
					//					fscanf(pFile, "%s", &aEqual[0]);
					//					fscanf(pFile, "%d", &g_select.selectInfo[nIdx].nType);
					//				}
					//				else if (strcmp("SELECTTYPE", pState) == 0)
					//				{
					//					fscanf(pFile, "%s", &aEqual[0]);
					//					fscanf(pFile, "%d", &g_select.selectInfo[nIdx].st);
					//				}
					//			}
					//			g_select.selectInfo[nIdx].bUse = true;
					//			nIdx++;
					//		}
					//	}
					//}
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