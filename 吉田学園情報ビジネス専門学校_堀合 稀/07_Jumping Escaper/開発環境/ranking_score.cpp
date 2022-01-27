//==============================================================================
//
// ランキングスコアの描画処理〔ranking_score.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "number.h"
#include "ranking.h"
#include "ranking_score.h"
#include "result_score.h"
#include "scene2D.h"
#include "stdio.h"
#include "textdata_ranking.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
int CRankingScore::m_aScore[RANK_NUM];

//==============================================================================
// コンストラクタ
//==============================================================================
CRankingScore::CRankingScore(int nPriority) : CRanking(nPriority)
{
	// ランクインのフラグ
	m_bRankIn = false;

	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{// ランクインアピールを行うスコアのフラグ
		m_bRankInScore[nCnt] = false;
	}

	// ランクインアピールの色
	m_appealCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 色変化のカウント
	m_nCntColor = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CRankingScore::~CRankingScore()
{
}

//==============================================================================
// ランキングスコアの初期化処理
//==============================================================================
HRESULT CRankingScore::Init(D3DXVECTOR3 pos)
{
	// スコアの読み込み
	CTextDataRanking::LoadData();

	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		//for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
		{// 読み込んだスコアの取得
			m_aScore[nCnt] = CTextDataRanking::GetData(nCnt);
		}

		// スコアの生成
		m_pScore[nCnt] = CScore::Create(D3DXVECTOR3(pos.x, pos.y + (nCnt * 90.0f), 0.0f), RANK_SCORE_SIZE);

		// スコア値の設定
		m_pScore[nCnt]->SetScore(m_aScore[nCnt]);
	}

	if (m_bFromTitle == false)
	{	
		// 結果スコアの設定
		m_nResultScore = CResultScore::GetScore();

		// 結果スコアの生成
		m_pResultScore = CScore::Create(RANK_RESULTSCORE_POS, RANK_SCORE_SIZE);

		// スコア設定
		m_pResultScore->SetScore(m_nResultScore);

		// スコアの並び替え
		SortScore();
	}

	return S_OK;
}

//==============================================================================
// ランキングスコアの終了処理
//==============================================================================
void CRankingScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_pScore[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_pScore[nCnt]->Uninit();
			m_pScore[nCnt] = NULL;
		}
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランキングスコアの更新処理
//==============================================================================
void CRankingScore::Update(void)
{
	// ランキングスコアの更新
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_pScore[nCnt] != NULL)
		{
			m_pScore[nCnt]->Update();
		}
	}

	// ランクインのアピール
	if (m_bRankIn == true)
	{
		AppealScore();
	}
}

//==============================================================================
// ランキングスコアの描画処理
//==============================================================================
void CRankingScore::Draw(void)
{
	// ランキングスコアの描画
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_pScore[nCnt] != NULL)
		{
			m_pScore[nCnt]->Draw();
		}
	}
}

//==============================================================================
// ランキングスコアの生成処理
//==============================================================================
CRankingScore *CRankingScore::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CRankingScore *pRankingScore;
	pRankingScore = new CRankingScore;

	// 初期化
	if (pRankingScore != NULL)
	{
		pRankingScore->Init(pos);
	}

	return pRankingScore;
}

//==============================================================================
// スコアの並び替え
//==============================================================================
void CRankingScore::SortScore(void)
{
	// ローカル変数宣言
	int nSubData;	// 交換用変数

	// ランキング外の除外
	if (m_aScore[RANK_NUM - 1] <= m_nResultScore)
	{ // スコアがランキング内の場合
		m_bRankIn = true;

		m_aScore[RANK_NUM - 1] = m_nResultScore;

		// 計算処理
		for (int nCnt = 0; nCnt < RANK_NUM - 1; nCnt++)
		{
			for (int nCnt2 = nCnt + 1; nCnt2 < RANK_NUM; nCnt2++)
			{
				if (m_aScore[nCnt] < m_aScore[nCnt2])
				{
					nSubData = m_aScore[nCnt];
					m_aScore[nCnt] = m_aScore[nCnt2];
					m_aScore[nCnt2] = nSubData;
				}
			}
		}

		for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
		{
			// ランクイン判定
			if (m_aScore[nCnt] == m_nResultScore)
			{
				m_bRankInScore[nCnt] = true;
				break;
			}
		}

		// 並び替えたスコアの設定
		for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
		{
			// スコア値の設定
			m_pScore[nCnt]->SetScore(m_aScore[nCnt]);
		}

		// データの保存処理
		CTextDataRanking::SaveData();
	}
}

//==============================================================================
// ランクインのアピール
//==============================================================================
void CRankingScore::AppealScore(void)
{
	// カウントの加算
	m_nCntColor++;

	// 特定のスコアの点滅
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (m_bRankInScore[nCnt] == true)
		{
			if (m_nCntColor % 20 == 0)
			{
				m_appealCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (m_nCntColor % 10 == 0 && m_nCntColor % 20 != 0)
			{
				m_appealCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			for (int nCntNum = 0; nCntNum < NUM_MAX; nCntNum++)
			{
				m_pScore[nCnt]->GetNumber(nCntNum)->SetCol(m_appealCol);
			}

			break;
		}
	}
}