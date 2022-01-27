//=============================================================================
//
// フォグ効果描画処理 [fog.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _FOG_H_
#define _FOG_H_

#include "main.h"

// マクロ定義
#define FOG_START	(620.0f)	// フォグ効果の距離開始点
#define FOG_END		(1245.0f)	// フォグ効果の距離終了点
#define FOG_COL_VALLEY	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))		// 渓谷ステージのフォグ色
#define FOG_COL_OCEAN	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))		// 海洋ステージのフォグ色
#define FOG_COL_LAVA	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// 溶岩ステージのフォグ色

//==============================================================================
// フォグ硬貨クラス
//==============================================================================
class CFog
{
public:
	CFog();
	~CFog();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFog *Create(void);

	// 距離の設定
	float GetFogStart(void) { return m_fFogStart; }
	float GetFogEnd(void) { return m_fFogEnd; }

	// フォグ効果の色設定
	void SetFogCol(D3DXCOLOR col) { m_col = col; }

private:
	float m_fFogStart;	// フォグ効果の開始
	float m_fFogEnd;	// フォグ効果終了の距離（完全に見えなくなる）
	D3DXCOLOR m_col;	// 色
};
#endif
