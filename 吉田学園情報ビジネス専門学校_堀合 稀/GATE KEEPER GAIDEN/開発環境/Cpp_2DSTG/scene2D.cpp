//==============================================================================
//
// 2Dポリゴン描画処理〔scene2D.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "texture.h"
#include "fade.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

//==============================================================================
// デストラクタ
//==============================================================================
CScene2D::~CScene2D()
{

}

//==============================================================================
// ポリゴン描画の初期化処理
//==============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
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

	// 変数初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーの中心座標
	m_pos = pos;

	// プレイヤーの各頂点座標
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

	// テクスチャ頂点情報の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// ポリゴン描画の終了処理
//==============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//==============================================================================
// ポリゴン描画の更新処理
//==============================================================================
void CScene2D::Update(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点情報の更新
	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをロックする
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ポリゴンの頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(
			m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength,
			m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength,
			0.0f);
		pVtx[1].pos = D3DXVECTOR3(
			m_pos.x + sinf(m_fAngle - D3DX_PI + m_rot.z) * m_fLength,
			m_pos.y + cosf(m_fAngle - D3DX_PI + m_rot.z) * m_fLength,
			0.0f);
		pVtx[2].pos = D3DXVECTOR3(
			m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength,
			m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength,
			0.0f);
		pVtx[3].pos = D3DXVECTOR3(
			m_pos.x + sinf(-m_fAngle + D3DX_PI + m_rot.z) * m_fLength,
			m_pos.y + cosf(-m_fAngle + D3DX_PI + m_rot.z) * m_fLength,
			0.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//==============================================================================
// ポリゴン描画の描画処理
//==============================================================================
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//==============================================================================
// 位置情報の設定
//==============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	if (m_pos != NULL)
	{
		m_pos = pos;
	}
}

////==============================================================================
//// 位置情報の取得
////==============================================================================
//D3DXVECTOR3 CScene2D::GetPosition(void)
//{
//	return m_pos;
//}

//==============================================================================
// テクスチャの割り当て
//==============================================================================
void CScene2D::BindTexture(CTexture::TEXTYPE texType)
{
	m_pTexture = CTexture::BindTexture(texType);
}

//==============================================================================
// ポリゴンのサイズ設定
//==============================================================================
void CScene2D::SetSize(float fSizeX, float fSizeY)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	// 対角線の設定
	m_fLength = sqrtf(m_fSizeX * m_fSizeX + m_fSizeY * m_fSizeY);
	m_fAngle = atan2f(m_fSizeX, m_fSizeY);
}

//==============================================================================
// テクスチャ座標の設定
//==============================================================================
void CScene2D::SetTex(int nCutX, int nCutY, int nPatternX, int nPatternY, float fTexX, float fTexY)
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
// 色情報の設定
//==============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
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