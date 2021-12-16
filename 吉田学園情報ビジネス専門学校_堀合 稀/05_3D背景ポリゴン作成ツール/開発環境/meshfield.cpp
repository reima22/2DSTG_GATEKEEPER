//==============================================================================
//
// メッシュフィールドの描画〔meshfield.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "meshfield.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "textdata.h"
#include "texture.h"
#include "input.h"
#include "scene2D.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CMeshfield::CMeshfield()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CMeshfield::~CMeshfield()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CMeshfield::Init(void)
{
	// ローカル変数宣言
	VERTEX_3D_1 *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの取得
	CTexture *pTexture = CManager::GetTexture();

	// テキストのデータ
	CTextData *pData;
	pData = CManager::GetTextData();

	// データの割り当て
	m_pos = pData->GetPosition();		// 位置
	m_rot = pData->GetRotation();		// 角度
	m_nWidth = pData->GetWidthNum();	// 幅枚数
	m_nDepth = pData->GetDepthNum();	// 奥行枚数
	m_fWidth = pData->GetSize().x;		// サイズ横幅
	m_fDepth = pData->GetSize().y;		// サイズ奥行

	m_fHeightWave = pData->GetHeightWave();						// 波の高さ
	m_fDistanceWave = pData->GetDistanceWave();					// 波の間隔
	m_fSpeedWave = pData->GetSpeedWave();						// 波の早さ

	m_syntheticType = (SYNTHETICTYPE)pData->GetSyntheticType();	// 合成方法
	m_waveType = (WAVETYPE)pData->GetWaveType();				// 波の方向
	m_bCutTex = pData->GetCutTex();								// テクスチャ分割
	m_bWireFrame = false;										// ワイヤーフレームの有効化

	// テクスチャ機能の初期化
	m_aTexMoveRot[TEXTUREINFO_0] = 0.0f;
	m_aTexMove[TEXTUREINFO_0] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_0]) / 100.0f, cosf(m_aTexMoveRot[TEXTUREINFO_0]) / 100.0f);

	m_aTexMoveRot[TEXTUREINFO_1] = 0.0f;
	m_aTexMove[TEXTUREINFO_1] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_1]) / 100.0f, cosf(m_aTexMoveRot[TEXTUREINFO_1]) / 100.0f);

	// テクスチャの設定
	m_pTexture[TEXTUREINFO_0] = pTexture->GetAddress(pData->GetType());
	m_pTexture[TEXTUREINFO_1] = pTexture->GetAddress(pData->GetTypeSub());
	m_nTextureNum = pTexture->GetTexMax();
	m_aTextureIdx[TEXTUREINFO_0] = pData->GetType();
	m_aTextureIdx[TEXTUREINFO_1] = pData->GetTypeSub();

	// UIの生成
	CreateUI();

	// 各変数の計算
	m_nAllPoint = m_nDepth * 2 * (m_nWidth + 2) - 2;				// 総頂点数
	m_nPolygon = m_nWidth * m_nDepth * 2 + (4 * (m_nDepth - 1));	// 総ポリゴン数
	m_nWidthPoint = m_nWidth + 1;
	m_nDepthPoint = m_nDepth + 1;

	m_nIdxPoint = m_nWidthPoint * m_nDepthPoint;					// インデックスバッファでの頂点数

	float fWidthMax = m_fWidth * (float)m_nWidth;
	float fDepthMax = m_fDepth * (float)m_nDepth;

	// 法線を求めるポリゴン数
	m_nNor = m_nWidth * m_nDepth * 2;

	// 法線値動的確保
	m_pNor = new D3DXVECTOR3[m_nNor];

	m_nIdxNor = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D_1) * m_nIdxPoint,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_1,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepthPoint; nCnt++)
	{// 奥行軸
		for (int nCntA = 0; nCntA < m_nWidthPoint; nCntA++)
		{// 横軸
		 // ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(
				m_pos.x - fWidthMax / 2.0f + (float)nCntA * m_fWidth,
				m_pos.y,
				m_pos.z + fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (m_bCutTex == true)
			{
				// テクスチャ頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

				// テクスチャ頂点情報の設定
				pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);
			}
			else
			{
				// テクスチャ頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);

				// テクスチャ頂点情報の設定
				pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);
			}

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nAllPoint,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < m_nDepth; nCntA++)
	{
		for (int nCnt = 0; nCnt < m_nWidthPoint + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == m_nWidthPoint && nCntA != m_nDepth - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == m_nDepth - 1 && nCnt == m_nWidthPoint)
			{// 終了時に無視する
				break;
			}
			else
			{// 通常配置
				pIdx[0] = m_nWidthPoint + (m_nWidthPoint * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - m_nWidthPoint;
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CMeshfield::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < TEXTUREINFO_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 法線開放
	if (m_pNor != NULL)
	{
		delete[] m_pNor;
		m_pNor = NULL;
	}

	// UIの破棄
	UninitUI();
}

//==============================================================================
// 更新処理
//==============================================================================
void CMeshfield::Update(void)
{
	// 波形の挙動
	MeshWave();

	// 法線の計算
	CalcuNormal();

	// メッシュ管理の操作
	ControlMesh();

	// テクスチャの取得
	CTexture *pTexture = CManager::GetTexture();

	m_pTexture[TEXTUREINFO_0] = pTexture->GetAddress(m_aTextureIdx[TEXTUREINFO_0]);
	m_pTexture[TEXTUREINFO_1] = pTexture->GetAddress(m_aTextureIdx[TEXTUREINFO_1]);

	// UIの更新
	UpdateUI();
}

//==============================================================================
// 描画処理
//==============================================================================
void CMeshfield::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bWireFrame == true)
	{
		// ワイヤーフレーム有効
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//// 向きの反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D_1));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D_1);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[TEXTUREINFO_0]);
	pDevice->SetTexture(1, m_pTexture[TEXTUREINFO_1]);

	// テクスチャの合成方法
	switch (m_syntheticType)
	{
		case SYNTHETICTYPE_NONE:	// 合成なし
			
			// 一枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			// 二枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			
			break;

		case SYNTHETICTYPE_ADD:	// 加算合成

			// 一枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			// 二枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

			break;

		case SYNTHETICTYPE_SUBTRA:	// 減算合成

			// 一枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			
			// 二枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

			break;

		case SYNTHETICTYPE_MULTI:	// 乗算合成

			// 一枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			// 二枚目のテクスチャのテクスチャステージ設定
			pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

			break;
	}

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nIdxPoint,			// 頂点数
		0,
		m_nPolygon);			// プリミティブ数

	pDevice->SetTexture(1, NULL);
	pDevice->SetTexture(0, NULL);

	// 通常のアルファブレンディング
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

//==============================================================================
// フィールドの生成
//==============================================================================
CMeshfield *CMeshfield::Create(void)
{
	// ローカル変数宣言
	CMeshfield *pMeshField;
	pMeshField = new CMeshfield;

	if (pMeshField != NULL)
	{
		// 初期化
		pMeshField->Init();
	}

	return pMeshField;
}

//==============================================================================
// 法線計算
//==============================================================================
void CMeshfield::CalcuNormal(void)
{
	// ローカル変数宣言
	VERTEX_3D_1 *pVtx;
	D3DXVECTOR3 vecA = VECTOR3_NULL;
	D3DXVECTOR3 vecB = VECTOR3_NULL;

	int nCntNormal = 0;	// 法線カウント

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 各面の法線算出
	for (int nCntDepth = 0; nCntDepth < m_nDepth; nCntDepth++)
	{// 奥行面数
		int nCntOdd = 0;	// 奇数カウント
		int nCntEven = 0;	// 偶数カウント

		for (int nCntWidth = 0; nCntWidth < m_nWidth * 2; nCntWidth++)
		{// 横幅頂点数（三角形ポリゴン数）						
			if (nCntWidth % 2 == 0)
			{
				// 偶数時ベクトル
				vecA =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) - m_nWidthPoint + nCntEven].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntEven].pos;

				vecB =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntEven + 1].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntEven].pos;

				nCntEven++;		// 偶数カウント
			}
			else
			{
				// 奇数時ベクトル
				vecA =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth + m_nWidthPoint].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth].pos;

				vecB =
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth - 1].pos -
					pVtx[m_nWidthPoint + (m_nWidthPoint * nCntDepth) + nCntOdd - m_nWidth].pos;

				nCntOdd++;		// 奇数カウント
			}

			// 法線ベクトルの計算
			D3DXVec3Cross(&m_pNor[nCntNormal], &vecA, &vecB);

			// 法線ベクトルの正規化
			D3DXVec3Normalize(&m_pNor[nCntNormal], &m_pNor[nCntNormal]);

			// 現在のカウント
			nCntNormal++;
		}
	}

	// ローカル変数宣言
	int nCntUnder = m_nWidth * 2 * (m_nDepth - 1);	// 下辺開始面法線インデックス
	int nCntLeft = m_nWidth * 2;					// 左辺開始面法線インデックス
	int nCntRight = m_nWidth * 2 - 1;				// 右辺開始面法線インデックス
	int nRight = 1;									// 右辺開始地点インデックス計算用

	int nCntCenterA = 0;							// 内側頂点計算用上側
	int nCntCenterB
		= nCntCenterA + (m_nWidthPoint * 2 - 1);	// 内側頂点計算用下側
	int nCntCenterC = 1;							// 内側頂点計算用折り返しカウント

													// 各頂点の法線算出
	for (int nCntNor = 0; nCntNor < m_nIdxPoint; nCntNor++)
	{
		// 2面法線
		if (nCntNor == 0)
		{// 最初(左上)
			pVtx[nCntNor].nor = m_pNor[0] + m_pNor[1];
		}
		else if (nCntNor == m_nIdxPoint - 1)
		{// 最後(右下)
			pVtx[nCntNor].nor = m_pNor[m_nNor - 1] + m_pNor[m_nNor - 2];
		}

		// 1面法線
		else if (nCntNor == m_nWidth)
		{// 右上
			pVtx[nCntNor].nor = m_pNor[m_nWidth * 2 - 1];
		}
		else if (nCntNor == (m_nWidth + 1) + ((m_nDepth - 1) * (m_nWidth + 1)))
		{// 左下
			pVtx[nCntNor].nor = m_pNor[m_nWidth * 2 * (m_nDepth - 1)];
		}

		// 3面法線
		else if (nCntNor > 0 && nCntNor < m_nWidth)
		{// 上辺中央部
		 // ローカル変数宣言
			int nCntA = nCntNor + (nCntNor - 1);	// 始発となる面インデックス

			pVtx[nCntNor].nor = m_pNor[nCntA] + m_pNor[nCntA + 1] + m_pNor[nCntA + 2];
		}
		else if (nCntNor >(m_nWidth + 1) + ((m_nDepth - 1) * (m_nWidth + 1)) && nCntNor < m_nIdxPoint - 1)
		{// 下辺中央部
			pVtx[nCntNor].nor = m_pNor[nCntUnder] + m_pNor[nCntUnder + 1] + m_pNor[nCntUnder + 2];
			nCntUnder += 2;
		}
		else if (nCntNor % m_nWidthPoint == 0 && nCntNor != 0)
		{// 左辺中央部
			pVtx[nCntNor].nor = m_pNor[nCntLeft] + m_pNor[nCntLeft + 1] + m_pNor[nCntLeft - (m_nWidth * 2)];
			nCntLeft += m_nWidth * 2;
		}
		else if (nCntNor % (m_nWidth + m_nWidthPoint * nRight) == 0 && nCntNor != 0)
		{// 右辺中央部
			pVtx[nCntNor].nor = m_pNor[nCntRight] + m_pNor[nCntRight - 1] + m_pNor[nCntRight + (m_nWidth * 2)];
			nCntRight += m_nWidth * 2;
			nRight++;
		}

		// 例外的頂点(6面法線)
		else
		{// 内側の頂点
			pVtx[nCntNor].nor =
				m_pNor[nCntCenterA] + m_pNor[nCntCenterA + 1] + m_pNor[nCntCenterA + 2] +
				m_pNor[nCntCenterB] + m_pNor[nCntCenterB + 1] + m_pNor[nCntCenterB + 2];

			if (nCntNor % ((m_nWidth - 1) + m_nWidthPoint * nCntCenterC) == 0)
			{// 折り返し地点
				nCntCenterA += 4;
				nCntCenterB += 4;
				nCntCenterC++;
			}
			else
			{// 例外
				nCntCenterA += 2;
				nCntCenterB += 2;
			}
		}

		// ベクトルの正規化
		D3DXVec3Normalize(&pVtx[nCntNor].nor, &pVtx[nCntNor].nor);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================================
// メッシュの波形
//==============================================================================
void CMeshfield::MeshWave(void)
{
	// 波形カウント
	m_nCntWave++;

	// ローカル変数宣言
	VERTEX_3D_1 *pVtx;

	m_aTexMove[TEXTUREINFO_0] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_0]) * m_aTexMoveSpeed[TEXTUREINFO_0], cosf(m_aTexMoveRot[TEXTUREINFO_0]) * m_aTexMoveSpeed[TEXTUREINFO_0]);
	m_aTexMove[TEXTUREINFO_1] = D3DXVECTOR2(sinf(m_aTexMoveRot[TEXTUREINFO_1]) * m_aTexMoveSpeed[TEXTUREINFO_1], cosf(m_aTexMoveRot[TEXTUREINFO_1]) * m_aTexMoveSpeed[TEXTUREINFO_1]);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDepth = 0; nCntDepth < m_nDepthPoint; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nWidthPoint; nCntWidth++)
		{
			// 波形のタイプ
			switch (m_waveType)
			{
			case WAVETYPE_NONE:
				pVtx[0].pos.y = m_pos.y;
				break;

			case WAVETYPE_WAVE_X:

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.x * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;
				
				break;

			case WAVETYPE_WAVE_Z:

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.z * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;

				break;

			case WAVETYPE_WAVE_XZ:

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.x * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;

				pVtx[0].pos.y +=
					sinf(pVtx[0].pos.z * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;

				break;

			case WAVETYPE_CIRCLE:
				// 方向
				D3DXVECTOR3 vec = pVtx[0].pos - m_pos;

				// 距離
				float fDistance = sqrtf(vec.x * vec.x + vec.z * vec.z);

				pVtx[0].pos.y += sinf(fDistance * m_fDistanceWave + m_nCntWave * m_fSpeedWave) * m_fHeightWave;
				break;
			}

			switch (m_texMove)
			{
			case TEXMOVE_STRAIGHT:
				// テクスチャの移動
				pVtx[0].tex += m_aTexMove[TEXTUREINFO_0];
				pVtx[0].tex1 += m_aTexMove[TEXTUREINFO_1];
				break;

			case TEXMOVE_WAVE:
				break;
			}


			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================================
// メッシュの操作
//==============================================================================
void CMeshfield::ControlMesh(void)
{	
	// ローカル変数宣言
	VERTEX_3D_1 *pVtx;
	int nIdx = 0;

	// テキストのデータ
	CTextData *pData;
	pData = CManager::GetTextData();

	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// 波の発生
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_FLUG) == true)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDepth = 0; nCntDepth < m_nDepthPoint; nCntDepth++)
		{
			for (int nCntWidth = 0; nCntWidth < m_nWidthPoint; nCntWidth++)
			{
				// 一度座標を戻す
				pVtx[nIdx].pos.y = m_pos.y;
				nIdx++;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		// 項目の指定
		if (m_waveType == WAVETYPE_MAX - 1)
		{
			m_waveType = (WAVETYPE)0;
		}
		else
		{
			m_waveType = (WAVETYPE)(m_waveType + 1);	
		}
	}

	// 合成方法の切り替え
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_SYNTHETIC) == true)
	{
		// 項目の指定
		if (m_syntheticType == SYNTHETICTYPE_MAX - 1)
		{
			m_syntheticType = (SYNTHETICTYPE)0;
		}
		else
		{
			m_syntheticType = (SYNTHETICTYPE)(m_syntheticType + 1);
		}
	}

	// 分割の切り替え
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_CUTTEX) == true)
	{
		m_bCutTex = !m_bCutTex;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nDepthPoint; nCnt++)
		{// 奥行軸
			for (int nCntA = 0; nCntA < m_nWidthPoint; nCntA++)
			{// 横軸

				if (m_bCutTex == true)
				{// 分割あり
				 // テクスチャ頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

					// テクスチャ頂点情報の設定
					pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);
				}
				else
				{
					// テクスチャ頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);

					// テクスチャ頂点情報の設定
					pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntA / m_nWidth, 1.0f * nCnt / m_nDepth);
				}

				pVtx++;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	// データの書き出し
	if (keyboard->GetTrigger(CInput::KEYINFO_SAVE) == true)
	{
		pData->SaveText();
	}

	// 操作するテクスチャの切り替え
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_TEX_CHANGE) == true)
	{
		if (m_texInfo == TEXTUREINFO_MAX - 1)
		{// 折り返し
			m_texInfo = (TEXTUREINFO)0;
		}
		else
		{// 次項
			m_texInfo = (TEXTUREINFO)(m_texInfo + 1);
		}
	}

	// テクスチャのインデックス操作
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_TEXTURE_IDX_PLUS) == true)
	{
		if (m_aTextureIdx[m_texInfo] == m_nTextureNum - 1)
		{
			m_aTextureIdx[m_texInfo] = 0;
		}
		else
		{
			m_aTextureIdx[m_texInfo]++;
		}	
	}
	if (keyboard->GetTrigger(CInput::KEYINFO_WAVE_TEXTURE_IDX_MINUS) == true)
	{
		if (m_aTextureIdx[m_texInfo] == 0)
		{
			m_aTextureIdx[m_texInfo] = m_nTextureNum - 1;
		}
		else
		{
			m_aTextureIdx[m_texInfo]--;
		}
	}

	// ワイヤーフレームの切り替え
	if (keyboard->GetTrigger(CInput::KEYINFO_WIREFRAME) == true)
	{
		m_bWireFrame = !m_bWireFrame;
	}

	// 波の高さの調整
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_HEIGHTUP) == true)
	{
		m_fHeightWave += WAVE_HEIGHT_ADJUST;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_HEIGHTDOWN) == true)
	{
		m_fHeightWave -= WAVE_HEIGHT_ADJUST;
	}

	// 波形間隔の調整
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_DISTANCEUP) == true)
	{
		m_fDistanceWave += WAVE_DISTANCE_ADJUST;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_DISTANCEDOWN) == true)
	{
		m_fDistanceWave -= WAVE_DISTANCE_ADJUST;
	}

	// 速さの調整
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_SPEEDUP) == true)
	{
		m_fSpeedWave += WAVE_SPEED_ADJUST;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_SPEEDDOWN) == true)
	{
		m_fSpeedWave -= WAVE_SPEED_ADJUST;
	}

	// テクスチャの流れる向き
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVEROT_PLUS) == true)
	{// 加算
		m_aTexMoveRot[m_texInfo] += 0.01f;

		// 角度の補正
		if (m_aTexMoveRot[m_texInfo] > D3DX_PI)
		{
			m_aTexMoveRot[m_texInfo] -= PI_RESET;
		}
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVEROT_MINUS) == true)
	{// 減算
		m_aTexMoveRot[m_texInfo] -= 0.01f;

		// 角度の補正
		if (m_aTexMoveRot[m_texInfo] < 0.0f)
		{
			m_aTexMoveRot[m_texInfo] += PI_RESET;
		}
	}

	// テクスチャの流れる速さ
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVE_PLUS) == true)
	{// 加算
		m_aTexMoveSpeed[m_texInfo] += 0.001f;
	}
	if (keyboard->GetPress(CInput::KEYINFO_WAVE_TEXMOVE_MINUS) == true)
	{// 減算
		m_aTexMoveSpeed[m_texInfo] -= 0.001f;
	}

	// 波形の高さ下限
	if (m_fHeightWave < 1.0f)
	{
		m_fHeightWave = 1.0f;
	}
}

