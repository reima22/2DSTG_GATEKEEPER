//=============================================================================
//
// ポーズ選択肢処理 [pause_select.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PAUSE_SELECT_H_
#define _PAUSE_SELECT_H_

#include "main.h"
#include "pause.h"

// 前方宣言
class CPolygon;

//==============================================================================
// ポーズ選択肢クラス
//==============================================================================
class CPauseSelect : public CScene2D
{
public:
	// ポーズ中の選択肢
	typedef enum
	{
		PAUSE_CONTINUE = 0,	// コンテニュー
		PAUSE_RETRY,		// リトライ
		PAUSE_QUIT,			// 終了
		PAUSE_MAX
	}PAUSE;

	CPauseSelect(int nPriority = CScene::PRIORITY_PAUSE);
	~CPauseSelect();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetPause(PAUSE pause) { m_pause = pause; }
	static CPauseSelect *Create(D3DXVECTOR3 pos);
	void SetMode(void);

private:
	static CScene2D *m_apScene2D[PAUSE_MAX];	// Scene2Dクラス
	static PAUSE m_pause;						// 選択中のカーソル
	static D3DXCOLOR m_col;						// 選択中のカーソル色
	bool m_bAlphaDown;							// 選択中カーソル点滅用判定
};
#endif
