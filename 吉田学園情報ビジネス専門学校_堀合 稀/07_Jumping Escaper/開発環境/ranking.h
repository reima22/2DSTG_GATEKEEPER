
//=============================================================================
//
// ランキングモード処理 [ranking.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "scene.h"
#include "mode.h"

// マクロ定義
#define RANK_NUM		(5)		// 順位の数
#define AUTO_FADE_CNT_T	(300)	// タイトルへの自動遷移カウント

// 前方宣言
class CScore;

//==============================================================================
// ランキングモードクラス
//==============================================================================
class CRanking : public CMode
{
public:
	CRanking(int nPriority = PRIORITY_DEFAULT);
	~CRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRanking *Create(void);

private:
	bool m_bFromTitle;					// タイトルからの遷移判定
	int m_nAutoFadeCnt;					// タイトルへ戻る自動遷移カウント
};

#endif
