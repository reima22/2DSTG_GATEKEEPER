//=============================================================================
//
// タイマーアラートロゴ描画処理 [timeralert_logo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TIMERALERT_LOGO_H_
#define _TIMERALERT_LOGO_H_

#include "main.h"
#include "scene2D.h"

// マクロ定義
#define ALERT_SIZE	(D3DXVECTOR2(400.0f, 80.0f))								// ロゴのサイズ
#define ALERT_POS	(D3DXVECTOR3(SCREEN_WIDTH + ALERT_SIZE.x, 500.0f, 0.0f))	// ロゴのデフォルト位置
#define ALERT_MOVE	(20.0f)														// ロゴの移動力

//==============================================================================
// タイマーアラートクラス
//==============================================================================
class CAlertLogo : public CScene2D
{
public:
	// 移動状態
	typedef enum
	{
		ALERTLOGOMOVE_NONE = 0,	// 初期状態
		ALERTLOGOMOVE_START,	// 移動開始
		ALERTLOGOMOVE_STOP,		// 一時停止
		ALERTLOGOMOVE_RESTART,	// 移動再開
		ALERTLOGOMOVE_END,		// 終了
	}ALERTLOGOMOVE;

	CAlertLogo(int nPriority = PRIORITY_UI);
	~CAlertLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAlertLogo *Create(void);

	void MoveTypeNext(void) { if (m_alertLogoMove == ALERTLOGOMOVE_NONE)m_alertLogoMove = (ALERTLOGOMOVE)(m_alertLogoMove + 1); }
	void MoveLogo(void);

private:
	CScene2D *m_pScene2D;			// 2Dポリゴンクラス
	D3DXVECTOR3 m_pos;				// 位置
	ALERTLOGOMOVE m_alertLogoMove;	// 移動状態
	int m_nCntStop;					// 停止間カウント
};

#endif#pragma once
