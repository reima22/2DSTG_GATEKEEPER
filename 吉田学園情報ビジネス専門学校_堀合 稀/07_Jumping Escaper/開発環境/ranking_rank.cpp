//==============================================================================
//
// ランキング順位の描画処理〔ranking_rank.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "ranking.h"
#include "ranking_rank.h"
#include "ranking_score.h"
#include "scene2D.h"

// 静的メンバ変数宣言
int CRankingRank::m_aRank[RANK_NUM];

//==============================================================================
// コンストラクタ
//==============================================================================
CRankingRank::CRankingRank(int nPriority) : CRanking(nPriority)
{
	// ポリゴンの初期化
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
	}

	m_pYourScore = NULL;
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
HRESULT CRankingRank::Init(void)
{
	// 自動遷移判定
	//m_bFromTitle = CTitle::GetAutoFade();

	// ポリゴン初期化
	for (int nCntRank = 0; nCntRank < RANK_NUM; nCntRank++)
	{
		if (m_apScene2D[nCntRank] == NULL)
		{
			m_apScene2D[nCntRank] = CScene2D::Create();
			m_apScene2D[nCntRank]->SetPosition(D3DXVECTOR3(RANK_POS.x, RANK_POS.y + (nCntRank * 90.0f), 0.0f));
			m_apScene2D[nCntRank]->SetSize(RANK_SIZE);
			m_apScene2D[nCntRank]->SetTex(1, 5, 0, m_aRank[nCntRank], 0.0f, 0.0f);
			m_apScene2D[nCntRank]->BindTexture("RANKING_RANK");

			// 順位の初期化
			m_aRank[nCntRank] = nCntRank;
		}

		m_aScore[nCntRank] = CRankingScore::GetRankingScore(nCntRank);
	}

	// 順位の設定
	SetRank();

	// 自動遷移でない場合の追加
	if (m_bFromTitle == false)
	{
		m_pYourScore = CScene2D::Create();
		m_pYourScore->SetPosition(RESULT_SCORE_LOGO_POS);
		m_pYourScore->SetSize(RANK_SIZE);
		m_pYourScore->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_pYourScore->BindTexture("RANKING_YOURSCORE");
	}
	
	return S_OK;
}

//==============================================================================
// ランキング順位の終了処理
//==============================================================================
void CRankingRank::Uninit(void)
{
	// ランキング順位表記の破棄
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}

	// 「今回のスコア」ロゴ表記の破棄
	if (m_pYourScore != NULL)
	{
		m_pYourScore->Uninit();
		m_pYourScore = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランキング順位の更新処理
//==============================================================================
void CRankingRank::Update(void)
{
	// ランキング順位の更新
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Update();
		}
	}

	// 「今回のスコア」ロゴの更新
	if (m_pYourScore != NULL)
	{
		m_pYourScore->Update();
	}
}

//==============================================================================
// ランキング順位の描画処理
//==============================================================================
void CRankingRank::Draw(void)
{
	// ランキング順位の描画
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}

	// 「今回のスコア」ロゴの描画
	if (m_pYourScore != NULL)
	{
		m_pYourScore->Draw();
	}
}

//==============================================================================
// ランキング順位の生成処理
//==============================================================================
CRankingRank *CRankingRank::Create(void)
{
	// インスタンス生成
	CRankingRank *pRankingRank;
	pRankingRank = new CRankingRank;

	// 初期化
	if (pRankingRank != NULL)
	{
		pRankingRank->Init();
	}

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
		if (m_aScore[nCnt] == m_aScore[nCnt - 1])
		{// 直前の順位のスコアと同じならば順位を揃える
			m_aRank[nCnt] = m_aRank[nCnt - 1];
		}

		m_apScene2D[nCnt]->SetTex(1, 5, 0, m_aRank[nCnt], 0.0f, 0.0f);
	}
}