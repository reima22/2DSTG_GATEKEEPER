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
#define RANK_SIZEX				(50.0f)								// 幅
#define RANK_SIZEY				(30.0f)								// 高さ

#define RESULT_SCORE_LOGO_POS	(D3DXVECTOR3(440.0f, 640.0f,0.0f))		// 結果スコアのロゴ 

// 前方宣言
class CPolygon;

//==============================================================================
// ランキング順位描画クラス
//==============================================================================
class CRankingRank : public CScene
{
public:
	CRankingRank(int nPriority = PRIORITY_UI);
	~CRankingRank();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CRankingRank *Create(D3DXVECTOR3 pos);
	void SetRank(void);

private:
	CPolygon *m_apPolygon[RANK_NUM];	// ポリゴンクラスポインタ
	CPolygon *m_pLogo;					// スコアロゴ用のポインタ
	bool m_bFromTitle;					// 自動遷移判定
	static int m_nRank[RANK_NUM];		// 順位
	int m_nScore[RANK_NUM];				// ランキングスコア
	D3DXVECTOR3 m_pos;					// 位置
};
#endif