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
#include "particle.h"
#include "sound.h"
#include "player.h"
#include "object.h"
#include "time.h"
#include "select.h"
#include "stdlib.h"
#include <stdio.h>

//==============================================================================
// マクロ定義　
//==============================================================================
#define ITEM_TURN		(0.05f)		// アイテムの回転速度
#define ITEM_SET_MAX	(8)			// 配置するアイテムの最大数

#define SET_0_0			(D3DXVECTOR3(-400.0f, 0.0f, 0.0f))
#define SET_0_1			(D3DXVECTOR3(400.0f, 0.0f, 0.0f))
#define SET_0_2			(D3DXVECTOR3(0.0f, 0.0f, -400.0f))
#define SET_0_3			(D3DXVECTOR3(0.0f, 0.0f, 400.0f))
#define SET_0_4			(D3DXVECTOR3(400.0f, 0.0f, 400.0f))
#define SET_0_5			(D3DXVECTOR3(400.0f, 0.0f, -400.0f))
#define SET_0_6			(D3DXVECTOR3(-400.0f, 0.0f, 400.0f))
#define SET_0_7			(D3DXVECTOR3(-400.0f, 0.0f, -400.0f))

//#define SET_1_0			(D3DXVECTOR3(-400.0f,0.0f,0.0f))
//#define SET_1_1			(D3DXVECTOR3(400.0f,0.0f,0.0f))
//#define SET_1_2			(D3DXVECTOR3(0.0f,0.0f,-400.0f))
//#define SET_1_3			(D3DXVECTOR3(0.0f,0.0f,400.0f))
//#define SET_1_4			(D3DXVECTOR3(-400.0f, 0.0f, 400.0f))
//#define SET_1_5			(D3DXVECTOR3(400.0f, 0.0f, -400.0f))
//#define SET_1_6			(D3DXVECTOR3(400.0f, 0.0f, -400.0f))
//#define SET_1_7			(D3DXVECTOR3(-400.0f, 0.0f, 400.0f))
//
//#define SET_2_0			(D3DXVECTOR3(-400.0f, 0.0f, 0.0f))
//#define SET_2_1			(D3DXVECTOR3(400.0f, 0.0f, 0.0f))
//#define SET_2_2			(D3DXVECTOR3(0.0f, 0.0f, -400.0f))
//#define SET_2_3			(D3DXVECTOR3(0.0f, 0.0f, 400.0f))
//#define SET_2_4			(D3DXVECTOR3(-400.0f, 0.0f, 400.0f))
//#define SET_2_5			(D3DXVECTOR3(400.0f, 0.0f, -400.0f))
//#define SET_2_6			(D3DXVECTOR3(400.0f, 0.0f, -400.0f))
//#define SET_2_7			(D3DXVECTOR3(-400.0f, 0.0f, 400.0f))

//==============================================================================
// グローバル変数
//==============================================================================
Item g_Item;	// アイテムの情報
D3DXVECTOR3 g_RandPos[ITEM_SET_MAX];	// ランダムのアイテム配置位置

