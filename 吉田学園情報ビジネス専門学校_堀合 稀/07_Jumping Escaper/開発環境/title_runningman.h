//=============================================================================
//
// タイトルランニングマン描画処理 [title_ruuningman.h]
// Author : Tatasuki Matsuda
//
//============================================================================= 
#ifndef _TITLE_RUNNINGMAN_H_
#define _TITLE_RUNNINGMAN_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

// マクロ定義
#define T_RUNNINGMAN_POS	(D3DXVECTOR3(0.0f, 350.0f, 0))									// ランニングマンの位置
#define T_RUNNINGMAN_SIZE	(D3DXVECTOR2(260.0f, 320.0f))									// ランニングマンのサイズ
#define T_RUNNINGMAN_SETPOS	(D3DXVECTOR3(SCREEN_WIDTH+T_RUNNINGMAN_SIZE.x, 300.0f, 0.0f))	// ランニングマンの最終座標

#define ANIMATION_PATTERN		(8)									//	アニメーションパターン数
#define ANIMATION_COUNT			(6)									//	アニメーションカウント

#define MOVE_RUNNINGMAN_Y		(0.0f)								//	タイトル　	Y軸の移動値
#define MOVE_RUNNINGMAN_X		(10.0f)								//	タイトル　	X軸の移動値
#define MOVE_SPEED				(15.0f)								//	速度値

//==============================================================================
// ランニングマンクラス
//==============================================================================
class CTitleRunningMan : public CScene2D
{
public:
	CTitleRunningMan(int nPriority = PRIORITY_UI);
	~CTitleRunningMan();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleRunningMan *Create(D3DXVECTOR3 pos);

	static bool GetLogoState(void) { return m_bRunningManMoveEnd; }

private:
	CScene2D *m_pScene2D;				// 2Dポリゴンクラスポインタ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_move;					// 移動力
	D3DXCOLOR m_col;					// 色

	float m_fSpead;						// 速度

	bool m_bEnter;						// エンターを押したかどうか
	bool m_bNextScene;					// 次のシーンに移動するかどうか
	bool m_bJump;						// ジャンプ判定
	static bool m_bRunningManMoveEnd;	// ランニングマン移動の終了 

	int m_nPatternAnimX;				// アニメーションX軸No.
	int m_nPatternAnimY;				// アニメーションY軸No.
	int m_nCounterAnim;					// アニメーションカウンター
	int m_nTimer;						// タイマー
};
#endif

