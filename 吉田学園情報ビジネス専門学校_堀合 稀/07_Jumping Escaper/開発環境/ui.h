//=============================================================================
//
// UIの描画処理 [ui.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CTimer;			// タイマーUI
class CFall;			// 落下数UI
class CBonus;			// ボーナスUI
class CStartCntdown;	// カウントダウンロゴ
class CCheckpointLogo;	// チェックポイントロゴ
class CAlertLogo;		// タイマーアラートロゴ

//==============================================================================
// UIクラス
//==============================================================================
class CUi : public CScene
{
public:
	CUi(int nPriority = PRIORITY_UI);
	~CUi();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CUi *Create(void);

	// ポインタの取得
	static CTimer			*GetTimer(void)				{ return m_pTimer; }
	static CFall			*GetFall(void)				{ return m_pFall; }
	static CStartCntdown	*GetStartCntdown(void)		{ return m_pStartCntdown; }
	static CBonus			*GetBonus(void)				{ return m_pBonus; }
	static CCheckpointLogo	*GetCheckPointLogo(void)	{ return m_pCheckPointLogo; }
	static CAlertLogo		*GetAlertLogo(void)			{ return m_pAlertLogo; }

private:
	static CTimer *m_pTimer;					// タイマーのポインタ
	static CFall *m_pFall;						// 落下数のポインタ
	static CBonus *m_pBonus;					// ボーナスクラスポインタ
	static CStartCntdown *m_pStartCntdown;		// 開始時カウントダウンクラス
	static CCheckpointLogo *m_pCheckPointLogo;	// チェックポイントロゴ
	static CAlertLogo *m_pAlertLogo;			// タイマーアラートロゴ
};
#endif