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
#include "ranking.h"
#include "score.h"

// マクロ定義
#define RESULT_SCORE_POS	(D3DXVECTOR3(440.0f, 400.0f, 0.0f))	// 通常遷移時の配置
#define RESULT_SCORE_SIZEX	(30.0f)								// 幅
#define RESULT_SCORE_SIZEY	(45.0f)								// 高さ

// 前方宣言
class CPolygon;

//==============================================================================
// リザルトスコア描画クラス
//==============================================================================
class CResultScore : public CScene
{
public:
	CResultScore(int nPriority = PRIORITY_UI);
	~CResultScore();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CResultScore *Create(D3DXVECTOR3 pos);
	static int GetScore(void) { return m_nScore; }

	static int GetShuffleCnt(void) { return m_nShuffleCnt; }
	static void SetShuffleCnt(int nSet) { m_nShuffleCnt = nSet; }

	static bool GetEnd(void) { return m_bCntEventEnd; }

private:
	CScore *m_pScore;			// ポリゴンクラスポインタ
	static int m_nScore;		// 結果スコア
	D3DXVECTOR3 m_pos;			// 位置
	static int m_nShuffleCnt;	// スコア表示シャッフルカウント

	static bool m_bCntEventEnd;	// カウント演出の終了
};
#endif