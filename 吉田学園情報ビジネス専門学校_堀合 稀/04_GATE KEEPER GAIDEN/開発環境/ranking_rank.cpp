//==============================================================================
//
// ランキング順位の描画処理〔ranking_rank.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "sound.h"
#include "title.h"
#include "ranking.h"
#include "ranking_rank.h"
#include "ranking_score.h"

// 静的メンバ変数宣言
int CRankingRank::m_nRank[RANK_NUM];

//==============================================================================
// コンストラクタ
//==============================================================================
CRankingRank::CRankingRank(int nPriority) : CScene(nPriority)
{
	// ポリゴンの初期化
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		m_apPolygon[nCnt] = NULL;
	}

	m_pLogo = NULL;
}

//==============================================================================
// デストラクタ
//==============================================================================
CRankingRank::~CRankingRank()
{
}

//==============================================================================
// ランキング順位の初期化処理
//==============================================================================
HRESULT CRankingRank::Init(D3DXVECTOR3 pos)
{
	// 自動遷移判定
	m_bFromTitle = CTitle::GetAutoFade();

	// メンバ変数の初期化
	m_pos = pos;

	// ポリゴン初期化
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_apPolygon[nCnt] == NULL)
		{
			m_apPolygon[nCnt] = CPolygon::Create(D3DXVECTOR3(pos.x, pos.y + (nCnt * 90.0f), 0.0f),
				RANK_SIZEX, RANK_SIZEY,
				CTexture::TEXTYPE_RANKING_RANK);

			// 順位の初期化
			m_nRank[nCnt] = nCnt;

			// テクスチャ座標設定
			m_apPolygon[nCnt]->SetTex(1, 5, 0, m_nRank[nCnt], 0.0f, 0.0f);
		}

		m_nScore[nCnt] = CRankingScore::GetRankingScore(nCnt);
	}

	// 順位の設定
	SetRank();

	// 自動遷移でない場合の追加
	if (m_bFromTitle == false)
	{
		m_pLogo = CPolygon::Create(RESULT_SCORE_LOGO_POS, RANK_SIZEX, RANK_SIZEY,
			CTexture::TEXTYPE_RANKING_SCORELOGO);
		// テクスチャ座標設定
		m_pLogo->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	}
	

	return S_OK;
}

//==============================================================================
// ランキング順位の終了処理
//==============================================================================
void CRankingRank::Uninit(void)
{
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_apPolygon[nCnt]->Uninit();
			m_apPolygon[nCnt] = NULL;
		}
	}

	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランキング順位の更新処理
//==============================================================================
void CRankingRank::Update(void)
{
	
}

//==============================================================================
// ランキング順位の描画処理
//==============================================================================
void CRankingRank::Draw(void)
{
	// ランキング順位の描画
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{
			m_apPolygon[nCnt]->Draw();
		}
	}

	if (m_pLogo != NULL)
	{
		m_pLogo->Draw();
	}
}

//==============================================================================
// ランキング順位の生成処理
//==============================================================================
CRankingRank *CRankingRank::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CRankingRank *pRankingRank;
	pRankingRank = new CRankingRank;

	// 初期化
	pRankingRank->Init(pos);

	return pRankingRank;
}

//==============================================================================
// 順位の設定
//==============================================================================
void CRankingRank::SetRank(void)
{
	// 1位を除いて順位の変動をチェック
	for (int nCnt = 1; nCnt < RANK_NUM; nCnt++)
	{
		if (m_nScore[nCnt] == m_nScore[nCnt - 1])
		{// 直前の順位のスコアと同じならば順位を揃える
			m_nRank[nCnt] = m_nRank[nCnt - 1];
		}

		m_apPolygon[nCnt]->SetTex(1, 5, 0, m_nRank[nCnt], 0.0f, 0.0f);
	}
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CRankingRank::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CRankingRank::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}