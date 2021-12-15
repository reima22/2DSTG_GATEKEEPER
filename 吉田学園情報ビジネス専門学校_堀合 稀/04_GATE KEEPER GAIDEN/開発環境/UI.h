//=============================================================================
//
// UI描画処理 [UI.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CPolygon;

//==============================================================================
// UIクラス
//==============================================================================
class CUI : public CScene
{
public:
	CUI(int nPriority = PRIORITY_UI);
	~CUI();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CUI *Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

private:
	CPolygon *m_pPolygon;	// ポリゴンクラスポインタ
};
#endif