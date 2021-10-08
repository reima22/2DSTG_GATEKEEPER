//=============================================================================
//
// フェード描画処理 [fade.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene.h"
#include "manager.h"

// 前方宣言
class CPolygon;

//==============================================================================
// フェードクラス
//==============================================================================
class CFade
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE = 0,	// 何もしない
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX
	}FADE;

	CFade();
	~CFade();
	static HRESULT Init(void);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	static CFade *Create(void);

	static void SetFade(FADE fade,CManager::MODE modeNext);
	static FADE GetFade(void);

	static void FadeInCancel(void);

private:
	static CPolygon *m_pPolygon;		// ポリゴンクラスポインタ
	static FADE m_fade;					// フェードの状態
	static CManager::MODE m_modeNext;	// 次のモード
	static D3DXCOLOR m_col;				// フェードの色・透明度
};
#endif