//=============================================================================
//
// リザルトスコア描画処理 [result_score.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_SCORE_H_
#define _RESULT_SCORE_H_

#include "main.h"
#include "scene.h"
#include "result.h"
#include "ranking.h"
#include "score.h"

// マクロ定義
#define RESULT_SCORE_POS		(D3DXVECTOR3(540.0f, 260.0f, 0.0f))	// 結果スコアの配置
#define RESULT_SCORE_SIZE		(D3DXVECTOR2(40.0f, 60.0f))			// 結果スコアのサイズ

#define RESULT_SHUFFLE_START	(30)	// シャッフル開始		
#define RESULT_SHUFFLE			(150)	// 数値シャッフルを完了する基準値
#define RESULT_SHUFFLE_END		(330)	// シャッフル演出の終了するカウント	

//==============================================================================
// リザルトスコア描画クラス
//==============================================================================
class CResultScore : public CResult
{
public:
	// スコアの種類
	typedef enum 
	{
		SCORETYPE_TIMER = 0,	// タイマー
		SCORETYPE_FALL,			// 落下数
		SCORETYPE_BONUS,		// ボーナス
		SCORETYPE_MAIN,			// メインスコア
		SCORETYPE_MAX
	}SCORETYPE;

	CResultScore(int nPriority = PRIORITY_UI);
	~CResultScore();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultScore *Create(D3DXVECTOR3 pos);

	// 値の取得
	static int GetScore(void) { return m_nScore; }

	// シャッフルカウントの取得・設定
	static int GetShuffleCnt(void) { return m_nShuffleCnt; }
	static void SetShuffleCnt(int nSet) { m_nShuffleCnt = nSet; }

	// 演出終了フラグの取得
	static bool GetEnd(int nIdx) { return m_bCntEventEnd[nIdx]; }

	// スコアポインタの取得
	CScore *GetScorePoint(void) { return m_pScore; }

private:
	CScore *m_pScore;								// ポリゴンクラスポインタ
	static int m_nScore;							// 結果スコア
	D3DXVECTOR3 m_pos;								// 位置
	static int m_nShuffleCnt;						// スコア表示シャッフルカウント
	static bool m_bCntEventEnd[SCORETYPE_MAX];		// カウント演出の終了
	int m_nShaffleIdx;								// シャッフルするスコアのインデックス
};
#endif