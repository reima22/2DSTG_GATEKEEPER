//==============================================================================
//
// リザルトスコアの描画処理〔result_score.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "fade.h"
#include "result_score.h"
#include "result_score_extra.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
int CResultScore::m_nScore;								// スコア
int CResultScore::m_nShuffleCnt;						// シャッフル演出のカウント
bool CResultScore::m_bCntEventEnd[SCORETYPE_MAX] = {};	// 演出のフラグ

//==============================================================================
// コンストラクタ
//==============================================================================
CResultScore::CResultScore(int nPriority) : CResult(nPriority)
{
	m_nScore = 0;
	m_nShuffleCnt = 0;
	m_nShaffleIdx = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CResultScore::~CResultScore()
{
}

//==============================================================================
// リザルトスコアの初期化処理
//==============================================================================
HRESULT CResultScore::Init(D3DXVECTOR3 pos)
{
	// スコアの取得
	m_nScore = 0;

	// カウントのリセット
	m_nShuffleCnt = 0;

	// フラグのリセット
	for (int nCnt = 0; nCnt < SCORETYPE_MAX; nCnt++)
	{
		m_bCntEventEnd[nCnt] = false;
	}

	// リザルトスコアの生成
	m_pScore = CScore::Create(pos, RESULT_SCORE_SIZE);

	// スコア設定
	m_pScore->SetScore(m_nScore);

	return S_OK;
}

//==============================================================================
// リザルトスコアの終了処理
//==============================================================================
void CResultScore::Uninit(void)
{
	if (m_pScore != NULL)
	{// NULLチェック後に破棄
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// リザルトスコアの更新処理
//==============================================================================
void CResultScore::Update(void)
{
	// フェード情報の取得
	CFade::FADE fade = CManager::GetFade()->GetFade();

	if (fade == CFade::FADE_NONE && m_nShuffleCnt <= (RESULT_SHUFFLE + CNT_ONE_SECOND * (SCORETYPE_MAX - 1)))
	{
		// シャッフルカウントの加算
		m_nShuffleCnt++;

		if (m_nShuffleCnt >= RESULT_SHUFFLE + (CNT_ONE_SECOND * (SCORETYPE_MAX - 1)) && m_bCntEventEnd[SCORETYPE_MAX - 1] == false)
		{// 約2秒
			m_nScore = CScore::GetResultScore() + 
				CResult::GetScoreExPoint(CResult::EXSCORETYPE_TIMER_CALCU)->GetScoreEx() - 
				CResult::GetScoreExPoint(CResult::EXSCORETYPE_COUNT_CALCU)->GetScoreEx() + 
				CResult::GetScoreExPoint(CResult::EXSCORETYPE_BONUS_CALCU)->GetScoreEx();

			m_pSound->Play(CSound::SOUND_LABEL_SE_DECIDE);
		}
		else if(m_nShuffleCnt < RESULT_SHUFFLE  + (CNT_ONE_SECOND * (SCORETYPE_MAX - 1)) && m_nShuffleCnt % 2 == 0 && m_nShuffleCnt >= RESULT_SHUFFLE_START)
		{
			// 0~99999999
			m_nScore = (rand() % 10000) * 10000 + (rand() % 10000);

			m_pSound->Play(CSound::SOUND_LABEL_SE_COUNT);
		}
	}

	if (m_nShuffleCnt > RESULT_SHUFFLE + (CNT_ONE_SECOND * m_nShaffleIdx))
	{
		m_bCntEventEnd[m_nShaffleIdx] = true;
		m_nShaffleIdx++;
	}

	// スコアの設定
	m_pScore->SetScore(m_nScore);
}

//==============================================================================
// リザルトスコアの描画処理
//==============================================================================
void CResultScore::Draw(void)
{
	// ランキングスコアの描画
	if (m_pScore != NULL)
	{
		m_pScore->Draw();
	}
}

//==============================================================================
// リザルトスコアの生成処理
//==============================================================================
CResultScore *CResultScore::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CResultScore *pResultScore;
	pResultScore = new CResultScore;

	// 初期化
	if (pResultScore != NULL)
	{
		pResultScore->Init(pos);
	}

	return pResultScore;
}