//==============================================================================
// アイテムの初期化処理
//==============================================================================
HRESULT InitItem(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	ItemTypeInfo *pItemType = &g_Item.typeinfo[0];
	ItemInfo *pItemInfo = &g_Item.iteminfo[0];

	// ローカル変数宣言
	int nNumVtx[ITEMTYPE_MAX];	// 頂点数
	DWORD sizeFVF[ITEMTYPE_MAX];	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ

	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// デバイスの取得
	pDevice = GetDevice();

	LoadItem();

	for (int nCnt = 0; nCnt < ITEM_SET_MAX; nCnt++)
	{
		//g_RandPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item.iteminfo[nCnt].bSet = false;
	}
	g_RandPos[0] = SET_0_0;
	g_RandPos[1] = SET_0_1;
	g_RandPos[2] = SET_0_2;
	g_RandPos[3] = SET_0_3;
	g_RandPos[4] = SET_0_4;
	g_RandPos[5] = SET_0_5;
	g_RandPos[6] = SET_0_6;
	g_RandPos[7] = SET_0_7;

	for (int nCntItem = 0; nCntItem < g_Item.nNumType; nCntItem++,pItemType++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			pItemType->aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pItemType->pBuffMatItem,
			NULL,
			&pItemType->nNumMatItem,
			&pItemType->pMeshItem);

		// 頂点数を取得
		nNumVtx[nCntItem] = pItemType->pMeshItem->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF[nCntItem] = D3DXGetFVFVertexSize(pItemType->pMeshItem->GetFVF());

		// 頂点バッファをロック
		pItemType->pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx[nCntItem]; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
			if (pItemType->vtxMaxItem.x < vtx.x)
			{
				pItemType->vtxMaxItem.x = vtx.x;
			}
			if (pItemType->vtxMaxItem.y < vtx.y)
			{
				pItemType->vtxMaxItem.y = vtx.y;
			}
			if (pItemType->vtxMaxItem.z < vtx.z)
			{
				pItemType->vtxMaxItem.z = vtx.z;
			}

			// 各座標の最小値の比較
			if (pItemType->vtxMinItem.x > vtx.x)
			{
				pItemType->vtxMinItem.x = vtx.x;
			}
			if (pItemType->vtxMinItem.y > vtx.y)
			{
				pItemType->vtxMinItem.y = vtx.y;
			}
			if (pItemType->vtxMinItem.z > vtx.z)
			{
				pItemType->vtxMinItem.z = vtx.z;
			}

			if (pItemType->vtxMinItem.x > pItemType->vtxMinItem.z)
			{
				pItemType->vtxMinItem.z = pItemType->vtxMinItem.x;
			}
			if (pItemType->vtxMinItem.x < pItemType->vtxMinItem.z)
			{
				pItemType->vtxMinItem.x = pItemType->vtxMinItem.z;
			}

			if (pItemType->vtxMaxItem.x < pItemType->vtxMaxItem.z)
			{
				pItemType->vtxMaxItem.x = pItemType->vtxMaxItem.z;
			}
			if (pItemType->vtxMaxItem.x > pItemType->vtxMaxItem.z)
			{
				pItemType->vtxMaxItem.z = pItemType->vtxMaxItem.x;
			}

			pVtxBuff += sizeFVF[nCntItem];	// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		pItemType->pMeshItem->UnlockVertexBuffer();
	}

	// 構造体の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItemInfo++)
	{
		pItemInfo->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItemInfo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItemInfo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pItemInfo->bUse = false;
		pItemInfo->vtxMaxItem = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		pItemInfo->vtxMinItem = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	}
	g_Item.nSetItem = 0;

	// アイテムの配置
	//SetItem(D3DXVECTOR3(-500.0f, 15.0f, 0.0f), 0);
	//SetItem(D3DXVECTOR3(-400.0f, 15.0f, 0.0f), 1);
	//SetItem(D3DXVECTOR3(-600.0f, 15.0f, 0.0f), 2);
	//SetItem(D3DXVECTOR3(-800.0f, 15.0f, 0.0f), 3);

	return S_OK;
}

//==============================================================================
// アイテムの終了処理
//==============================================================================
void UninitItem(void)
{
	// ローカル変数宣言
	ItemTypeInfo *pItemType = &g_Item.typeinfo[0];

	// メッシュの破棄
	for (int nCntItem = 0; nCntItem < g_Item.nNumType; nCntItem++,pItemType++)
	{
		if (pItemType->pMeshItem != NULL)
		{
			pItemType->pMeshItem->Release();
			pItemType->pMeshItem = NULL;
		}

		// マテリアルの破棄
		if (pItemType->pBuffMatItem != NULL)
		{			 
			pItemType->pBuffMatItem->Release();
			pItemType->pBuffMatItem = NULL;
		}
	}
}

