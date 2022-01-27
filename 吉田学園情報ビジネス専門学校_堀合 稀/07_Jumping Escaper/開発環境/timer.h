//=============================================================================
//
// タイマー描画処理 [timer.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define TIMER_UIBASE_POS	(D3DXVECTOR3(140.0f, 50.0f, 0.0f))	// UIベースの位置
#define TIMER_UIBASE_SIZE	(D3DXVECTOR2(140.0f, 50.0f))		// UIベースのサイズ
#define TIMER_COUNT_POS		(D3DXVECTOR3(150.0f, 50.0f, 0.0f))	// カウントの位置
#define TIMER_COUNT_SIZE	(D3DXVECTOR2(20.0f, 40.0f))			// カウント1桁分のサイズ
#define TIMER_LOGO_POS		(D3DXVECTOR3(70.0f, 50.0f, 0.0f))	// ロゴの位置
#define TIMER_LOGO_SIZE		(D3DXVECTOR2(60.0f, 40.0f))			// ロゴのサイズ

// 前方宣言
class CUiBase;			// UIベースクラス
class CTimerCount;		// タイマーカウントクラス
class CTimerLogo;		// タイマーロゴクラス

//==============================================================================
// タイマークラス
//==============================================================================
class CTimer : public CScene
{
public:
	CTimer(int nPriority = PRIORITY_UI);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer *Create(void);

	// タイマーカウントの取得
	CTimerCount	*GetTimerCnt(void) { return m_pTimerCount; }

private:
	CUiBase *m_pUiBase;				// UIベースクラス
	CTimerCount *m_pTimerCount;		// タイマーカウントクラスポインタ
	CTimerLogo *m_pTimerLogo;		// タイマーロゴクラスポインタ
};

#endif
