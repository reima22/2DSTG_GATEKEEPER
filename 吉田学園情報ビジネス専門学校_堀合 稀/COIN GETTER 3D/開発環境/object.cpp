//==============================================================================
//
// 3Dモデルの描画〔object.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "object.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "math.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPD3DXMESH g_pMeshObject = NULL;			// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatObject = NULL;		// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatObject = 0;					// マテリアルの数
//D3DXVECTOR3 g_vtxMinObject, g_vtxMaxObject;	// モデルの各座標のの最大値・最小値
Object object;
D3DXVECTOR3 g_aPos[4];					// 当たり判定頂点情報
D3DXVECTOR3 vec;
D3DXVECTOR3 g_aVec[4];					// 当たり判定ベクトル

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
		&g_pBuffMatObject,
		NULL,
		&g_nNumMatObject,
		&g_pMeshObject);

	// ローカル変数宣言
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_pMeshObject->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshObject->GetFVF());

	// 変数の初期化
	object.pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	object.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	object.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	object.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 影の設定
	object.nIdx = SetShadow(D3DXVECTOR3(object.pos.x,0.0f,object.pos.z), 10.0f, 10.0f);

	// 頂点バッファをロック
	g_pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 各座標の最大値の比較
		if (object.vtxMaxObject.x < vtx.x)
		{	
			object.vtxMaxObject.x = vtx.x;
		}	
		if (object.vtxMaxObject.y < vtx.y)
		{	
			object.vtxMaxObject.y = vtx.y;
		}	
		if (object.vtxMaxObject.z < vtx.z)
		{	
			object.vtxMaxObject.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (object.vtxMinObject.x > vtx.x)
		{	
			object.vtxMinObject.x = vtx.x;
		}	
		if (object.vtxMinObject.y > vtx.y)
		{	
			object.vtxMinObject.y = vtx.y;
		}	
		if (object.vtxMinObject.z > vtx.z)
		{	
			object.vtxMinObject.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	g_pMeshObject->UnlockVertexBuffer();

	// オブジェクト矩形の各頂点
	g_aPos[0] = D3DXVECTOR3(object.pos.x + object.vtxMinObject.x, 0.0f, object.pos.z + object.vtxMinObject.z);	// 左手前
	g_aPos[1] = D3DXVECTOR3(object.pos.x + object.vtxMinObject.x, 0.0f, object.pos.z + object.vtxMaxObject.z);	// 左奥
	g_aPos[2] = D3DXVECTOR3(object.pos.x + object.vtxMaxObject.x, 0.0f, object.pos.z + object.vtxMaxObject.z);	// 右奥
	g_aPos[3] = D3DXVECTOR3(object.pos.x + object.vtxMaxObject.x, 0.0f, object.pos.z + object.vtxMinObject.z);	// 右手前

	// オブジェクト矩形の各面のベクトル
	g_aVec[0] = g_aPos[1] - g_aPos[0];
	g_aVec[1] = g_aPos[2] - g_aPos[1];
	g_aVec[2] = g_aPos[3] - g_aPos[2];
	g_aVec[3] = g_aPos[0] - g_aPos[3];

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitObject(void)
{
	// メッシュの破棄
	if (g_pMeshObject != NULL)
	{		   
		g_pMeshObject->Release();
		g_pMeshObject = NULL;
	}

	// マテリアルの破棄
	if (g_pBuffMatObject != NULL)
	{			  
		g_pBuffMatObject->Release();
		g_pBuffMatObject = NULL;
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

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&object.mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, object.rot.y, object.rot.x, object.rot.z);
	D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, object.pos.x, object.pos.y, object.pos.z);
	D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &object.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		g_pMeshObject->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// オブジェクトの取得
//==============================================================================
Object *GetObject(void)
{
	return &object;
}

//==============================================================================
// 当たり判定
//==============================================================================
bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin)
{
	bool bLand = false;

	if (pPos->x >= pPosOld->x && pPos->x < object.pos.x)
	{ // 左側の判定
		if (pPos->z + fDepthMax >= object.pos.z + object.vtxMinObject.z &&
			pPos->z + fDepthMin <= object.pos.z + object.vtxMaxObject.z &&
			pPos->x + fWidthMax >= object.pos.x + object.vtxMinObject.x &&
			pPos->x + fWidthMin <= object.pos.x + object.vtxMaxObject.x)
		{ // ブロックの当たり判定
			pPos->x = object.pos.x + object.vtxMinObject.x - fWidthMax;	// 足場の更新

			pMove->x = 0.0f;	// 足場への定着
		}
	}
	else if (pPos->x <= pPosOld->x && pPos->x > object.pos.x)
	{ // 右側の判定
		if (pPos->z + fDepthMax >= object.pos.z + object.vtxMinObject.z &&
			pPos->z + fDepthMin <= object.pos.z + object.vtxMaxObject.z &&
			pPos->x + fWidthMax >= object.pos.x + object.vtxMinObject.x &&
			pPos->x + fWidthMin <= object.pos.x + object.vtxMaxObject.x)
		{ // ブロックの当たり判定
			pPos->x = object.pos.x + object.vtxMaxObject.x - fWidthMin;
			pMove->x = 0.0f;
		}
	}
	else if (pPos->z >= pPosOld->z && pPos->z < object.pos.z)
	{ // 手前側の判定
		if (pPos->z + fDepthMax >= object.pos.z + object.vtxMinObject.z &&
			pPos->z + fDepthMin <= object.pos.z + object.vtxMaxObject.z &&
			pPos->x + fWidthMax >= object.pos.x + object.vtxMinObject.x &&
			pPos->x + fWidthMin <= object.pos.x + object.vtxMaxObject.x)
		{ // ブロックの当たり判定
			pPos->z = object.pos.z + object.vtxMinObject.z - fDepthMax;
			pMove->z = 0.0f;
		}
	}
	else if (pPos->z <= pPosOld->z && pPos->z > object.pos.z)
	{ // 奥側の判定
		if (pPos->z + fDepthMax >= object.pos.z + object.vtxMinObject.z &&
			pPos->z + fDepthMin <= object.pos.z + object.vtxMaxObject.z &&
			pPos->x + fWidthMax >= object.pos.x + object.vtxMinObject.x &&
			pPos->x + fWidthMin <= object.pos.x + object.vtxMaxObject.x)
		{ // ブロックの当たり判定
			pPos->z = object.pos.z + object.vtxMaxObject.z - fDepthMin;
			pMove->z = 0.0f;
		}
	}

	return bLand;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ローカル変数宣言
	bool bLand;
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル
	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
	Player *pPlayer = GetPlayer();

	if (pPlayer->bOnBlock == false)
	{
		bLand = false;
	}
	else
	{
		bLand = true;
	}

	// オブジェクトとの当たり判定
	for (int nCnt = 0; nCnt < COLLISION_PARTS; nCnt++)
	{
		if (nCnt == 0)
		{// 左面
			aVec[nCnt] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - g_aPos[nCnt];
		}
		else if (nCnt == 1)
		{// 奥側
			aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - g_aPos[nCnt];
		}
		else if (nCnt == 2)
		{// 右面
			aVec[nCnt] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - g_aPos[nCnt];
		}
		else if (nCnt == 3)
		{// 手前
			aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - g_aPos[nCnt];
		}

		// 数値が「+」の時、当たり判定が有効
		object.fPlayerVec[nCnt] = (g_aVec[nCnt].z * aVec[nCnt].x) - (g_aVec[nCnt].x * aVec[nCnt].z);	

		if (pPos->y <= object.pos.y + object.vtxMaxObject.y - fHeightMin && pPos->y > object.pos.y + object.vtxMinObject.y - fHeightMax)
		{
			if (object.fPlayerVec[0] > 0.0f && object.fPlayerVec[1] > 0.0f && object.fPlayerVec[2] > 0.0f && object.fPlayerVec[3] > 0.0f)
			{
				if (pPos->y <= pPosOld->y &&
					pPos->y <= (object.pos.y + object.vtxMaxObject.y) &&
					pPosOld->y >= (object.pos.y + object.vtxMaxObject.y) &&
					object.vtxMaxObject.y >= pPos->y)
				{// 上側
					pPos->y = object.pos.y + object.vtxMaxObject.y;
					bLand = true;
				}
				else if (pPos->x > pPosOld->x && pPosOld->x < object.pos.x + object.vtxMinObject.x)
				{// 左側	
					pPos->x = object.pos.x + object.vtxMinObject.x - fWidthMax;
				}
				else if (pPos->x < pPosOld->x && pPosOld->x > object.pos.x + object.vtxMaxObject.x)
				{// 右側	
					pPos->x = object.pos.x + object.vtxMaxObject.x - fWidthMin;
				}
				else if (pPos->z <= pPosOld->z && pPos->z > object.pos.z)
				{// 奥側
					pPos->z = object.pos.z + object.vtxMaxObject.z - fDepthMin;
				}
				else if (pPos->z >= pPosOld->z && pPos->z < object.pos.z)
				{// 手前
					pPos->z = object.pos.z + object.vtxMinObject.z - fDepthMax;
				}
			}
		}
	}

	return bLand;
}