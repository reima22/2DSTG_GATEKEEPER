//=============================================================================
//
// ボーナスカウントUI描画処理 [bonus.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _BONUS_H_
#define _BONUS_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define BONUS_UIBASE_POS	(D3DXVECTOR3(1130.0f, 150.0f, 0.0f))	// UIベースの位置
#define BONUS_UIBASE_SIZE	(D3DXVECTOR2(150.0f, 50.0f))			// UIベースのサイズ
#define BONUS_COUNT_POS		(D3DXVECTOR3(1160.0f, 150.0f, 0.0f))	// カウントの位置
#define BONUS_COUNT_SIZE	(D3DXVECTOR2(20.0f, 40.0f))				// カウント1桁分のサイズ
#define BONUS_LOGO_POS		(D3DXVECTOR3(1070.0f, 150.0f, 0.0f))	// ロゴの位置
#define BONUS_LOGO_SIZE		(D3DXVECTOR2(60.0f, 40.0f))				// ロゴのサイズ

// 前方宣言
class CBonusLogo;		// ボーナスロゴクラス
class CBonusCount;		// ボーナスカウントクラス

//==============================================================================
// 落下数UIクラス
//==============================================================================
class CBonus : public CScene
{
public:
	CBonus(int nPriority = PRIORITY_UI);
	~CBonus();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBonus *Create(void);

	CBonusCount *GetCount(void) { return m_pBonusCount; }

private:
	CBonusLogo *m_pBonusLogo;	// ボーナスロゴクラスポインタ
	CBonusCount *m_pBonusCount;	// ボーナスカウントクラスポインタ
};

#endif#pragma once
