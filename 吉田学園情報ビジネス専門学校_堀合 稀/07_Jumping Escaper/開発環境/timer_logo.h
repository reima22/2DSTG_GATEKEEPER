//=============================================================================
//
// タイマーロゴ描画処理 [timer_logo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TIMER_LOGO_H_
#define _TIMER_LOGO_H_

#include "main.h"
#include "timer.h"

//==============================================================================
// タイマーロゴクラス
//==============================================================================
class CTimerLogo : public CTimer
{
public:
	CTimerLogo(int nPriority = PRIORITY_UI);
	~CTimerLogo();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimerLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

private:
	CScene2D *m_pScene2D;	// 2Dクラスポインタ
};
#endif