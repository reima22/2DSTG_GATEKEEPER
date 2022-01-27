//=============================================================================
//
// UIの下地描画処理 [ui_base.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _UI_BASE_H_
#define _UI_BASE_H_

#include "main.h"
//#include "scene.h"

//==============================================================================
// タイマークラス
//==============================================================================
class CUiBase : public CScene2D
{
public:
	CUiBase(int nPriority = PRIORITY_UI);
	~CUiBase();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, char *pTypeCheck);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CUiBase *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, char *pTypeCheck);

private:
	CScene2D *m_pScene2D;	// 2Dポリゴンポインタ
};
#endif