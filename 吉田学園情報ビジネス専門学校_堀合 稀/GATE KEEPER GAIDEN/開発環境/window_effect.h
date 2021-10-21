//=============================================================================
//
// 画面エフェクト描画処理 [window_effect.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _WINDOW_EFFECT_H_
#define _WINDOW_EFFECT_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define W_EFFECT_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 315.0f, 0.0f))	// 位置
#define W_EFFECT_SIZEX	(SCREEN_WIDTH / 2)								// 幅
#define W_EFFECT_SIZEY	(315.0f)										// 高さ
#define W_EFFECT_ALPHA	(0.02f)											// 減少するα値
#define W_EFFECT_ALPHA_SET	(0.7f)										// 表示する際のα値
#define W_EFFECT_ALPHA_DOWN	(0.1f)										// 明滅時の最小α値
#define W_EFFECT_ALPHA_UP	(0.5f)										// 明滅時の最大α値

// 前方宣言
class CPolygon;

//==============================================================================
// 画面エフェクトクラス
//==============================================================================
class CWindowEffect : public CScene2D
{
public:
	// ラインの状態
	typedef enum
	{
		EFFECT_STATE_NONE = 0,	// 平常時
		EFFECT_STATE_INVASION,	// 侵入時
		EFFECT_STATE_CAUTION,	// 侵入数危険水域時
		EFFECT_STATE_MAX
	}EFFECT_STATE;

	CWindowEffect(int nPriority = PRIORITY_UI);
	~CWindowEffect();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CWindowEffect *Create(D3DXVECTOR3 pos);

	// 状態の設定
	static void SetState(EFFECT_STATE state) { m_state = state; }
	void SetEffectState(void);

	// 撃破数に応じた状態
	static void SetInvationEvent(void) { m_bInvation = !m_bInvation; }

private:
	static EFFECT_STATE m_state;	// ウィンドウの状態
	D3DXCOLOR m_col;				// 色
	bool m_bAlpha;					// α値増減の判定
	static bool m_bInvation;		// 侵入エフェクト中
};
#endif
