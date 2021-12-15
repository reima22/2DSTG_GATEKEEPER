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
#include "chara3D.h"
#include "texture.h"
//#include "model.h"

// 静的メンバ変数宣言
char *CChara3D::m_pFileName[CHARA3DTYPE_MAX] =
{
	{ "data/MODEL/meteor.x" },		// 隕石
	{ "data/MODEL/debri00.x" },		// 宇宙ゴミ0
};

//==============================================================================
// コンストラクタ
//==============================================================================
CChara3D::CChara3D(int nPriority) : CScene3D(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CChara3D::~CChara3D()
{

}

//==============================================================================
// 3Dモデル描画の初期化処理
//==============================================================================
HRESULT CChara3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャポインタの取得
	CTexture *pTexture;
	pTexture = CManager::GetTexture();

	// 初期設定
	m_pos = pos;
	m_move = move;
	//m_rotMove = rotMove;
	m_type = (CHARA3DTYPE)nType;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		m_pFileName[m_type],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// テクスチャポインタ配列の動的確保(マテリアルの数だけ)
	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// バッファポインタの取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャあり
		 // テクスチャ情報の割り当て
			m_pTexture[nCntMat] = pTexture->GetAddress3D(pMat[nCntMat].pTextureFilename);
		}
		else
		{// テクスチャの割り当てがない
			m_pTexture[nCntMat] = NULL;
		}
	}

	return S_OK;
}

//==============================================================================
// 3Dモデル描画の終了処理
//==============================================================================
void CChara3D::Uninit(void)
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

	Release();
}

//==============================================================================
// 3Dモデル描画の更新処理
//==============================================================================
void CChara3D::Update(void)
{
	// 挙動計算
	m_pos += m_move;
	m_rot += m_rotMove;

	// 回転角度補正
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= PI_RESET;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += PI_RESET;
	}

	if (m_rot.x >= D3DX_PI)
	{
		m_rot.x -= PI_RESET;
	}
	else if (m_rot.x <= -D3DX_PI)
	{
		m_rot.x += PI_RESET;
	}

	CScene3D::SetPos(m_pos);
}

//==============================================================================
// 3Dモデル描画の描画処理
//==============================================================================
void CChara3D::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス
	D3DMATERIAL9 matDef;												// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;													// マテリアルデータへのポインタ

	// Zテストの更新
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		if (m_pTexture[nCntMat] != NULL)
		{
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================================
// 生成処理
//==============================================================================
CChara3D *CChara3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	// ローカル変数宣言
	CChara3D *pChara3D;
	pChara3D = new CChara3D;	// インスタンス生成

	// 初期化
	if (pChara3D != NULL)
	{
		pChara3D->Init(pos, move, nType);
	}

	return pChara3D;
}