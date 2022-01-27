//==============================================================================
//
// 当たり判定管理〔collision.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "collision.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CCollision::CCollision()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CCollision::~CCollision()
{

}

//==============================================================================
// 当たり判定ポインタの生成
//==============================================================================
CCollision *CCollision::Create(void)
{
	// 当たり判定
	CCollision *pCollision;
	pCollision = new CCollision;

	if (pCollision != NULL)
	{
		pCollision->SetCollisionBool(false);
	}

	return pCollision;
}

//==============================================================================
// 3Dモデル間の当たり判定処理
//==============================================================================
bool CCollision::Collision3D(CPlayer *pPlayer, CModel *pObject)
{
	// ローカル変数宣言
	D3DXVECTOR3 aVec[COLLISION_NUM];				// 矩形頂点から判定対象へのベクトル

	// プレイヤー情報
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// 判定対象の位置
	D3DXVECTOR3 posOld = pPlayer->GetPosOld();		// プレイヤーの過去の位置
	float fRadius = pPlayer->GetRadius();			// プレイヤー半径の取得
	float fHeight = pPlayer->GetHeight();			// プレイヤー高さの取得
	D3DXVECTOR3 move = pPlayer->GetMove();			// プレイヤーの移動力

	// オブジェクト情報
	D3DXVECTOR3 posObject = pObject->GetPosition();		// オブジェクトの位置
	D3DXVECTOR3 vtxMin = pObject->GetVtxMin();			// オブジェクト6方の最小値
	D3DXVECTOR3 vtxMax = pObject->GetVtxMax();			// オブジェクト6方の最大値
	D3DXVECTOR3 moveObject = pObject->GetMove();		// オブジェクトの移動力
	D3DXVECTOR3 posOldObject = pObject->GetPosOld();	// オブジェクトの過去の位置

	float fPlayerVec[COLLISION_NUM];					// プレイヤーへのベクトル数値

	m_bOnObject = false;
	m_bUnderObject = false;
	m_bCollision = false;

	// オブジェクトとの当たり判定
	for (int nCnt = 0; nCnt < COLLISION_NUM; nCnt++)
	{
		// ローカル変数宣言
		D3DXVECTOR3 posVtx = pObject->GetPosVtx(nCnt);
		D3DXVECTOR3 vecVtx = pObject->GetVecVtx(nCnt);

		if (nCnt == 0)
		{// 左面
			aVec[nCnt] = posPlayer + D3DXVECTOR3(fRadius, 0.0f, 0.0f) - posVtx;
		}
		else if (nCnt == 1)
		{// 奥側
			aVec[nCnt] = posPlayer + D3DXVECTOR3(0.0f, 0.0f, -fRadius) - posVtx;
		}
		else if (nCnt == 2)
		{// 右面
			aVec[nCnt] = posPlayer + D3DXVECTOR3(-fRadius, 0.0f, 0.0f) - posVtx;
		}
		else if (nCnt == 3)
		{// 手前
			aVec[nCnt] = posPlayer + D3DXVECTOR3(0.0f, 0.0f, fRadius) - posVtx;
		}

		// 数値が「+」の時、当たり判定が有効
		fPlayerVec[nCnt] = (vecVtx.z * aVec[nCnt].x) - (vecVtx.x * aVec[nCnt].z);
	}

	// 当たり判定
	if (fPlayerVec[0] > 0.0f && fPlayerVec[1] > 0.0f && fPlayerVec[2] > 0.0f && fPlayerVec[3] > 0.0f)
	{// 算出したベクトルの範囲内判定
		if (posPlayer.y >= posObject.y + vtxMax.y)
		{
			m_bOnObject = true;
		}
		else if (posOld.y + fHeight < posObject.y + vtxMin.y)
		{
			m_bUnderObject = true;
		}

		if ((posPlayer.y <= (posObject.y + vtxMax.y) &&
			posPlayer.y > posObject.y + vtxMin.y - fHeight) || 
			posPlayer.y == posOldObject.y + vtxMax.y)
		{
			// 下降時の補正
			if (moveObject.y < 0.0f && m_bOnObject == true)
			{
				posPlayer.y = posObject.y + vtxMax.y + moveObject.y;
			}

			if (posPlayer.y <= posObject.y + vtxMax.y &&
				(posOld.y >= posObject.y + vtxMax.y || posPlayer.y >= posOldObject.y + vtxMax.y))
			{// 上側
				//if (posPlayer.y < posObject.y + vtxMax.y)
				{
					m_bCollision = true;
				}

				// 位置の反映
				posPlayer.y = posObject.y + vtxMax.y;

				// 移動力の反映
				posPlayer.x += moveObject.x;				
				posPlayer.z += moveObject.z;
			}
			else if (posPlayer.y + fHeight > posObject.y + vtxMin.y && posOld.y < posObject.y + vtxMin.y && m_bUnderObject == true)
			{// 下側
				posPlayer.y = posObject.y + vtxMin.y - fHeight;
				pPlayer->SetMove(D3DXVECTOR3(move.x, -move.y / 2.0f, move.z));
			}
			else if (posPlayer.x < posObject.x && posOld.x < posObject.x + vtxMin.x)
			{// 左側	
				posPlayer.x = posObject.x + vtxMin.x - fRadius - 1.0f;
				pPlayer->SetDashCnt(0);
			}
			else if (posPlayer.x > posObject.x && posOld.x > posObject.x + vtxMax.x)
			{// 右側	
				posPlayer.x = posObject.x + vtxMax.x + fRadius + 1.0f;
				pPlayer->SetDashCnt(0);
			}
			else if (posPlayer.z >= posObject.z)
			{// 奥側
				posPlayer.z = posObject.z + vtxMax.z + fRadius + 1.0f;
				pPlayer->SetDashCnt(0);
			}
			else if (posPlayer.z <= posObject.z)
			{// 手前
				posPlayer.z = posObject.z + vtxMin.z - fRadius - 1.0f;
				pPlayer->SetDashCnt(0);
			}
		}

		// 位置の反映
		pPlayer->SetPosition(posPlayer);
	}

	// オブジェクトの過去の位置を更新
	pObject->SetPosOld(posObject);

	return m_bCollision;
}

