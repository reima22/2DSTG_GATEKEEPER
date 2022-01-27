//=============================================================================
//
// スコア描画処理 [score.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

// マクロ定義
#define NUM_MAX	(6)	// 最大桁数

// 前方宣言
class CNumber;

//==============================================================================
// スコアクラス
//==============================================================================
class CScore : public CScene
{
public:
	CScore(int nPriority = PRIORITY_UI);
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	
	void AddScore(int nValue) { m_nScore += nValue; }

	void SetScore(int nScore);
	int GetScore(void) { return m_nScore; }

	CNumber *GetNumber(int nNumber) { return m_apNumber[nNumber]; }

	static void SetResultScore(int nScore) { m_nResultScore = nScore; }
	static int GetResultScore(void) { return m_nResultScore; }

	void SetPosition(D3DXVECTOR3 pos) { m_posBase = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }
	void SetCol(D3DXCOLOR col);

private:
	CNumber *m_apNumber[NUM_MAX];	// ナンバークラスポインタ
	int m_nScore;					// スコア数値
	static int m_nResultScore;		// 結果スコア

	D3DXVECTOR3 m_posBase;
	D3DXVECTOR2 m_size;
};

#endif
