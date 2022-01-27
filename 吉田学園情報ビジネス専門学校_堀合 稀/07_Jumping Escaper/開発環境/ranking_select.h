//=============================================================================
//
// ポーズ選択肢処理 [ranking_select.h]
// Author : Tatasuki Matsuda
//
//============================================================================= 
#ifndef _RANKING_SELECT_H_
#define _RANKING_SELECT_H_

#include "main.h"
#include "ranking.h"

//==============================================================================
// ランキング選択肢クラス
//==============================================================================
class CRankingSelect : public CRanking
{
public:
	// ランキング中の選択肢
	typedef enum
	{
		RANKING_RETRY = 0,	// リトライ
		RANKING_STAGE,		// ステージセレクト
		RANKING_QUIT,		// 終了
		RANKING_MAX
	}RANKING;

	CRankingSelect(int nPriority = CScene::PRIORITY_UI);
	~CRankingSelect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRankingSelect *Create(void);
	static void SetRanking(RANKING ranking) { m_ranking = ranking; }
	void SetMode(void);

private:
	static CScene2D *m_apScene2D[RANKING_MAX];	// Scene2Dクラス
	static RANKING m_ranking;					// 選択中のカーソル
	static D3DXCOLOR m_col;						// 選択中のカーソル色
	bool m_bAlphaDown;							// 選択中カーソル点滅用判定
	bool m_bOK;									// 選択決定用判定
	bool m_bSelect;								// 現在の選択判定
	int m_nCntTime;								// 点滅カウント時間
};
#endif
