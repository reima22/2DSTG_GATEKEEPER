//=============================================================================
//
// ランキング順位描画処理 [ranking_rank.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RANKING_RANK_H_
#define _RANKING_RANK_H_

#include "main.h"
#include "scene.h"
#include "ranking.h"

// マクロ定義
#define RANK_POS				(D3DXVECTOR3(440.0f, 170.0f, 0.0f))	// 通常遷移時の配置
#define RANK_POS_T				(D3DXVECTOR3(440.0f, 220.0f, 0.0f))	// 自動遷移時の配置
#define RANK_SIZE				(D3DXVECTOR2(50.0f, 30.0f))			// サイズ

#define RESULT_SCORE_LOGO_POS	(D3DXVECTOR3(440.0f, 640.0f,0.0f))	// 結果スコアのロゴ 

// 前方宣言
class CScene2D;

//==============================================================================
// ランキング順位描画クラス
//==============================================================================
class CRankingRank : public CRanking
{
public:
	CRankingRank(int nPriority = PRIORITY_UI);
	~CRankingRank();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ランキング順位の生成
	static CRankingRank *Create(void);
	
	// 順位表記の設定
	void SetRank(void);

private:
	CScene2D *m_apScene2D[RANK_NUM];	// 順位表記
	CScene2D *m_pYourScore;				// 「今回のスコア」ロゴ

	bool m_bFromTitle;					// 自動遷移判定
	static int m_aRank[RANK_NUM];		// 順位
	int m_aScore[RANK_NUM];				// ランキングスコア
	D3DXVECTOR3 m_posBase;				// 1番目の基準となる位置
};
#endif