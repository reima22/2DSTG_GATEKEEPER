//=============================================================================
//
// 難易度選択肢処理 [title_difficulty.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TITLE_DIFFICULTY_H_
#define _TITLE_DIFFICULTY_H_

#include "main.h"
#include "scene2D.h"

#define T_DIFFICULT_POS	(D3DXVECTOR3(640.0f, 480.0f, 0.0f))
#define T_DIFFICULT_X	(200.0f)
#define T_DIFFICULT_Y	(50.0f)

//==============================================================================
// 難易度選択肢クラス
//==============================================================================
class CTitleDifficulty : public CScene2D
{
public:
	// 選択肢
	typedef enum
	{
		DIFFICULTY_EASY = 0,	// 簡単
		DIFFICULTY_NORMAL,		// 普通
		DIFFICULTY_HARD,		// 難しい
		DIFFICULTY_MAX
	}DIFFICULTY;

	CTitleDifficulty(int nPriority = CScene::PRIORITY_UI);
	~CTitleDifficulty();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleDifficulty *Create(D3DXVECTOR3 pos);
	void SetMode(void);

	// 選択開始の可否
	static bool GetSelect(void) { return m_bSelectDifficulty; }
	static void SetSelect(bool bSelect) { m_bSelectDifficulty = bSelect; }

	// 選択した難易度の取得
	static DIFFICULTY GetDifficulty(void) { return m_difficulty; }

private:
	static CScene2D *m_apScene2D[DIFFICULTY_MAX];	// Scene2Dクラス
	static DIFFICULTY m_difficulty;					// 選択中のカーソル
	static D3DXCOLOR m_col[DIFFICULTY_MAX];			// カーソル色
	static bool m_bSelectDifficulty;				// 選択開始			
	bool m_bAlphaDown;								// 選択中カーソル点滅用判定
	int m_nBlinkCnt;								// 点滅カウント
};
#endif