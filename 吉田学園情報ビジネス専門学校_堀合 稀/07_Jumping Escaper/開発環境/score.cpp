//==============================================================================
//
// スコア描画処理〔score.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "manager.h"
#include "number.h"
#include "renderer.h"
#include "scene2D.h"
#include "score.h"

// 静的メンバ変数宣言
int CScore::m_nResultScore = 0;

//==============================================================================
// コンストラクタ
//==============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		// 各ナンバーの初期化
		m_apNumber[nCnt] = NULL;
	}
}

//==============================================================================
// デストラクタ
//==============================================================================
CScore::~CScore()
{
}

//==============================================================================
// スコアの初期化処理
//==============================================================================
HRESULT CScore::Init(void)
{
	// 数値の生成
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		if (m_apNumber[nCnt] == NULL)
		{
			m_apNumber[nCnt] = 
				CNumber::Create(
					D3DXVECTOR3(
						m_posBase.x + (nCnt * (m_size.x * 2)), 
						m_posBase.y, 
						0.0f), 
					m_size);
		}
	}

	// スコア初期化
	m_nScore = 0;

	return S_OK;
}

//==============================================================================
// スコアの終了処理
//==============================================================================
void CScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// NULLチェック後に破棄
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = NULL;
		}
	}

	Release();	// CScoreインスタンスのデリート
}

//==============================================================================
// スコアの更新処理
//==============================================================================
void CScore::Update(void)
{
	// 値の補正
	if (m_nScore < 0)
	{
		m_nScore = 0;
	}

	// スコアの設定
	SetScore(m_nScore);
}

//==============================================================================
// スコアの描画処理
//==============================================================================
void CScore::Draw(void)
{
	// ナンバーの描画
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		if (m_apNumber != NULL)
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//==============================================================================
// スコアの生成処理
//==============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScoreのインスタンス生成
	CScore *pScore;
	pScore = new CScore;

	// CScoreの初期化
	if (pScore != NULL)
	{
		pScore->SetPosition(pos);
		pScore->SetSize(size);
		pScore->Init();
	}

	return pScore;
}

//==============================================================================
// スコア設定
//==============================================================================
void CScore::SetScore(int nScore)
{
	// スコアの設定
	m_nScore = nScore;

	int nData = (int)pow(10, (NUM_MAX - 1));

	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		// 各桁の算出
		int nNumber;
		nNumber = (m_nScore / nData) % 10;
		nData /= 10;

		// 桁の設定
		m_apNumber[nCnt]->SetNumber(nNumber);
	}
}

//==============================================================================
// 色の設定
//==============================================================================
void CScore::SetCol(D3DXCOLOR col)
{
	// 色の設定
	for (int nCntDigit = 0; nCntDigit < NUM_MAX; nCntDigit++)
	{
		m_apNumber[nCntDigit]->SetCol(col);
	}
}