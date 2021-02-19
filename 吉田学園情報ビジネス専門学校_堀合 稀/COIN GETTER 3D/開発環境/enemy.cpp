//==============================================================================
//
// 敵の描画〔enemy.cpp〕
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

// マクロ定義
#define COLLISION_PARTS	(4)			// 当たり判定の面の数
#define MAX_ENEMY		(8)			// 敵の最大数
#define MOVE_ENEMY0		(100.0f)	// 敵の移動範囲数値
#define MOVE_ENEMY1		(150.0f)	// 敵の移動範囲数値
#define MOVE0			(2.0f)		// 敵の移動力
#define MOVE1			(3.0f)		// 敵の移動力

//==============================================================================
// グローバル変数
//==============================================================================
LPD3DXMESH g_pMeshEnemy = NULL;			// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy = NULL;	// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatEnemy = 0;				// マテリアルの数
Enemy g_enemy[MAX_ENEMY];				// 敵の構造体

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitEnemy(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/cube.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy,
		NULL,
		&g_nNumMatEnemy,
		&g_pMeshEnemy);

	// ローカル変数宣言
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_pMeshEnemy->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy->GetFVF());

	// 変数の初期化
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

	// 頂点座標の比較
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 頂点バッファをロック
		g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
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

			// 各座標の最小値の比較
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

			pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		g_pMeshEnemy->UnlockVertexBuffer();
	}

	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, MOVE_ENEMY0), D3DXVECTOR3(MOVE0, 0.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -MOVE_ENEMY0), D3DXVECTOR3(-MOVE0, 0.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, MOVE_ENEMY1), D3DXVECTOR3(-MOVE1, 0.0f, 0.0f), 1);


	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitEnemy(void)
{
	// メッシュの破棄
	if (g_pMeshEnemy != NULL)
	{
		g_pMeshEnemy->Release();
		g_pMeshEnemy = NULL;
	}

	// マテリアルの破棄
	if (g_pBuffMatEnemy != NULL)
	{
		g_pBuffMatEnemy->Release();
		g_pBuffMatEnemy = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateEnemy(void)
{
	// ローカル変数宣言
	Enemy *pEnemy = &g_enemy[0];
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 vec;

	// 敵の移動処理
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//vec = pPlayer->pos - pEnemy->pos;	// ベクトル算出
			//pEnemy->rot = vec;
			//pEnemy->move = (pPlayer->pos - pEnemy->pos) * 0.01f;
			// 移動力算出
			//pEnemy->move.x = sinf(D3DX_PI * vec.x);
			//pEnemy->move.z = cosf(D3DX_PI * vec.z);

			switch (pEnemy->nMovePattern)
			{
			case 0:
				if (pEnemy->pos == D3DXVECTOR3(MOVE_ENEMY0, 0.0f, MOVE_ENEMY0))
				{// 右上到達
					pEnemy->move.x = 0.0f;
					pEnemy->move.z = -MOVE0;
				}
				else if (pEnemy->pos == D3DXVECTOR3(MOVE_ENEMY0, 0.0f, -MOVE_ENEMY0))
				{// 右下到達
					pEnemy->move.x = -MOVE0;
					pEnemy->move.z = 0.0f;
				}
				else if (pEnemy->pos == D3DXVECTOR3(-MOVE_ENEMY0, 0.0f, -MOVE_ENEMY0))
				{// 左下到達
					pEnemy->move.x = 0.0f;
					pEnemy->move.z = MOVE0;
				}
				else if (pEnemy->pos == D3DXVECTOR3(-MOVE_ENEMY0, 0.0f, MOVE_ENEMY0))
				{// 左上到達
					pEnemy->move.x = MOVE0;
					pEnemy->move.z = 0.0f;
				}
				break;

			case 1:
				if (pEnemy->pos == D3DXVECTOR3(MOVE_ENEMY1, 0.0f, MOVE_ENEMY1))
				{// 右上到達
					pEnemy->move.x = -MOVE1;
					pEnemy->move.z = 0.0f;
				}
				else if (pEnemy->pos == D3DXVECTOR3(MOVE_ENEMY1, 0.0f, -MOVE_ENEMY1))
				{// 右下到達
					pEnemy->move.x = 0.0f;
					pEnemy->move.z = MOVE1;
				}
				else if (pEnemy->pos == D3DXVECTOR3(-MOVE_ENEMY1, 0.0f, -MOVE_ENEMY1))
				{// 左下到達
					pEnemy->move.x = MOVE1;
					pEnemy->move.z = 0.0f;
				}
				else if (pEnemy->pos == D3DXVECTOR3(-MOVE_ENEMY1, 0.0f, MOVE_ENEMY1))
				{// 左上到達
					pEnemy->move.x = 0.0f;
					pEnemy->move.z = -MOVE1;
				}
				break;

			default:
				break;
			}

			// 移動力の加算
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.z += pEnemy->move.z;

			// 影の追従
			SetPositionShadow(pEnemy->nIdx, D3DXVECTOR3(pEnemy->pos.x, 0.0f, pEnemy->pos.z));

			// 頂点位置の算出
			pEnemy->posPoint[0] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMinObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMinObject.x);
			pEnemy->posPoint[1] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMinObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMaxObject.x);
			pEnemy->posPoint[2] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMaxObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMaxObject.x);
			pEnemy->posPoint[3] = D3DXVECTOR3(pEnemy->pos.x + pEnemy->vtxMaxObject.x, 0.0f, pEnemy->pos.z + pEnemy->vtxMinObject.x);

			// 四辺ベクトルの算出
			pEnemy->vecPoint[0] = pEnemy->posPoint[1] - pEnemy->posPoint[0];
			pEnemy->vecPoint[1] = pEnemy->posPoint[2] - pEnemy->posPoint[1];
			pEnemy->vecPoint[2] = pEnemy->posPoint[3] - pEnemy->posPoint[2];
			pEnemy->vecPoint[3] = pEnemy->posPoint[0] - pEnemy->posPoint[3];
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawEnemy(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
	Enemy *pEnemy = &g_enemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pEnemy->mtxWorld);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEnemy->rot.y, pEnemy->rot.x, pEnemy->rot.z);
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEnemy->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// モデル(パーツ)の描画
				g_pMeshEnemy->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==============================================================================
