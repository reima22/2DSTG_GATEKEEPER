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

// マクロ定義
#define NUM_MAX	(8)	// 最大桁数

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
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	
	static void AddScore(int nValue);

	void SetScore(int nScore);
	static int GetScore(void) { return m_nScore; }

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	CNumber *GetNumber(int nNumber) { return m_apNumber[nNumber]; }

	static void SetResultScore(int nScore) { m_nResultScore = nScore; }
	static int GetResultScore(void) { return m_nResultScore; }

private:
	CNumber *m_apNumber[NUM_MAX];	// ナンバークラスポインタ
	static int m_nScore;			// スコア数値
	static float m_fSizeX;			// サイズX軸
	static float m_fSizeY;			// サイズY軸
	static int m_nResultScore;		// 結果スコア
};

#endif
