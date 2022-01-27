//==============================================================================
//
// ランキングモード処理〔ranking.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "bg.h" 
#include "fade.h"
#include "ranking.h"
#include "ranking_logo.h"
#include "ranking_rank.h"
#include "ranking_score.h"
#include "ranking_select.h"
#include "select.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CRanking::CRanking(int nPriority) : CMode(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CRanking::~CRanking()
{
}

//==============================================================================
// ランキング画面の初期化処理
//==============================================================================
HRESULT CRanking::Init()
{
	// ステージ情報
	CStageSelect::STAGE_SELECT select = CStageSelect::GetSelectingStage();

	// 自動遷移カウントの初期化
	//m_nAutoFadeCnt = 0;

	// 背景生成
	CBg::Create();

	// ロゴ生成
	switch (select)
	{
	case CStageSelect::STAGE_SELECT_1:
		
		CRankingLogo::Create(RANK_LOGO_POS, RANK_LOGO_SIZE, "RANKING_LOGO_EASY");
		
		break;

	case CStageSelect::STAGE_SELECT_2:
		
		CRankingLogo::Create(RANK_LOGO_POS, RANK_LOGO_SIZE, "RANKING_LOGO_NORMAL");

		break;

	case CStageSelect::STAGE_SELECT_3:
		
		CRankingLogo::Create(RANK_LOGO_POS, RANK_LOGO_SIZE, "RANKING_LOGO_HARD");

		break;

	default:
		break;
	}


	// スコアの生成
	CRankingScore::Create(RANK_SCORE_POS);

	// 順位の生成
	CRankingRank::Create();

	// 選択肢の生成
	CRankingSelect::Create();

	// BGM再生
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_RANKING) == true)
	{
		m_pSound->Play(CSound::SOUND_LABEL_BGM006);
	}

	return S_OK;
}

//==============================================================================
// ランキング画面の終了処理
//==============================================================================
void CRanking::Uninit(void)
{
	// BGMの停止
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_RANKING) == true)
	{
		m_pSound->Stop();
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// ランキング画面の更新処理
//==============================================================================
void CRanking::Update(void)
{

}

//==============================================================================
// ランキング画面の描画処理
//==============================================================================
void CRanking::Draw(void)
{

}

//==============================================================================
// ランキングモード生成処理
//==============================================================================
CRanking *CRanking::Create()
{
	// インスタンス生成
	CRanking *pRanking;
	pRanking = new CRanking;

	// 初期化
	if (pRanking != NULL)
	{
		pRanking->Init();
	}

	return pRanking;
}