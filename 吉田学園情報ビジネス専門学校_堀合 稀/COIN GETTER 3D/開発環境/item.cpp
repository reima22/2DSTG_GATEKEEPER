//==============================================================================
//
// アイテムの描画〔item.cpp〕
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
// マクロ定義　
//==============================================================================
#define ITEM_TURN	(0.05f)		// アイテムの回転速度

//==============================================================================
// グローバル変数
//==============================================================================
Item g_Item;					// アイテムの情報

//==============================================================================
// アイテムの初期化処理
//==============================================================================
HRESULT InitItem(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ItemInfo *pItemInfo = &g_Item.itemInfo[0];
	ItemType *pItemType = &g_Item.itemType[0];
	int nNumVtx[MAX_ITEM_TYPE];	// 頂点数
	DWORD sizeFVF[MAX_ITEM_TYPE];	// 頂点フォーマットのサイズ

	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// 変数初期化
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

	// アイテム情報のテキスト読み込み
	LoadItem();

	for (int nCntItem = 0; nCntItem < g_Item.nNumItem; nCntItem++,pItemType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			pItemType->aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pItemType->pBuffMat,
			NULL,
			&pItemType->nNumMat,
			&pItemType->pMesh);

		// ローカル変数宣言
		BYTE *pVtxBuff;	// 頂点バッファへのポインタ

		// 頂点数を取得
		nNumVtx[nCntItem] = pItemType->pMesh->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF[nCntItem] = D3DXGetFVFVertexSize(pItemType->pMesh->GetFVF());

		// 頂点バッファをロック
		pItemType->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx[nCntItem]; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
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

			// 各座標の最小値の比較
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

			pVtxBuff += sizeFVF[nCntItem];	// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		pItemType->pMesh->UnlockVertexBuffer();
	}

	// アイテムの配置
	pItemInfo = &g_Item.itemInfo[0];
	for (int nCntItem = 0; nCntItem < g_Item.nSetItem; nCntItem++, pItemInfo++)
	{
		SetItem(pItemInfo->pos, pItemInfo->rot, pItemInfo->nType);
	}

	// ランダムでハイスコアアイテムの選定
	g_Item.itemInfo[RandItem()].bHiScore = true;

	return S_OK;
}

//==============================================================================
// アイテムの終了処理
//==============================================================================
void UninitItem(void)
{
	// ローカル変数宣言
	ItemType *pItemType = &g_Item.itemType[0];

	for (int nCntItem = 0; nCntItem < g_Item.nNumItem; nCntItem++, pItemType++)
	{
		// メッシュの破棄
		if (pItemType->pMesh != NULL)
		{
			pItemType->pMesh->Release();
			pItemType->pMesh = NULL;
		}

		// マテリアルの破棄
		if (pItemType->pBuffMat != NULL)
		{
			pItemType->pBuffMat->Release();
			pItemType->pBuffMat = NULL;
		}
	}
}

//==============================================================================
// アイテムの更新処理
//==============================================================================
void UpdateItem(void)
{
	// ローカル変数宣言
	ItemInfo *pInfo = &g_Item.itemInfo[0];

	// アイテムの回転
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == true)
		{// 使用状態か
			pInfo->rot.y += ITEM_TURN;	// 角度の加算

			if (pInfo->rot.y > D3DX_PI)
			{// 角度の数値１周時
				pInfo->rot.y -= D3DX_PI * 2.0f;
			}

			//CollisionVec(&pItem->pos,NULL,NULL,pItem->vtxMaxObject.x, pItem->vtxMaxObject.y);
		}
	}

	g_Item.nCntAnim++;

	// ハイスコアコインの色変化
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
// アイテムの描画処理
//==============================================================================
void DrawItem(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
	ItemInfo *pInfo = &g_Item.itemInfo[0];


	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
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
			pMat = (D3DXMATERIAL*)g_Item.itemType[pInfo->nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Item.itemType[pInfo->nType].nNumMat; nCntMat++)
			{
				if (pInfo->bHiScore == true)
				{
					pDevice->SetMaterial(&g_Item.matHigh);
				}
				else if(pInfo->bHiScore == false)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				
				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// モデル(パーツ)の描画
				g_Item.itemType[pInfo->nType].pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==============================================================================
// アイテムの設定
//==============================================================================
void SetItem(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int nType)
{
	// ローカル変数宣言
	ItemInfo *pInfo = &g_Item.itemInfo[0];
	ItemType *pType = &g_Item.itemType[0];

	// アイテムの設定
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == false)
		{// アイテムが未使用か
			pInfo->pos = pos;	// 位置

			pInfo->rot = rot;	// 角度

			pType = &g_Item.itemType[0];

			pType += pInfo->nType;

			pInfo->vtxMaxItem = pType->vtxMaxType;
			pInfo->vtxMinItem = pType->vtxMinType;

			// 各頂点位置
			pInfo->posPoint[0] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMinItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMinItem.x);
			pInfo->posPoint[1] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMinItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMaxItem.x);
			pInfo->posPoint[2] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMaxItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMaxItem.x);
			pInfo->posPoint[3] = D3DXVECTOR3(pInfo->pos.x + pInfo->vtxMaxItem.x, 0.0f, pInfo->pos.z + pInfo->vtxMinItem.x);

			// 4辺のベクトル
			pInfo->vecPoint[0] = pInfo->posPoint[1] - pInfo->posPoint[0];
			pInfo->vecPoint[1] = pInfo->posPoint[2] - pInfo->posPoint[1];
			pInfo->vecPoint[2] = pInfo->posPoint[3] - pInfo->posPoint[2];
			pInfo->vecPoint[3] = pInfo->posPoint[0] - pInfo->posPoint[3];

			// 影の設定
			pInfo->nIdx = SetShadow(D3DXVECTOR3(pInfo->pos.x, 0.0f, pInfo->pos.z), 10.0f, 10.0f);

			g_Item.nCntItem++;

			pInfo->bUse = true;	// 使用状態へ移行

			break;
		}
	}
}

