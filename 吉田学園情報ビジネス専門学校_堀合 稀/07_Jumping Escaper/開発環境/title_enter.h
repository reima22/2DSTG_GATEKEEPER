//=============================================================================
//
// タイトルエンターサイン描画処理 [title_enter.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TITLE_ENTER_H_
#define _TITLE_ENTER_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

// マクロ定義
#define T_ENTER_POS		(D3DXVECTOR3(640.0f, 3100.0f, 0))	// エンターサインの位置
#define T_ENTER_SIZE	(D3DXVECTOR2(800.0f / 2.0f, 150.0f / 2.0f))		// エンターサインのサイズ
#define T_ENTER_SETPOS	(D3DXVECTOR3(640.0f, 550.0f, 0.0f))

#define TIME_COLOR				(50)								//	色変化時間

#define MOVE_START_Y			(-8.0f)								//	START　		Y軸の移動値
#define MOVE_START_X			(0.0f)

//#define T_ENTER_SIZEX	(340.0f)	// 幅
//#define T_ENTER_SIZEY	(40.0f)		// 高さ



//==============================================================================
// エンターサインクラス
//==============================================================================
class CTitleEnter : public CScene2D
{
public:
	CTitleEnter(int nPriority = PRIORITY_UI);
	~CTitleEnter();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleEnter *Create(D3DXVECTOR3 pos);

private:
	CScene2D *m_pScene2D;	// 2Dポリゴンクラスポインタ
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_move;		// 移動力
	D3DXCOLOR m_col;		// 色

	bool m_bEnter;			// エンターを押したかどうか
	bool m_bNextScene;		// 次のシーンに移動するかどうか
	bool m_bJump;			// ジャンプ判定
	int m_nCntColor;		// 待機点滅カウント
	int m_nTimeColor;		// 決定点滅カウント
	int m_nTimer;			// タイマー
};
#endif
