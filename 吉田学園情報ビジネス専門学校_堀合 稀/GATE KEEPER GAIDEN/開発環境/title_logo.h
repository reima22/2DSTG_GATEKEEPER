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

// マクロ定義
#define T_LOGO_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, -80.0f, 0.0f))	// ロゴの位置、-80→200で移動
#define T_LOGO_SIZEX	(480.0f)	// 幅
#define T_LOGO_SIZEY	(80.0f)		// 高さ

// 前方宣言
class CPolygon;

//==============================================================================
// UIクラス
//==============================================================================
class CTitleLogo : public CScene
{
public:
	CTitleLogo(int nPriority = PRIORITY_UI);
	~CTitleLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CTitleLogo *Create(D3DXVECTOR3 pos);

	static bool GetLogoState(void) { return m_bLogoMoveEnd; }

private:
	CPolygon *m_pPolygon;		// ポリゴンクラスポインタ
	D3DXVECTOR3 m_pos;			// 位置
	int m_nMoveCnt;				// ロゴ移動のカウント
	static bool m_bLogoMoveEnd;	// ロゴ移動の終了
};
#endif
