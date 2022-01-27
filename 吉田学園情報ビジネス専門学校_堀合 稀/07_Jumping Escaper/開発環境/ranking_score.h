//=============================================================================
//
// ランキングスコア描画処理 [ranking_score.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RANKING_SCORE_H_
#define _RANKING_SCORE_H_

#include "main.h"
#include "scene.h"
#include "ranking.h"
#include "score.h"

// マクロ定義
#define RANK_SCORE_POS			(D3DXVECTOR3(550.0f, 170.0f, 0.0f))		// 通常遷移時の配置開始位置
#define RANK_SCORE_POS_T		(D3DXVECTOR3(550.0f, 220.0f, 0.0f))		// 自動遷移時の配置開始位置
#define RANK_SCORE_SIZE			(D3DXVECTOR2(20.0f, 30.0f))

#define RANK_RESULTSCORE_POS	(D3DXVECTOR3(550.0f, 640.0f, 0.0f))		// 結果スコアの配置

// 前方宣言
class CScene2D;

//==============================================================================
// ランキングスコア描画クラス
//==============================================================================
class CRankingScore : public CRanking
{
public:
	CRankingScore(int nPriority = PRIORITY_UI);
	~CRankingScore();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ランキングスコアの生成
	static CRankingScore *Create(D3DXVECTOR3 pos);

	// ランキングスコアの取得
	static int GetRankingScore(int nRank) { return m_aScore[nRank]; }

	void SortScore(void);
	void AppealScore(void);
	
private:
	CScore *m_pScore[RANK_NUM];			// ポリゴンクラスポインタ
	CScore *m_pResultScore;				// 結果スコアポインタ

	static int m_aScore[RANK_NUM];		// ランキングスコア
	int m_nResultScore;					// 結果スコア
	bool m_bFromTitle;					// 自動遷移判定
	D3DXVECTOR3 m_pos;					// 位置

	bool m_bRankIn;						// ランクインのフラグ
	bool m_bRankInScore[RANK_NUM];		// ランクインアピールのフラグ
	D3DXCOLOR m_appealCol;				// ランクインアピールの色
	int m_nCntColor;					// 色変化のカウント
};
#endif