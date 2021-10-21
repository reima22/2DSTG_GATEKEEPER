//=============================================================================
//
// 警告画面描画処理 [caution.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CAUTION_H_
#define _CAUTION_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define CAUTION_CNT		(180)	// 演出する期間
#define CAUTION_ALPHA	(0.03f)	// 変化する透明度

// 前方宣言
class CPolygon;

//==============================================================================
// 警告画面クラス
//==============================================================================
class CCaution : public CScene
{
public:
	// ラインの状態
	typedef enum
	{
		CAUTION_STATE_NONE = 0,		// 透明化
		CAUTION_STATE_ALERT,		// アクティブ
		CAUTION_STATE_ALERT_END,	// アラートの終了
		CAUTION_STATE_MAX
	}CAUTION_STATE;

	CCaution(int nPriority = PRIORITY_UI);
	~CCaution();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCaution *Create(D3DXVECTOR3 pos);

	void SetCautionState(void);
	static CAUTION_STATE GetCautionState(void) { return m_cautionState; }	// 状態の取得

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

private:
	CPolygon *m_pPolygon;					// ポリゴンクラスポインタ
	static CAUTION_STATE m_cautionState;	// 警告の状態
	D3DXVECTOR3 m_pos;						// 位置
	static D3DXCOLOR m_col;					// 色
	int m_nCntState;						// 状態変化カウント
	int m_nCautionCnt;						// 危険水域時変化カウント

	bool m_bStartAlert;						// アラートの開始
	float m_fAlpha;							// 点滅α値
};
#endif