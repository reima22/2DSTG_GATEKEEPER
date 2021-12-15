//==============================================================================
//
// 爆発描画処理〔explosion.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "explosion.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CExplosion::CExplosion(int nPriority) : CScene2D(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CExplosion::~CExplosion()
{
	
}

//==============================================================================
// 爆発の初期化処理
//==============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY)
{
	// 初期化設定
	CScene2D::Init(pos);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetObjType(objType);

	return S_OK;
}

//==============================================================================
// 爆発の終了処理
//==============================================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 爆発の更新処理
//==============================================================================
void CExplosion::Update(void)
{
	// カウントアップ
	m_nCounterAnim++;

	// アニメーション更新
	if (m_nCounterAnim % 4 == 0)
	{
		// パターン加算
		m_nPatternAnim++;

		// テクスチャ設定
		CScene2D::SetTex(EXPLOSION_TEX_X, EXPLOSION_TEX_Y, m_nPatternAnim, 0, 0.0f, 0.0f);

		// 爆発の消滅
		if (m_nPatternAnim >= EXPLOSION_TEX_X)
		{
			Uninit();
		}
	}

	// 更新処理
	CScene2D::Update();
}

//==============================================================================
// 爆発の描画処理
//==============================================================================
void CExplosion::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//==============================================================================
// 爆発の生成処理
//==============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY)
{
	// ローカル変数宣言
	CExplosion *pExplosion;
	pExplosion = new CExplosion;

	// 初期化
	if (pExplosion != NULL)
	{
		pExplosion->Init(pos, objType, fSizeX, fSizeY);
	}

	// テクスチャ割り当て
	pExplosion->BindTexture(CTexture::TEXTYPE_EXPLOSION);
	
	return pExplosion;
}