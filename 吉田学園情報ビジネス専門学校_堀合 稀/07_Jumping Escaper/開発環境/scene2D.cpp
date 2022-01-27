//==============================================================================
//
// 2Dポリゴン描画処理〔scene2D.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "fade.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR2_NULL;
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_bZbuff = false;
	m_bAlphaBlend = false;
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
HRESULT CScene2D::Init(void)
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

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

	// シーンクラスへの情報
	SetPosition(m_pos);
	SetRotation(m_rot);
}

//==============================================================================
// ポリゴン描画の描画処理
//==============================================================================
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	if (m_bZbuff == true)
	{
		// Zテストの更新
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	if (m_bAlphaBlend == true)
	{
		// 加算合成によるアルファブレンディング
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

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

	if (m_bZbuff == true)
	{
		// 元に戻す
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	if (m_bAlphaBlend == true)
	{
		// 通常のアルファブレンディング
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

//==============================================================================
// 2Dの生成
//==============================================================================
CScene2D *CScene2D::Create(void)
{
	// ローカル変数宣言
	CScene2D *pScene2D;
	pScene2D = new CScene2D;

	// 初期化
	if (pScene2D != NULL)
	{
		pScene2D->Init();
	}

	return pScene2D;
}

//==============================================================================
// テクスチャの割り当て
//==============================================================================
void CScene2D::BindTexture(char *pTypeCheck)
{
	// テクスチャポインタの取得
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	m_pTexture = pTexture->GetAddress(pTypeCheck);
}

//==============================================================================
// テクスチャの割り当て(数値)
//==============================================================================
void CScene2D::BindTextureNum(int nTexNum)
{
	// テクスチャポインタの取得
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	m_pTexture = pTexture->GetAddressNum(nTexNum);
}

//==============================================================================
// ポリゴンのサイズ設定
//==============================================================================
void CScene2D::SetSize(D3DXVECTOR2 size)
{
	m_size = size;

	// 対角線の設定
	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);
	m_fAngle = atan2f(m_size.x, m_size.y);
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
// テクスチャ座標の設定
//==============================================================================
void CScene2D::SetTexUV(D3DXVECTOR2 Vertex_0, D3DXVECTOR2 Vertex_1, D3DXVECTOR2 Vertex_2, D3DXVECTOR2 Vertex_3)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをロックしてポインタ取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標設定
		pVtx[0].tex = Vertex_0;
		pVtx[1].tex = Vertex_1;
		pVtx[2].tex = Vertex_2;
		pVtx[3].tex = Vertex_3;

		// 頂点バッファアンロック
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