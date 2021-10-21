//==============================================================================
//
// ボーダーライン描画処理〔line.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "window_effect.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "game.h"

// 静的メンバ変数宣言
CWindowEffect::EFFECT_STATE CWindowEffect::m_state;
bool CWindowEffect::m_bInvation = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CWindowEffect::CWindowEffect(int nPriority) : CScene2D(nPriority)
{
	// エフェクトの状態
	m_state = EFFECT_STATE_NONE;
}

//==============================================================================
// デストラクタ
//==============================================================================
CWindowEffect::~CWindowEffect()
{
}

//==============================================================================
// ボーダーラインの初期化処理
//==============================================================================
HRESULT CWindowEffect::Init(D3DXVECTOR3 pos)
{
	// メンバ変数の初期化
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// 生成
	CScene2D::Init(pos);
	CScene2D::SetSize(W_EFFECT_SIZEX, W_EFFECT_SIZEY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetCol(m_col);

	return S_OK;
}

//==============================================================================
// ボーダーラインの終了処理
//==============================================================================
void CWindowEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 画面エフェクトの更新処理
//==============================================================================
void CWindowEffect::Update(void)
{
	// エフェクトの状態
	SetEffectState();

	CScene2D::Update();
}

//==============================================================================
// 画面エフェクトの描画処理
//==============================================================================
void CWindowEffect::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// 画面エフェクトの生成処理
//==============================================================================
CWindowEffect *CWindowEffect::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CWindowEffect *pWindowEffect;
	pWindowEffect = new CWindowEffect;

	// 初期化
	pWindowEffect->Init(pos);

	// テクスチャの設定
	pWindowEffect->BindTexture(CTexture::TEXTYPE_WINDOWEFFECT);

	return pWindowEffect;
}

//==============================================================================
// 画面エフェクトの設定
//==============================================================================
void CWindowEffect::SetEffectState(void)
{
	// ラインの状態ごとの処理
	switch (m_state)
	{
	case EFFECT_STATE_NONE:
		// 透明
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.1f, 0.0f);

		break;

	case EFFECT_STATE_INVASION:

		if (m_bInvation == true)
		{
			// リセット
			m_bAlpha = false;
		}

		if (m_bAlpha == false)
		{// α値の設定
			m_col.a = W_EFFECT_ALPHA_SET;
			m_bAlpha = true;
			m_bInvation = false;
		}

		// 侵入時のライン色変化
		m_col.a -= W_EFFECT_ALPHA;

		// 次ステータスへ移行
		if (m_col.a <= 0.0f)
		{
			// 状態変化
			if (CInvasion::GetInvasion() >= (float)CGame::GetEndInvasion() / 10 * 8)
			{// 警告状態
				m_state = EFFECT_STATE_CAUTION;				
			}
			else
			{// 通常へ
				m_state = EFFECT_STATE_NONE;
			}

			m_bAlpha = false;
		}

		break;

	case EFFECT_STATE_CAUTION:
		// α値の増減
		float fAlpha = W_EFFECT_ALPHA;

		if (m_bAlpha == false)
		{// 減少時
			fAlpha *= -1;
		}

		m_col.a += fAlpha;

		if (m_col.a <= W_EFFECT_ALPHA_DOWN)
		{
			m_bAlpha = true;
		}
		else if (m_col.a >= W_EFFECT_ALPHA_UP)
		{
			m_bAlpha = false;
		}

		break;
	}

	// 色の設定
	CScene2D::SetCol(m_col);
}