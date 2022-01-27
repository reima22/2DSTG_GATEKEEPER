//=============================================================================
//
// 落下数ロゴ描画処理 [fall_logo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _FALL_LOGO_H_
#define _FALL_LOGO_H_

#include "main.h"
#include "fall.h"

//==============================================================================
// 落下数ロゴクラス
//==============================================================================
class CFallLogo : public CFall
{
public:
	CFallLogo();
	~CFallLogo();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFallLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

private:
	CScene2D *m_pScene2D;	// 2Dクラスポインタ
};

#endif

