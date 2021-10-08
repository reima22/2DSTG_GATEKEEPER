//=============================================================================
//
// リザルトロゴ描画処理 [result_logo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_LOGO_H_
#define _RESULT_LOGO_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define R_LOGO_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f))
#define R_LOGO_SIZEX	(480)		// 幅
#define R_LOGO_SIZEY	(80.0f)		// 高さ

// 前方宣言
class CPolygon;

//==============================================================================
// リザルトロゴクラス
//==============================================================================
class CResultLogo : public CScene
{
public:
	CResultLogo(int nPriority = PRIORITY_UI);
	~CResultLogo();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultLogo *Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

private:
	CPolygon *m_pPolygon;	// ポリゴンクラスポインタ
	D3DXVECTOR3 m_pos;		// 位置
};
#endif