//==============================================================================
// アイテムの更新処理
//==============================================================================
void UpdateItem(void)
{
	// ローカル変数宣言
	ItemInfo *pItemInfo = &g_Item.iteminfo[0];
	Select *pSelect = GetSelect();

	// アイテムの回転
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItemInfo++)
	{
		if (pItemInfo->bUse == true)
		{// 使用状態か
			pItemInfo->rot.y += ITEM_TURN;	// 角度の加算

			if (pItemInfo->rot.y > D3DX_PI)
			{// 角度の数値１周時
				pItemInfo->rot.y -= D3DX_PI * 2.0f;
			}
		}
	}
	
	g_Item.nSetCnt++;

	if (g_Item.nSetItem < ITEM_SET_MAX && g_Item.nSetCnt % 600 == 0)
	{
		RandItem((int)pSelect->stage);
		g_Item.nSetCnt = 0;
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
	ItemInfo *pItemInfo = &g_Item.iteminfo[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItemInfo++)
	{
		if (pItemInfo->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pItemInfo->mtxWorld);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pItemInfo->rot.y, pItemInfo->rot.x, pItemInfo->rot.z);
			D3DXMatrixMultiply(&pItemInfo->mtxWorld, &pItemInfo->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pItemInfo->pos.x, pItemInfo->pos.y, pItemInfo->pos.z);
			D3DXMatrixMultiply(&pItemInfo->mtxWorld, &pItemInfo->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pItemInfo->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Item.typeinfo[(int)pItemInfo->type].pBuffMatItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Item.typeinfo[(int)pItemInfo->type].nNumMatItem; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// モデル(パーツ)の描画
				g_Item.typeinfo[(int)pItemInfo->type].pMeshItem->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==============================================================================
// アイテムの設定
//==============================================================================
void SetItem(D3DXVECTOR3 pos,int nType)
{
	// ローカル変数宣言
	ItemInfo *pItemInfo = &g_Item.iteminfo[0];
	ItemTypeInfo *pItemType;

	// アイテムの設定
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItemInfo++)
	{
		if (pItemInfo->bUse == false)
		{// アイテムが未使用か
			pItemInfo->pos = pos;	// 位置

			pItemInfo->type = (ItemType)nType; // アイテムの種類

			pItemType = &g_Item.typeinfo[(int)pItemInfo->type];

			// 各頂点位置
			pItemInfo->posPoint[0] = D3DXVECTOR3(pItemInfo->pos.x + pItemType->vtxMinItem.x, 0.0f, pItemInfo->pos.z + pItemType->vtxMinItem.x);
			pItemInfo->posPoint[1] = D3DXVECTOR3(pItemInfo->pos.x + pItemType->vtxMinItem.x, 0.0f, pItemInfo->pos.z + pItemType->vtxMaxItem.x);
			pItemInfo->posPoint[2] = D3DXVECTOR3(pItemInfo->pos.x + pItemType->vtxMaxItem.x, 0.0f, pItemInfo->pos.z + pItemType->vtxMaxItem.x);
			pItemInfo->posPoint[3] = D3DXVECTOR3(pItemInfo->pos.x + pItemType->vtxMaxItem.x, 0.0f, pItemInfo->pos.z + pItemType->vtxMinItem.x);

			// ４辺のベクトル
			pItemInfo->vecPoint[0] = pItemInfo->posPoint[1] - pItemInfo->posPoint[0];
			pItemInfo->vecPoint[1] = pItemInfo->posPoint[2] - pItemInfo->posPoint[1];
			pItemInfo->vecPoint[2] = pItemInfo->posPoint[3] - pItemInfo->posPoint[2];
			pItemInfo->vecPoint[3] = pItemInfo->posPoint[0] - pItemInfo->posPoint[3];

			// 影の設定
			pItemInfo->nIdx = SetShadow(D3DXVECTOR3(pItemInfo->pos.x, 0.0f, pItemInfo->pos.z), 10.0f, 10.0f);

			g_Item.nSetItem++;		// 配置アイテム数の加算

			pItemInfo->bUse = true;	// 使用状態へ移行

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
void RandItem(int nStage)
{
	// ローカル変数宣言
	int nRand = rand() % 100;
	int nRandPos = 0;
	D3DXVECTOR3 pos;
	
	while (g_Item.iteminfo[nRandPos].bSet == true)
	{
		nRandPos = rand() % 8;
	}

	pos = g_RandPos[nRandPos];
	g_Item.iteminfo[nRandPos].bSet = true;

	switch (nStage)
	{
	case 0:
		if (nRand < 5)
		{
			SetItem(pos, ITEMTYPE_RANCHER);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else if (nRand >= 5 && nRand < 30)
		{
			SetItem(pos, ITEMTYPE_MACHINEGUN);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else if (nRand >= 30 && nRand < 55)
		{
			SetItem(pos, ITEMTYPE_HANDGUN);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else
		{
			SetItem(pos, ITEMTYPE_KNIFE);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		break;

	case 1:
		if (nRand < 5)
		{
			SetItem(pos, ITEMTYPE_RANCHER);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else if (nRand >= 5 && nRand < 30)
		{
			SetItem(pos, ITEMTYPE_MACHINEGUN);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else if (nRand >= 30 && nRand < 55)
		{
			SetItem(pos, ITEMTYPE_HANDGUN);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else
		{
			SetItem(pos, ITEMTYPE_KNIFE);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		break;

	case 2:
		if (nRand < 5)
		{
			SetItem(pos, ITEMTYPE_RANCHER);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else if (nRand >= 5 && nRand < 30)
		{
			SetItem(pos, ITEMTYPE_MACHINEGUN);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else if (nRand >= 30 && nRand < 55)
		{
			SetItem(pos, ITEMTYPE_HANDGUN);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		else
		{
			SetItem(pos, ITEMTYPE_KNIFE);
			PlaySound(SOUND_LABEL_SE_SETITEM);
		}
		break;

	default:
		break;
	}
}

//==============================================================================
// アイテムの取得
//==============================================================================
void TouchItem(D3DXVECTOR3 *pPos, float fInRadius, float fHeight,int nCntPlayer)
{
	// ローカル変数宣言
	ItemInfo *pItemInfo = &g_Item.iteminfo[0];
	Player *pPlayer = GetPlayer();	// プレイヤーの取得
	D3DXVECTOR3 aVec[FOUR_POINT];	// 各頂点からプレイヤーへのベクトル
	float fItemVec[FOUR_POINT];		// 判定用変数
	D3DXVECTOR3 pos = *pPos;		// プレイヤーの位置

	pPlayer += nCntPlayer;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItemInfo++)
	{
		if (pItemInfo->bUse == true)
		{// アイテムの外積当たり判定
			ItemTypeInfo *pItemType = &g_Item.typeinfo[(int)pItemInfo->type];

			for (int nCnt = 0; nCnt < FOUR_POINT; nCnt++)
			{
				switch (nCnt)
				{
				case 0:	// 左側
					aVec[nCnt] = pos + D3DXVECTOR3(fInRadius, 0.0f, 0.0f) - pItemInfo->posPoint[nCnt];
					break;

				case 1:	// 奥側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, -fInRadius) - pItemInfo->posPoint[nCnt];
					break;

				case 2:	// 右側
					aVec[nCnt] = pos + D3DXVECTOR3(-fInRadius, 0.0f, 0.0f) - pItemInfo->posPoint[nCnt];
					break;

				case 3:	// 手前側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fInRadius) - pItemInfo->posPoint[nCnt];
					break;
				}

				// 判定用数値算出
				fItemVec[nCnt] = (pItemInfo->vecPoint[nCnt].z * aVec[nCnt].x) - (pItemInfo->vecPoint[nCnt].x * aVec[nCnt].z);
			}

			// アイテムとの接触
			if (fItemVec[0] > 0.0f && fItemVec[1] > 0.0f && fItemVec[2] > 0.0f && fItemVec[3] > 0.0f && 
				pPlayer->pos.y <= (pItemInfo->pos.y + pItemType->vtxMaxItem.y) && (pPlayer->pos.y + fHeight) >= pItemInfo->pos.y)
			{// アイテムの取得
				// エフェクトの発生
				SetEffect(D3DXVECTOR3(pItemInfo->pos.x, pItemInfo->pos.y + 5.0f, pItemInfo->pos.z), 0.01f, D3DXCOLOR(1.0f, 0.1f, 1.0f, 1.0f), 5.0f, 0.05f, 30);
				PlaySound(SOUND_LABEL_SE_ITEM);
				Shadow *pShadow = GetShadow();	// 影の取得
				pShadow += pItemInfo->nIdx;
				pShadow->bUse = false;			// 影の消滅
				pItemInfo->bUse = false;		// アイテムの消滅
				pItemInfo->bSet = false;
				g_Item.nSetItem--;
				pPlayer->weapon = (Weapon)(pItemInfo->type + 1);
				switch (pPlayer->weapon)
				{
				case WEAPON_HANDGUN:
					pPlayer->nBullet = 15;
					break;

				case WEAPON_MACHINEGUN:
					pPlayer->nBullet = 150;
					break;

				case WEAPON_RUNCHER:
					pPlayer->nBullet = 2;
					break;

				default:
					break;
				}
				if (pPlayer->weapon == WEAPON_KNIFE)
				{
					pPlayer->sr = SHORT_KNIFE;
				}
				else
				{
					pPlayer->sr = SHORT_NONE;
				}
				break;
			}
		}
	}
}

void LoadItem(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aEqual[2] = { NULL };				// 「=」読み取り用変数
	int nCntFile = 0;						// Xファイルの数
	int nIdx = 0;							// パーツ数
	bool bComment = false;					// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };		// テキスト読み取り用変数
	//char aItem[TEXT_LENGTH] = { NULL };	// ポリゴン情報読み込み用変数
	//char aState[TEXT_LENGTH] = { NULL };	// 詳細情報読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	//char *pItem = &aItem[0];
	//char *pState = &aState[0];

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
				{// 文字列の先頭が「#」ならばコメントアウトへ
					bComment = true;
				}
				else
				{// 通常時
					if (strcmp("NUM_ITEM", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &g_Item.nNumType);
					}
					else if (strcmp("ITEM_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &g_Item.typeinfo[nCntFile].aFileName[0]);
						nCntFile++;
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