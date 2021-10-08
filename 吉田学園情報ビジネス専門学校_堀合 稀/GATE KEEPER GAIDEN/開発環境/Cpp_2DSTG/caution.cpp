//==============================================================================
//
// ボス出現警告描画処理〔caution.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "caution.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "effect.h"
#include "game.h"

// 静的メンバ変数宣言
CCaution::CAUTION_STATE CCaution::m_cautionState = CAUTION_STATE_NONE;		// 警告の状態
D3DXCOLOR CCaution::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// ボーダーラインの色

//==============================================================================
// コンストラクタ
//==============================================================================
CCaution::CCaution(int nPriority) : CScene(nPriority)
{
	// ポリゴンの初期化
	m_pPolygon = NULL;

	// 状態変化カウント
	m_nCntState = 0;
	m_nCautionCnt = 0;

	m_cautionState = CAUTION_STATE_NONE;
	m_bStartAlert = false;

	m_fAlpha = CAUTION_ALPHA;
}

//==============================================================================
// デストラクタ
//==============================================================================
CCaution::~CCaution()
{
}

//==============================================================================
// 警告画面の初期化処理
//==============================================================================
HRESULT CCaution::Init(D3DXVECTOR3 pos)
{
	// メンバ変数の初期化
	m_pos = pos;

	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
			SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,
			CTexture::TEXTYPE_CAUTION);

		
		m_pPolygon->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		m_pPolygon->SetCol(m_col);
	}

	return S_OK;
}

//==============================================================================
// 警告画面の終了処理
//==============================================================================
void CCaution::Uninit(void)
{
	if (m_pPolygon != NULL)
	{// NULLチェック後に破棄
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// 警告画面の更新処理
//==============================================================================
void CCaution::Update(void)
{
	// ラインの状態
	SetCautionState();
}

//==============================================================================
// 警告画面の描画処理
//==============================================================================
void CCaution::Draw(void)
{
	// ラインの描画
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

//==============================================================================
// 警告画面の生成処理
//==============================================================================
CCaution *CCaution::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CCaution *pCaution;
	pCaution = new CCaution;

	// 初期化
	pCaution->Init(pos);

	return pCaution;
}

//==============================================================================
// 警告画面の設定
//==============================================================================
void CCaution::SetCautionState(void)
{
	// ラインの状態ごとの処理
	switch (m_cautionState)
	{
	case CAUTION_STATE_NONE:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		// 母艦戦へ移行
		if (CGame::GetMotherEnemy() == true && m_bStartAlert == false)
		{
			m_cautionState = CAUTION_STATE_ALERT;
			m_bStartAlert = true;
		}
		
		break;

	case CAUTION_STATE_ALERT:

		// カウント加算
		m_nCautionCnt++;

		// 色の点滅変化
		if (m_nCautionCnt <= 180)
		{			
			if (m_col.a <= 0.0f)
			{
				m_fAlpha = CAUTION_ALPHA;
			}
			else if (m_col.a >= 1.0f)
			{
				m_fAlpha = -CAUTION_ALPHA;
			}

			// 透明度変化
			m_col.a += m_fAlpha;
		}
		else
		{// アラート終了、透明へ
			m_col.a -= CAUTION_ALPHA;
			if (m_col.a <= 0.0f)
			{
				m_cautionState = CAUTION_STATE_ALERT_END;
			}
		}

		break;
	}

	m_pPolygon->SetCol(m_col);
}

//==============================================================================
// 位置の取得(仮想関数)
//==============================================================================
D3DXVECTOR3 CCaution::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CCaution::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}