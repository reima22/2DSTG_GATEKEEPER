//=============================================================================
//
// タイマーカウント描画処理 [timer_count.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TIMER_COUNT_H_
#define _TIMER_COUNT_H_

#include "main.h"
#include "timer.h"

// マクロ定義
#define TIMER_DIGIT_NUM	(3)		// 最大桁数
#define TIMER_START		(120)	// 開始時のタイマー

// 前方宣言
class CNumber;

//==============================================================================
// タイマーカウントクラス
//==============================================================================
class CTimerCount : public CTimer
{
public:
	CTimerCount();
	~CTimerCount();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimerCount *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

	void TimerDecrease(void);		// タイマー減少処理

	// タイマー数値の取得・設定
	int GetNumber(void) { return m_nTimer; }
	void SetNumber(int nTimer);

	// タイマーカウント開始フラグ設定
	bool GetStartCnt(void) { return m_bStartCnt; }
	void SetStartCnt(bool bStartCnt) { m_bStartCnt = bStartCnt; }

	// タイマーの引継ぎ
	static int GetTimerNum(void) { return m_nTimerResult; }

private:
	CNumber *m_apNumber[TIMER_DIGIT_NUM];	// ナンバークラスポインタ
	int m_nTimer;							// タイマー数値
	int m_nDecereaseCnt;					// 減少カウント
	bool m_bStartCnt;						// カウント開始のフラグ
	static int m_nTimerResult;				// リザルトへ引き継ぐ値
};
#endif