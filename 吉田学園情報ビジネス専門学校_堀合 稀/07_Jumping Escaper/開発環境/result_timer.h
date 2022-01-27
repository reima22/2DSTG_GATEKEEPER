//=============================================================================
//
// リザルトタイマー描画処理 [result_timer.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_TIMER_H_
#define _RESULT_TIMER_H_

#include "main.h"
#include "scene.h"
#include "result.h"
#include "ranking.h"
#include "score.h"

// マクロ定義
#define RESULT_TIMER_POS	(D3DXVECTOR3(250.0f, 400.0f, 0.0f))	// 通常遷移時の配置
#define RESULT_TIMER_SIZE	(D3DXVECTOR2(25.0f, 40.0f))			// 一桁分のサイズ

// 前方宣言
class CTimerCount;	// タイマーカウントクラス

//==============================================================================
// リザルトタイマー描画クラス
//==============================================================================
class CResultTimer : public CResult
{
public:
	CResultTimer(int nPriority = PRIORITY_UI);
	~CResultTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultTimer *Create(void);

private:
	CTimerCount *m_pTimerCount;	// 落下カウントクラス
	int m_nTimer;				// 結果落下カウント
};
#endif