// オブジェクトの取得
//==============================================================================
Enemy *GetEnemy(void)
{
	return &g_enemy[0];
}
//==============================================================================
// 敵の設定
//==============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nMove)
{
	// ローカル変数宣言
	Enemy *pEnemy = &g_enemy[0];

	// 敵の設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{// 未使用状態の時
			pEnemy->pos = pos;				// 位置

			pEnemy->move = move;			// 移動力
			
			pEnemy->nMovePattern = nMove;	// 行動パターン
			
			pEnemy->nIdx = SetShadow(D3DXVECTOR3(pEnemy->pos.x, 0.0f, pEnemy->pos.z), 15.0f, 15.0f);	// 影の設定

			pEnemy->bUse = true;			// 使用状態へ
			
			break;
		}
	}
}

//==============================================================================
// 敵との外積利用した接触判定
//==============================================================================
bool TouchEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ローカル変数宣言
	Enemy *pEnemy = &g_enemy[0];
	Player *pPlayer = GetPlayer();		// プレイヤーの取得
	D3DXVECTOR3 aVec[FOUR_POINT];		// 4頂点とプレイヤーのベクトル
	D3DXVECTOR3 pos = *pPos;			// プレイヤーの位置
	bool bOnBlock;						// 乗っているか

	// 地面、オブジェクトに乗っているか
	if (pPlayer->bOnBlock == false)
	{
		bOnBlock = false;
	}
	else
	{
		bOnBlock = true;
	}

	// 敵ごとの判定
	for (int nCntItem = 0; nCntItem < MAX_ENEMY; nCntItem++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// アイテムの外積当たり判定
			for (int nCnt = 0; nCnt < FOUR_POINT; nCnt++)
			{
				switch (nCnt)
				{
				case 0:	// 左辺のベクトル
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - pEnemy->posPoint[nCnt];
					break;

				case 1:	// 奥側のベクトル
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - pEnemy->posPoint[nCnt];
					break;

				case 2:	// 右辺のベクトル
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - pEnemy->posPoint[nCnt];
					break;

				case 3:	// 手前側のベクトル
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - pEnemy->posPoint[nCnt];
					break;
				}

				// 判定用数値の算出
				pEnemy->fVec[nCnt] = (pEnemy->vecPoint[nCnt].z * aVec[nCnt].x) - (pEnemy->vecPoint[nCnt].x * aVec[nCnt].z);
			}

			// 当たり判定
			if (pPos->y <= pEnemy->pos.y + pEnemy->vtxMaxObject.y - fHeightMin && pPos->y > pEnemy->pos.y + pEnemy->vtxMinObject.y - fHeightMax)
			{// 敵の高さより下か
				if (pEnemy->fVec[0] > 0.0f && pEnemy->fVec[1] > 0.0f && pEnemy->fVec[2] > 0.0f && pEnemy->fVec[3] > 0.0f)
				{// 敵の四方ベクトル範囲内か
					if (pPos->y <= pPosOld->y && 
						pPos->y <= (pEnemy->pos.y + pEnemy->vtxMaxObject.y) && 
						pPosOld->y >= (pEnemy->pos.y + pEnemy->vtxMaxObject.y) &&
						pEnemy->vtxMaxObject.y >= pPos->y)
					{// 上側
						pPos->y = pEnemy->pos.y + pEnemy->vtxMaxObject.y;
						bOnBlock = true;
					}
					else if (pPos->x > pPosOld->x && pPosOld->x < pEnemy->pos.x + pEnemy->vtxMinObject.x && pPlayer->state == PLAYERSTATE_NORMAL)
					{// 左側	
						pPos->x = pEnemy->pos.x + pEnemy->vtxMinObject.x - fWidthMax;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
					}
					else if (pPos->x < pPosOld->x && pPosOld->x > pEnemy->pos.x + pEnemy->vtxMaxObject.x && pPlayer->state == PLAYERSTATE_NORMAL)
					{// 右側	
						pPos->x = pEnemy->pos.x + pEnemy->vtxMaxObject.x - fWidthMin;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
					}
					else if (pPos->z <= pPosOld->z && pPos->z > pEnemy->pos.z && pPlayer->state == PLAYERSTATE_NORMAL)
					{// 奥側
						pPos->z = pEnemy->pos.z + pEnemy->vtxMaxObject.z - fDepthMin;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
					}
					else if (pPos->z >= pPosOld->z && pPos->z < pEnemy->pos.z && pPlayer->state == PLAYERSTATE_NORMAL)
					{// 手前
						pPos->z = pEnemy->pos.z + pEnemy->vtxMinObject.z - fDepthMax;
						pPlayer->state = PLAYERSTATE_DAMAGE;
						pPlayer->nCntState = 9;
						pPlayer->nLife--;
					}
				}
			}
		}
	}

	return bOnBlock;
}