//==============================================================================
//
// 影の描画〔shadow.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "shadow.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "textdata_texture.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CShadow::CShadow(int nPriority) : CScene(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CShadow::~CShadow()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CShadow::Init(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み取り
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	//m_pTexture = pTexture->GetAddress(pTexture->GetData()->GetIdxShadow());

	//// テクスチャの読み込み
	//D3DXCreateTextureFromFile(
	//	pDevice,
	//	"data/TEXTURE/shadow000.jpg",
	//	&m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, m_pos.z - m_size.y);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, m_pos.z + m_size.y);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, m_pos.z - m_size.y);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, m_pos.z + m_size.y);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
	pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
	pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CShadow::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void CShadow::Update(void)
{
}

//==============================================================================
// 描画処理
//==============================================================================
void CShadow::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = NULL;	// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	
	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	if (pDevice != NULL)
	{
		// 減算合成の生成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			0,						// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数

		//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 通常の合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//==============================================================================
// 影の生成
//==============================================================================
CShadow *CShadow::Create(D3DXVECTOR2 size)
{
	// ローカル変数宣言
	CShadow *pShadow;
	pShadow = new CShadow;

	if (pShadow != NULL)
	{
		pShadow->SetSize(size);
		pShadow->Init();
	}

	return pShadow;
}