//==============================================================================
// 敵の取得
//==============================================================================
Item *GetItem(void)
{
	return &g_Item;
}

//==============================================================================
// ランダムでハイスコアアイテムの選定
//==============================================================================
int RandItem(void)
{
	// ローカル変数宣言
	int nHigh = 0;

	nHigh = rand() % g_Item.nSetItem;

	return nHigh;
}

//==============================================================================
// アイテムの取得
//==============================================================================
void TouchItem(D3DXVECTOR3 *pPos, float fInRadius, float fHeight)
{
	// ローカル変数宣言
	ItemInfo *pInfo = &g_Item.itemInfo[0];
	Player *pPlayer = GetPlayer();	// プレイヤーの取得
	D3DXVECTOR3 aVec[FOUR_POINT];	// 各頂点からプレイヤーへのベクトル
	float fItemVec[FOUR_POINT];		// 判定用変数
	D3DXVECTOR3 pos = *pPos;		// プレイヤーの位置

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pInfo++)
	{
		if (pInfo->bUse == true)
		{// アイテムの外積当たり判定
			for (int nCnt = 0; nCnt < FOUR_POINT; nCnt++)
			{
				switch (nCnt)
				{
				case 0:	// 左側
					aVec[nCnt] = pos + D3DXVECTOR3(fInRadius, 0.0f, 0.0f) - pInfo->posPoint[nCnt];
					break;

				case 1:	// 奥側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, -fInRadius) - pInfo->posPoint[nCnt];
					break;

				case 2:	// 右側
					aVec[nCnt] = pos + D3DXVECTOR3(-fInRadius, 0.0f, 0.0f) - pInfo->posPoint[nCnt];
					break;

				case 3:	// 手前側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fInRadius) - pInfo->posPoint[nCnt];
					break;
				}

				// 判定用数値算出
				fItemVec[nCnt] = (pInfo->vecPoint[nCnt].z * aVec[nCnt].x) - (pInfo->vecPoint[nCnt].x * aVec[nCnt].z);
			}

			// アイテムとの接触
			if (fItemVec[0] > 0.0f && fItemVec[1] > 0.0f && fItemVec[2] > 0.0f && fItemVec[3] > 0.0f && 
				pPlayer->pos.y <= (pInfo->pos.y + pInfo->vtxMaxItem.y) && (pPlayer->pos.y + pPlayer->fHeight) >= pInfo->pos.y)
			{// アイテムの取得
				if (pInfo->bHiScore == true && g_Item.matHigh.Diffuse.g == 0.0f)
				{
					// エフェクトの発生
					SetEffect(D3DXVECTOR3(pInfo->pos.x, pInfo->pos.y + 5.0f, pInfo->pos.z), 0.01f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 5.0f, 0.05f, 30);
					PlaySound(SOUND_LABEL_SE_HIGHSCORE);	// 効果音の再生
					AddScore(10000);						// 高いスコアの加算
				}
				else
				{
					// エフェクトの発生
					SetEffect(D3DXVECTOR3(pInfo->pos.x, pInfo->pos.y + 5.0f, pInfo->pos.z), 0.01f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 5.0f, 0.05f, 30);
					PlaySound(SOUND_LABEL_SE_COIN);	// 効果音の再生
					AddScore(1000);					// スコアの加算
				}
				
				Shadow *pShadow = GetShadow();	// 影の取得
				pShadow += pInfo->nIdx;			// 対応する影番号
				pShadow->bUse = false;			// 影の消滅
				pInfo->bUse = false;			// アイテムの消滅
				g_Item.nCntItem--;					// アイテムカウントの減算
				if (g_Item.nCntItem <= 0)
				{// アイテム全取得したとき
					pPlayer->state = PLAYERSTATE_CLEAR;
				}
				break;
			}
		}
	}
}

//==============================================================================
// アイテムの読み込み
//==============================================================================
void LoadItem(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	int nCntFile = 0;							// Xファイルの数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aItemSet[DATA_LENGTH] = { NULL };		// キャラデータ読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pItemSet = &aItemSet[0];

	// データの読み込み
	pFile = fopen("data/TEXT/item.txt", "r");
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
					if (strcmp("NUM_ITEM", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s %d", &aEqual[0], &g_Item.nNumItem);
					}
					else if (strcmp("ITEM_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s %s", &aEqual[0], &g_Item.itemType[nCntFile].aFileName[0]);

						nCntFile++;
					}
					else if (strcmp("ITEMSET", pText) == 0)
					{// キャラの情報
						aItemSet[0] = {};
						while (strcmp("END_ITEMSET", pItemSet) != 0)
						{// キャラ情報の読み取り
							fscanf(pFile, "%s", &aItemSet[0]);
							if (strcmp("TYPE", pItemSet) == 0)
							{// 当たり判定
								fscanf(pFile, "%s %d", &aEqual[0],&g_Item.itemInfo[g_Item.nSetItem].nType);

							}
							else if (strcmp("POS", pItemSet) == 0)
							{// 当たり判定
								fscanf(pFile, "%s %f %f %f", &aEqual[0],
									&g_Item.itemInfo[g_Item.nSetItem].pos.x,
									&g_Item.itemInfo[g_Item.nSetItem].pos.y,
									&g_Item.itemInfo[g_Item.nSetItem].pos.z);
							}
							else if (strcmp("ROT", pItemSet) == 0)
							{// パーツ情報
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