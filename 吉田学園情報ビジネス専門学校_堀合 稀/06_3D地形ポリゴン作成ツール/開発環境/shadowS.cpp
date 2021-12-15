//==============================================================================
//
// 影モデル描画処理〔shadowS.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "sceneX.h"
#include "texture.h"
#include "model.h"
#include "shadowS.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CShadowS::CShadowS()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CShadowS::~CShadowS()
{

}

//==============================================================================
// 3Dモデル描画の初期化処理
//==============================================================================
HRESULT CShadowS::Init(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// モデルの生成
	m_pSceneX = CSceneX::Create(pos);

	// 位置の設定
	SetPosition(pos);

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

	// ポリゴンの配置
	m_posPolygon = SCREEN_CENTER;

	// ポリゴンの大きさ
	m_sizePolygon = D3DXVECTOR2(640.0f, 360.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 数値の各頂点座標
	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - m_sizePolygon.x, m_posPolygon.y + m_sizePolygon.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - m_sizePolygon.x, m_posPolygon.y - m_sizePolygon.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + m_sizePolygon.x, m_posPolygon.y + m_sizePolygon.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + m_sizePolygon.x, m_posPolygon.y - m_sizePolygon.y, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定(黒)
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// 3Dモデル描画の終了処理
//==============================================================================
void CShadowS::Uninit(void)
{
	//// メッシュポインタの破棄
	//if (m_pMesh != NULL)
	//{
	//	m_pMesh->Release();
	//	m_pMesh = NULL;
	//}

	//// マテリアルバッファの破棄
	//if (m_pBuffMat != NULL)
	//{
	//	m_pBuffMat->Release();
	//	m_pBuffMat = NULL;
	//}

	CScene3D::Uninit();
}

//==============================================================================
// 3Dモデル描画の更新処理
//==============================================================================
void CShadowS::Update(void)
{

}

//==============================================================================
// 3Dモデル描画の描画処理
//==============================================================================
void CShadowS::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス

	// 位置と向き
	D3DXVECTOR3 pos = CScene3D::GetPosition();
	D3DXVECTOR3 rot = CScene3D::GetRot();
	D3DXMATRIX mtx = m_pSceneX->GetMatrix();


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);


	// ステンシルテストを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// Zバッファの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// カラーバッファへの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);


	// ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);

	// ステンシルテスト合格・Zテスト合格
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// ステンシルテスト合格・Zテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);

	// ステンシルテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCRSAT);

	// 表面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 影モデルの描画
	m_pSceneX->Draw();

	// ステンシルバッファの参照値を設定（"1"にする）
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ステンシルテスト合格・Zテスト合格
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);

	// ステンシルテスト合格・Zテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// ステンシルテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	// 裏面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 影モデルの描画
	m_pSceneX->Draw();

	// カラーバッファへの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// ステンシルバッファの参照値を設定("2"にする)
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ステンシルテスト合格・Zテスト合格
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// ステンシルテスト合格・Zテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// ステンシルテスト不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	// ポリゴンの表示
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数


	// 裏面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Zバッファの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ステンシルテストを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==============================================================================
// 生成処理
//==============================================================================
CShadowS *CShadowS::Create(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	CShadowS *pShadowS;
	pShadowS = new CShadowS;	// シーンXのインスタンス

	// 初期化
	if (pShadowS != NULL)
	{
		pShadowS->Init(pos);
	}

	return pShadowS;
}

//==============================================================================
// 影の追従
//==============================================================================
void CShadowS::SetPosition(D3DXVECTOR3 pos)
{
	CScene3D *pScene3D;
	pScene3D = m_pSceneX->GetScene3D();

	if (pScene3D != NULL)
	{
		pScene3D->SetPos(pos);
	}

}