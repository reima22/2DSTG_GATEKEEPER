//==============================================================================
//
// 3DモデルX描画処理〔sceneX.cpp〕
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

//==============================================================================
// コンストラクタ
//==============================================================================
CSceneX::CSceneX()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CSceneX::~CSceneX()
{

}

//==============================================================================
// 3Dモデル描画の初期化処理
//==============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャポインタの取得
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	m_pScene3D = CScene3D::Create();

	// 初期設定
	m_pScene3D->SetPos(pos);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/shadowS.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// テクスチャポインタ配列の動的確保(マテリアルの数だけ)
	//m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	//D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	//// バッファポインタの取得
	//pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	//{
	//	if (pMat[nCntMat].pTextureFilename != NULL)
	//	{// テクスチャあり
	//		// テクスチャ情報の割り当て
	//		m_pTexture[nCntMat] = pTexture->GetAddress3D(pMat[nCntMat].pTextureFilename);

	//		// ファイル名を使用してテクスチャを読み込む
	//		D3DXCreateTextureFromFile(
	//			pDevice,
	//			pMat[nCntMat].pTextureFilename,
	//			&m_pTexture[nCntMat]);
	//	}
	//	else
	//	{// テクスチャの割り当てがない
	//		m_pTexture[nCntMat] = NULL;
	//	}
	//}

	return S_OK;
}

//==============================================================================
// 3Dモデル描画の終了処理
//==============================================================================
void CSceneX::Uninit(void)
{
	// メッシュポインタの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルバッファの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	CScene3D::Uninit();
}

//==============================================================================
// 3Dモデル描画の更新処理
//==============================================================================
void CSceneX::Update(void)
{
	D3DXVECTOR3 pos = CScene3D::GetPosition();

	m_pScene3D->SetPosition(pos);
}

//==============================================================================
// 3Dモデル描画の描画処理
//==============================================================================
void CSceneX::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice =CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス
	D3DMATERIAL9 matDef;												// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;													// マテリアルデータへのポインタ

	// 位置と向き
	D3DXVECTOR3 pos = m_pScene3D->GetPosition();
	D3DXVECTOR3 rot = m_pScene3D->GetRot();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		//pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// 生成処理
//==============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	CSceneX *pSceneX;
	pSceneX = new CSceneX;	// シーンXのインスタンス

	// 初期化
	if (pSceneX != NULL)
	{
		pSceneX->Init(pos);
	}

	return pSceneX;
}

// 位置の反映