// 2DポリゴンUIの生成
void CMeshfield::CreateUI(void)
{
	for (int nCntPoly = 0; nCntPoly < TEXTUREINFO_MAX; nCntPoly++)
	{
		if (m_apScene2D[nCntPoly] == NULL)
		{
			m_apScene2D[nCntPoly] = CScene2D::Create();
			m_apScene2D[nCntPoly]->SetSize(TEXTURE_UI_SIZE);
			m_apScene2D[nCntPoly]->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
			m_apScene2D[nCntPoly]->BindTexture(m_aTextureIdx[nCntPoly]);
		}
	}

	m_apScene2D[TEXTUREINFO_0]->SetPosition(TEXTURE_UI_POS_0);
	m_apScene2D[TEXTUREINFO_1]->SetPosition(TEXTURE_UI_POS_1);
}

// 2DポリゴンUIの破棄
void CMeshfield::UninitUI(void)
{
	for (int nCntPoly = 0; nCntPoly < TEXTUREINFO_MAX; nCntPoly++)
	{
		if (m_apScene2D[nCntPoly] != NULL)
		{// ポリゴンの破棄
			m_apScene2D[nCntPoly]->Uninit();
			m_apScene2D[nCntPoly] = NULL;
		}
	}
}

// 2DポリゴンUIの更新
void CMeshfield::UpdateUI(void)
{
	for (int nCntPoly = 0; nCntPoly < TEXTUREINFO_MAX; nCntPoly++)
	{
		if (m_apScene2D[nCntPoly] != NULL)
		{// テクスチャの変化
			m_apScene2D[nCntPoly]->BindTexture(m_aTextureIdx[nCntPoly]);
		}

		if (nCntPoly == m_texInfo)
		{
			m_apScene2D[nCntPoly]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			m_apScene2D[nCntPoly]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
}