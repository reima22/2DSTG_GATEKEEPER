//=============================================================================
//
// チュートリアルページ描画処理 [tutorial_page.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TUTORIAL_PAGE_H_
#define _TUTORIAL_PAGE_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define T_ENTER_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f))	// エンターサインの位置
#define T_ENTER_SIZEX	(340.0f)	// 幅
#define T_ENTER_SIZEY	(40.0f)		// 高さ

// 前方宣言
class CPolygon;

//==============================================================================
// チュートリアルページクラス
//==============================================================================
class CTutorialPage : public CScene
{
public:
	typedef enum
	{
		TUTORIAL_PAGE_0 = 0,
		TUTORIAL_PAGE_1,
		TUTORIAL_PAGE_2,
		TUTORIAL_PAGE_MAX
	}TUTORIAL_PAGE;

	CTutorialPage(int nPriority = PRIORITY_BG);
	~CTutorialPage();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTutorialPage *Create(D3DXVECTOR3 pos);
	static int GetPageCnt(void) { return m_nPageCnt; }

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

private:
	CPolygon *m_pPolygon[TUTORIAL_PAGE_MAX];	// ポリゴンクラスポインタ
	static int m_nPageCnt;								// ページのカウント
	static D3DXCOLOR m_col[TUTORIAL_PAGE_MAX];		// 色
};
#endif
