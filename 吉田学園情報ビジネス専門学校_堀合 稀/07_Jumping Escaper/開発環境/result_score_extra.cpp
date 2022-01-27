//==============================================================================
//
// 他リザルトスコアの描画処理〔result_score_extra.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "bonus_count.h"
#include "fade.h"
#include "fall_count.h"
#include "number.h"
#include "result_score_extra.h"
#include "timer_count.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CResultScoreEx::CResultScoreEx(int nPriority) : CResultScore(nPriority)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CResultScoreEx::~CResultScoreEx()
{
}

//==============================================================================
// 他リザルトスコアの初期化処理
//==============================================================================
HRESULT CResultScoreEx::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nDigit, int nScore)
{
	// 変数設定
	m_nDigit = nDigit;
	m_nScoreEx = nScore;

	// 数字クラスの動的確保
	m_pNumber = new CNumber*[m_nDigit];

	// 桁の設定
	for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
	{
		m_pNumber[nCntDigit] = CNumber::Create(
			D3DXVECTOR3(pos.x + (nCntDigit * (size.x * 2)), pos.y, 0.0f), size);
	}

	return S_OK;
}

//==============================================================================
// 他リザルトスコアの終了処理
//==============================================================================
void CResultScoreEx::Uninit(void)
{
	// ナンバークラスの開放
	if (m_pNumber != NULL)
	{
		if (m_nDigit > 1)
		{
			delete[] m_pNumber;
		}
		else
		{
			delete m_pNumber;
		}

		m_pNumber = NULL;
	}

	Release();	// インスタンスのデリート
}

//==============================================================================
// 他リザルトスコアの更新処理
//==============================================================================
void CResultScoreEx::Update(void)
{
	//// フェード情報の取得
	//CFade::FADE fade = CManager::GetFade()->GetFade();
	//if (fade == CFade::FADE_NONE)
	//{
	//	m_nShuffleCnt++;
	//	if (m_nShuffleCnt >= 150 && m_bCntEventEnd == false)
	//	{// 約2秒
	//		m_nScore = CScore::GetResultScore();
	//		m_pSound->Play(CSound::SOUND_LABEL_SE_COUNT);
	//		m_bCntEventEnd = true;
	//	}
	//	else if (m_nShuffleCnt < 150 && m_nShuffleCnt % 2 == 0 && m_nShuffleCnt >= 30)
	//	{
	//		// 0~99999999
	//		m_nScore = (rand() % 10000) * 10000 + (rand() % 10000);
	//		m_pSound->Play(CSound::SOUND_LABEL_SE_COUNT);
	//	}
	//}
	//// スコアの設定
	//m_pScore->SetScore(m_nScore);

	// スコアの設定
	SetScoreEx(m_nScoreEx);
}

//==============================================================================
// 他リザルトスコアの描画処理
//==============================================================================
void CResultScoreEx::Draw(void)
{
	for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
	{
		if (m_pNumber[nCntDigit] != NULL)
		{
			m_pNumber[nCntDigit]->Draw();
		}
	}
}

//==============================================================================
// 他リザルトスコアの生成処理
//==============================================================================
CResultScoreEx *CResultScoreEx::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nDigit,int nScore)
{
	// インスタンス生成
	CResultScoreEx *pResultScoreEx;
	pResultScoreEx = new CResultScoreEx;

	// 初期化
	if (pResultScoreEx != NULL)
	{
		pResultScoreEx->Init(pos, size, nDigit,nScore);
	}

	return pResultScoreEx;
}

//==============================================================================
// 他リザルトスコア設定
//==============================================================================
void CResultScoreEx::SetScoreEx(int nScore)
{
	// スコアの設定
	m_nScoreEx = nScore;

	int nData = (int)pow(10, (m_nDigit - 1));

	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		// 各桁の算出
		int nNumber;
		nNumber = (m_nScoreEx / nData) % 10;
		nData /= 10;

		// 桁の設定
		m_pNumber[nCnt]->SetNumber(nNumber);
	}
}

//==============================================================================
// 数値のシャッフル演出
//==============================================================================s
void CResultScoreEx::ShffleNum(int nType)
{
	// ローカル変数宣言
	int nShuffleCnt = CResultScore::GetShuffleCnt();

	// イベント終了フラグの取得
	bool bEndEvent[SCORETYPE_MAX];

	bEndEvent[SCORETYPE_TIMER] = CResultScore::GetEnd(SCORETYPE_TIMER);
	bEndEvent[SCORETYPE_FALL] = CResultScore::GetEnd(SCORETYPE_FALL);
	bEndEvent[SCORETYPE_BONUS] = CResultScore::GetEnd(SCORETYPE_BONUS);

	// 計算用のタイム、落下数
	int nTimer = CTimerCount::GetTimerNum();
	int nFallCount = CFallCount::GetFallNum();
	int nBonus = CBonusCount::GetBonusNum();

	// フェード情報の取得
	CFade::FADE fade = CManager::GetFade()->GetFade();

	// フェードイン完了後
	if (fade == CFade::FADE_NONE)
	{
		if (nType == CResult::EXSCORETYPE_TIMER_CALCU)
		{
			if (nShuffleCnt >= 150 && bEndEvent[0] == false)
			{// 約2秒
				m_nScoreEx = nTimer * EXSCORE_TIMER;

				m_pSound->Play(CSound::SOUND_LABEL_SE_DECIDE);
			}
			else if (nShuffleCnt < RESULT_SHUFFLE && nShuffleCnt % 2 == 0 && nShuffleCnt >= RESULT_SHUFFLE_START)
			{
				// 0~99999999
				m_nScoreEx = (rand() % 10000) * 10000 + (rand() % 10000);

				m_pSound->Play(CSound::SOUND_LABEL_SE_COUNT);
			}
		}
		else if (nType == CResult::EXSCORETYPE_COUNT_CALCU)
		{
			if (nShuffleCnt >= 210 && bEndEvent[1] == false)
			{// 約2秒
				m_nScoreEx = nFallCount * EXSCORE_COUNT;

				m_pSound->Play(CSound::SOUND_LABEL_SE_DECIDE);
			}
			else if (nShuffleCnt < 210 && nShuffleCnt % 2 == 0 && nShuffleCnt >= RESULT_SHUFFLE_START)
			{
				// 0~99999999
				m_nScoreEx = (rand() % 10000) * 10000 + (rand() % 10000);

				m_pSound->Play(CSound::SOUND_LABEL_SE_COUNT);
			}
		}
		else if(nType == CResult::EXSCORETYPE_BONUS_CALCU)
		{
			if (nShuffleCnt >= 270 && bEndEvent[2] == false)
			{// 約2秒
				m_nScoreEx = nBonus * EXSCORE_BONUS;

				m_pSound->Play(CSound::SOUND_LABEL_SE_DECIDE);
			}
			else if (nShuffleCnt < 270 && nShuffleCnt % 2 == 0 && nShuffleCnt >= RESULT_SHUFFLE_START)
			{
				// 0~99999999
				m_nScoreEx = (rand() % 10000) * 10000 + (rand() % 10000);

				m_pSound->Play(CSound::SOUND_LABEL_SE_COUNT);
			}
		}
	}
}

//==============================================================================
// 色の設定
//==============================================================================
void CResultScoreEx::SetCol(D3DXCOLOR col)
{
	// 色の反映
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		m_pNumber[nCntNumber]->SetCol(col);
	}
}