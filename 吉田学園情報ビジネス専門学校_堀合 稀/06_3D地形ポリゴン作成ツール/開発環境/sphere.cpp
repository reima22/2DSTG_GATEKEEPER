//==============================================================================
//
// メッシュフィールドの描画〔meshfield.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
//#include "meshfield.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "textdata.h"
//#include "textdata_meshfield.h"
#include "texture.h"
#include "sphere.h"
#include "exe.h"
#include "collision.h"
#include "camera.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CSphere::CSphere()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CSphere::~CSphere()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CSphere::Init(void)
{
	// ローカル変数宣言
	//VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();


	m_fRadius = 5.0f;
	m_nSlice = 10;
	m_nStack = 10;

	// 球体の生成
	D3DXCreateSphere(
		pDevice,
		m_fRadius,
		m_nSlice,
		m_nStack,
		&m_pMeshSphere,
		&m_pBuffMatSphere);

	m_pos.y += m_fRadius / 2.0f;

	SetLanding(true);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CSphere::Uninit(void)
{

}

//==============================================================================
// 更新処理
//==============================================================================
void CSphere::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの方向
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();

	// 他オブジェクトとの当たり判定
	CCollision *pCollision;
	pCollision = CManager::GetCollision(); // 当たり判定取得

	if (pCollision != NULL)
	{
		if (pCollision->CollisionMesh(this, CExe::GetMeshField()) == true)
		{
			m_bLand = true;
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CSphere::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRotAttack, mtxTransAttack;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATRIX mtxParent;						// 親のマトリックス
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	//D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

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

	// 各パーツの位置を反映
	D3DXMatrixTranslation(
		&mtxTransAttack,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxTransAttack);

	// 判定のワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMatSphere->GetBufferPointer();

	pMat[0].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// マテリアルの設定
	pDevice->SetMaterial(&pMat[0].MatD3D);

	// 球体の描画
	m_pMeshSphere->DrawSubset(0);

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// フィールドの生成
//==============================================================================
CSphere *CSphere::Create(void)
{
	// ローカル変数宣言
	CSphere *pSphere;
	pSphere = new CSphere;

	if (pSphere != NULL)
	{
		// 初期化
		pSphere->Init();
	}

	return pSphere;
}