//==============================================================================
// メッシュフィールドの判定
//==============================================================================
bool CCollision::CollisionMesh(CPlayer *pPlayer, CMeshfield *pMesh)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	D3DXVECTOR3 vec[3];
	D3DXVECTOR3 vecA, vecB, vecC;
	float fPlayerVec[3];

	int nWidth = pMesh->GetWidthNum();
	int nDepth = pMesh->GetDepthNum();
	int nWidthPoint = pMesh->GetWidthPoint();
	int nDepthPoint = pMesh->GetDepthPoint();

	// プレイヤーの情報
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();		// 現在の位置
	D3DXVECTOR3 posPlayerOld = pPlayer->GetPosOld();	// 過去の位置
	bool bLand = false;									// プレイヤーの着地状態
	bool bInMesh = false;								// メッシュの範囲内

	// バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pMesh->GetVtxBuff();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 各面の算出
	for (int nCntDepth = 0; nCntDepth < nDepth; nCntDepth++)
	{// 奥行面数
		int nCntOdd = 0;	// 奇数カウント
		int nCntEven = 0;	// 偶数カウント

		for (int nCntWidth = 0; nCntWidth < nWidth * 2; nCntWidth++)
		{// 横幅頂点数（三角形ポリゴン数）						
			if (nCntWidth % 2 == 0)
			{
				// 偶数時ベクトル
				vecA =
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) - nWidthPoint + nCntEven].pos -
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven].pos;

				vecB =
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven + 1].pos -
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) - nWidthPoint + nCntEven].pos;

				vecC =
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven].pos -
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven + 1].pos;

				vec[0] = posPlayer - pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven].pos;
				vec[1] = posPlayer - pVtx[nWidthPoint + (nWidthPoint * nCntDepth) - nWidthPoint + nCntEven].pos;
				vec[2] = posPlayer - pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven + 1].pos;

				nCntEven++;		// 偶数カウント
			}
			else
			{
				// 奇数時ベクトル
				vecA =
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth + nWidthPoint].pos -
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth].pos;

				vecB =
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth - 1].pos -
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth + nWidthPoint].pos;

				vecC =
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth].pos -
					pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth - 1].pos;

				vec[0] = posPlayer - pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth].pos;
				vec[1] = posPlayer - pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth + nWidthPoint].pos;
				vec[2] = posPlayer - pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth - 1].pos;

				nCntOdd++;		// 奇数カウント
			}

			fPlayerVec[0] = (vecA.z * vec[0].x) - (vec[0].z * vecA.x);
			fPlayerVec[1] = (vecB.z * vec[1].x) - (vec[1].z * vecB.x);
			fPlayerVec[2] = (vecC.z * vec[2].x) - (vec[2].z * vecC.x);

			if (fPlayerVec[0] >= 0.0f && fPlayerVec[1] >= 0.0f && fPlayerVec[2] >= 0.0f)
			{// メッシュ領域内
				bInMesh = true;

				// ローカル変数宣言
				D3DXVECTOR3 vec0, vec1, vec2, vecP, nor;	// 計算用ベクトル
				float fHeight = 0;							// 計算するプレイヤーの高さ

				if (nCntWidth % 2 == 0)
				{
					nCntEven--;

					vec0 = pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven].pos;
					vec1 =
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) - nWidthPoint + nCntEven].pos -
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven].pos;

					vec2 =
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven + 1].pos -
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntEven].pos;
				}
				else
				{
					nCntOdd--;

					vec0 = pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth].pos;

					vec1 =
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth + nWidthPoint].pos -
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth].pos;

					vec2 =
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth - 1].pos -
						pVtx[nWidthPoint + (nWidthPoint * nCntDepth) + nCntOdd - nWidth].pos;
				}

				// 特定の頂点からプレイヤーへのベクトル
				vecP = posPlayer - vec0;

				// 法線ベクトルの計算
				D3DXVec3Cross(&nor, &vec1, &vec2);

				// 法線の正規化
				D3DXVec3Normalize(&nor, &nor);

				// 内積が0であることを利用したプレイヤーの高さ計算
				if (nor.y != 0.0f)
				{
					fHeight = (-vecP.x * nor.x - vecP.z * nor.z) / nor.y + vec0.y;

					// 位置の設定
					if (posPlayer.y <= fHeight /*&& (posPlayerOld.y >= fHeight || posPlayerOld.y < fHeight)*/)
					{// メッシュに接地した
						// 横移動からの貫通補正
						if (posPlayer.y <= posPlayerOld.y)
						{
							if (posPlayerOld.y < fHeight)
							{
								posPlayerOld.y = fHeight;
							}
						}
						else if (posPlayer.y > posPlayerOld.y)
						{
							if (posPlayerOld.y > fHeight)
							{
								posPlayerOld.y = fHeight;
							}
						}

						// メッシュの上である
						if ((posPlayer.y <= posPlayerOld.y && posPlayerOld.y >= fHeight) ||
							(posPlayer.y > posPlayerOld.y && posPlayerOld.y < fHeight))
						{
							bLand = true;
						}

											if (bLand == true)
					{
						pPlayer->SetPosition(D3DXVECTOR3(posPlayer.x, fHeight, posPlayer.z));
						return bLand;
					}
					}


					

					//if(posPlayer.y <= fHeight && posPlayerOld.y >= fHeight)
					//{
					//	pPlayer->SetPosition(D3DXVECTOR3(posPlayer.x, fHeight, posPlayer.z));
					//	bLand = true;
					//}
					//else if (posPlayer.y < posPlayerOld.y && posPlayerOld.y > fHeight)
					//{
					//	if (posPlayer.y <= fHeight)
					//	{
					//		pPlayer->SetPosition(D3DXVECTOR3(posPlayer.x, fHeight, posPlayer.z));
					//		bLand = true;
					//	}
					//}
					//else if (posPlayerOld.y < fHeight)
					//{
					//	if (posPlayer.y >= posPlayerOld.y)
					//	{
					//		pPlayer->SetPosition(D3DXVECTOR3(posPlayer.x, fHeight, posPlayer.z));
					//		bLand = true;
					//	}
					//}
					//else if (posPlayer.y > fHeight && posPlayer.y >= posPlayerOld.y)
					//{
					//	pPlayer->SetPosition(D3DXVECTOR3(posPlayer.x, fHeight, posPlayer.z));
					//	bLand = true;
					//}
				}
			}
		}
	}

	if (bInMesh == false)
	{
		//pPlayer->SetLandMesh(false);
		return false;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	return bLand;
}