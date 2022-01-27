//==============================================================================
//
// ポリゴン描画処理〔polygon.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "polygon.h"
#include "renderer.h"
#include "texture.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CPolygon::CPolygon()
{
	
}

//==============================================================================
// デストラクタ
//==============================================================================
CPolygon::~CPolygon()
{
}

//==============================================================================
// ポリゴン描画の初期化処理
//==============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報を設定
	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	m_fLength = sqrtf(fSizeX * fSizeX + fSizeY * fSizeY);
	m_fAngle = atan2f(fSizeX, fSizeY);
	m_pos = pos;
	m_nType = nType;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 数値の各頂点座標
	pVtx[0].pos = D3DXVECTOR3(
		m_pos.x + sinf(-m_fAngle) * m_fLength,
		m_pos.y + cosf(-m_fAngle) * m_fLength,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(
		m_pos.x + sinf(m_fAngle - D3DX_PI) * m_fLength,
		m_pos.y + cosf(m_fAngle - D3DX_PI) * m_fLength,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(
		m_pos.x + sinf(m_fAngle) * m_fLength,
		m_pos.y + cosf(m_fAngle) * m_fLength,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(
		m_pos.x + sinf(-m_fAngle + D3DX_PI) * m_fLength,
		m_pos.y + cosf(-m_fAngle + D3DX_PI) * m_fLength,
		0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// ポリゴン描画の終了処理
//==============================================================================
void CPolygon::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//==============================================================================
// ポリゴン描画の更新処理
//==============================================================================
void CPolygon::Update(void)
{
	/*任意で処理*/
}

//==============================================================================
// ポリゴン描画の描画処理
//==============================================================================
void CPolygon::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャポインタの取得
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (m_pTypeCheck != NULL)
	{
		pDevice->SetTexture(0, pTexture->GetAddress(m_pTypeCheck));
	}
	else
	{
		pDevice->SetTexture(0, NULL);
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//==============================================================================
// 生成処理
//==============================================================================
CPolygon *CPolygon::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// インスタンス生成
	CPolygon *pPolygon;
	pPolygon = new CPolygon;

	// 初期化
	pPolygon->Init(pos, fSizeX, fSizeY,nType);

	return pPolygon;
}

//==============================================================================
// 位置の設定
//==============================================================================
void CPolygon::SetPosition(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	m_pos = pos;

	// 頂点情報の更新
	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをロックする
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 数値の各頂点座標
		pVtx[0].pos = D3DXVECTOR3(
			m_pos.x + sinf(-m_fAngle) * m_fLength,
			m_pos.y + cosf(-m_fAngle) * m_fLength,
			0.0f);
		pVtx[1].pos = D3DXVECTOR3(
			m_pos.x + sinf(m_fAngle - D3DX_PI) * m_fLength,
			m_pos.y + cosf(m_fAngle - D3DX_PI) * m_fLength,
			0.0f);
		pVtx[2].pos = D3DXVECTOR3(
			m_pos.x + sinf(m_fAngle) * m_fLength,
			m_pos.y + cosf(m_fAngle) * m_fLength,
			0.0f);
		pVtx[3].pos = D3DXVECTOR3(
			m_pos.x + sinf(-m_fAngle + D3DX_PI) * m_fLength,
			m_pos.y + cosf(-m_fAngle + D3DX_PI) * m_fLength,
			0.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//==============================================================================
// テクスチャ座標の設定
//==============================================================================
void CPolygon::SetTex(int nCutX, int nCutY, int nPatternX, int nPatternY, float fTexX, float fTexY)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点情報の更新
	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをロックする
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(
			nPatternX * (1.0f / nCutX) + fTexX,
			nPatternY * (1.0f / nCutY) + (1.0f / nCutY) + fTexY);

		pVtx[1].tex = D3DXVECTOR2(
			nPatternX * (1.0f / nCutX) + fTexX,
			nPatternY * (1.0f / nCutY) + fTexY);

		pVtx[2].tex = D3DXVECTOR2(
			nPatternX * (1.0f / nCutX) + (1.0f / nCutX) + fTexX,
			nPatternY * (1.0f / nCutY) + (1.0f / nCutY) + fTexY);

		pVtx[3].tex = D3DXVECTOR2(
			nPatternX * (1.0f / nCutX) + (1.0f / nCutX) + fTexX,
			nPatternY * (1.0f / nCutY) + fTexY);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//==============================================================================
// テクスチャ座標の設定
//==============================================================================
void CPolygon::SetCol(D3DXCOLOR col)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点情報の更新
	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをロックする
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ頂点情報の設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}