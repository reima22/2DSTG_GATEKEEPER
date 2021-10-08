//==============================================================================
//
// リザルトスコアの描画処理〔result_score.cpp〕
// Author : Mare Horiai
//
//==============================================================================
//#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "sound.h"
#include "title.h"
#include "result_score.h"
#include "fade.h"
#include "score.h"
//#include "ranking_rank.h"
//#include "ranking_score.h"
//#include "stdio.h"

// 静的メンバ変数宣言
int CResultScore::m_nScore;
int CResultScore::m_nShuffleCnt;
bool CResultScore::m_bCntEventEnd;

//==============================================================================
// コンストラクタ
//==============================================================================
CResultScore::CResultScore(int nPriority) : CScene(nPriority)
{
	m_nScore = 0;
	m_nShuffleCnt = 0;
	m_bCntEventEnd = false;
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
	//m_nScore = CScore::GetScore();

	// リザルトスコアの生成
	m_pScore = CScore::Create(pos, RESULT_SCORE_SIZEX, RESULT_SCORE_SIZEY);

	//m_pScore->SetScore(m_nScore);

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
// ランキングスコアの更新処理
//==============================================================================
void CResultScore::Update(void)
{
	// フェード情報の取得
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{
		m_nShuffleCnt++;

		if (m_nShuffleCnt >= 150 && m_bCntEventEnd == false)
		{// 約2秒
			m_nScore = CScore::GetResultScore();

			CSound::Play(CSound::SOUND_LABEL_SE_COUNT);

			m_bCntEventEnd = true;
		}
		else if(m_nShuffleCnt < 150 && m_nShuffleCnt % 2 == 0 && m_nShuffleCnt >= 30)
		{
			// 0~99999999
			m_nScore = (rand() % 10000) * 10000 + (rand() % 10000);

			CSound::Play(CSound::SOUND_LABEL_SE_COUNT);
		}
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
	pResultScore->Init(pos);

	return pResultScore;
}

//==============================================================================
// 位置の取得
//==============================================================================
D3DXVECTOR3 CResultScore::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// サイズの取得
//==============================================================================
D3DXVECTOR3 CResultScore::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}