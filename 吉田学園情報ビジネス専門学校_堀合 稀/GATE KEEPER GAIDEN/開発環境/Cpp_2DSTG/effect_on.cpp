//==============================================================================
//
// 描画優先低エフェクト描画処理〔effect.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "effect_on.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CEffectOn::CEffectOn(int nPriority) : CScene2D(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CEffectOn::~CEffectOn()
{

}

//==============================================================================
// エフェクトの初期化処理
//==============================================================================
HRESULT CEffectOn::Init(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern)
{
	// 初期化設定
	CScene2D::Init(pos);
	CScene2D::SetSize(fRadius, fRadius);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(OBJTYPE_EFFECT);
	CScene2D::SetCol(col);
	m_effectType = (EFFECT_TYPE)nPattern;

	// 大きさ
	m_fRadius = fRadius;

	// 透明度減少
	m_fAlphaDown = fAlphaDown;

	// 色情報
	m_col = col;

	// パーティクルの角度と動き
	float fAngle;

	// パターン分け
	if (m_effectType == 0)
	{
		// 全方位拡散
		fAngle = (float)((rand() % 628) - 314) / 100.0f;
	}
	else if (m_effectType == 1)
	{
		// 上方パーティクル
		fAngle = (float)((rand() % 115 + 200) / 100.0f);
		int nPlus = rand() % 2;
		if (nPlus == 0)
		{// 左右反転乱数
			fAngle *= -1;
		}
	}
	else if(m_effectType == 2)
	{
		fAngle = 0.0f;
		fMove = 0.0f;
	}
	else
	{
		int nLeft = rand() % 2;
		fAngle = rand() % 30 / 100.0f;
		if (nLeft == 0)
		{
			fAngle *= -1;
		}
	}

	// エフェクトの移動設定
	m_Move.x = (sinf(fAngle) * (float)(rand() % 500 + 1) * fMove);
	m_Move.y = (cosf(fAngle) * (float)(rand() % 500 + 1) * fMove);

	return S_OK;
}

//==============================================================================
// エフェクトの終了処理
//==============================================================================
void CEffectOn::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// エフェクトの更新処理
//==============================================================================
void CEffectOn::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 posEffect = CScene2D::GetPosition();

	// パーティクル移動処理
	posEffect += m_Move;

	// エフェクトの半径変化
	if (m_effectType != EFFECT_TYPE_WAVE)
	{
		m_fRadius -= 0.02f;						// 大きさの減少
	}
	else
	{
		m_fRadius += 3.5f;
	}

	CScene2D::SetSize(m_fRadius, m_fRadius);	// 変化を反映

												// エフェクトの色情報変化
	m_col.a -= m_fAlphaDown;	// 徐々に透明化
	CScene2D::SetCol(m_col);	// 変化を反映

								// 特定条件下で終了
	if (m_col.a < 0.0f)
	{
		Uninit();
		return;
	}

	// 位置情報の設定
	CScene2D::SetPosition(posEffect);

	CScene2D::Update();
}

//==============================================================================
// エフェクトの描画処理
//==============================================================================
void CEffectOn::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 加算合成によるアルファブレンディング
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	CScene2D::Draw();

	// 通常のアルファブレンディング
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================
// エフェクトの生成処理
//==============================================================================
CEffectOn *CEffectOn::Create(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern)
{
	// ローカル変数宣言
	CEffectOn *pEffectOn;
	pEffectOn = new CEffectOn;

	// 初期化
	if (pEffectOn != NULL)
	{
		pEffectOn->Init(pos, fMove, col, fRadius, fAlphaDown, nPattern);
	}

	// テクスチャ割り当て
	pEffectOn->BindTexture(CTexture::TEXTYPE_EFFECT);

	return pEffectOn;
}