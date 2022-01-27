//=============================================================================
//
// タイトルロゴ描画処理 [title_logo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TITLE_LOGO_H_
#define _TITLE_LOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

// マクロ定義
#define T_LOGO_POS		(D3DXVECTOR3(300.0f, -1000.0f, 0.0f))			// ロゴの開始位置
#define T_LOGO_SIZE		(D3DXVECTOR2(900.0f / 2.0f, 450.0f / 2.0f))		// ロゴのサイズ
#define T_LOGO_SETPOS	(D3DXVECTOR3(630.0f, 230.0f, 0.0f))				// ロゴの最終地点
//#define COLOR_CNT		(50)

//#define POS_START_Y		(550.0f)								//	START,Y軸の位置
//#define MOVE_START_Y	(-8.0f)							//	START,Y軸の移動値
//#define POS_TITLE_Y				(230.0f)							//	タイトル　	Y軸の位置
//#define POS_TITLE_X				(630.0f)							//	タイトル　	X軸の位置

#define MOVE_TITLE_Y			(0.0f)								//	タイトル　	Y軸の移動値
#define MOVE_TITLE_X			(0.014f)
#define MOVE_SPEED				(15.0f)								//	速度値
#define GRAVITY					(0.4f)	

// 前方宣言
//class CPolygon;

//==============================================================================
// タイトルロゴクラス
//==============================================================================
class CTitleLogo : public CScene2D
{
public:
	CTitleLogo(int nPriority = PRIORITY_UI);
	~CTitleLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleLogo *Create(D3DXVECTOR3 pos);

	static bool GetLogoState(void) { return m_bLogoMoveEnd; }

private:
	CScene2D *m_pScene2D;		// 2Dポリゴンクラスポインタ

	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_move;			// 移動力
	D3DXCOLOR m_col;			// 色

	int m_nJumpCnt;				//	ジャンプカウント
	float m_fSpead;				// 速度

	static bool m_bLogoMoveEnd;	// ロゴ移動の終了
};
#endif
