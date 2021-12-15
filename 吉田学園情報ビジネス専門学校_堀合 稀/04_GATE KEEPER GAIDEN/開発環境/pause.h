//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene.h"
#include "manager.h"

// マクロ定義
#define PAUSE_POS	(SCREEN_CENTER)

// 前方宣言
class CPolygon;

//==============================================================================
// ポーズクラス
//==============================================================================
class CPause
{
public:
	CPause();
	~CPause();
	HRESULT Init(void);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	static CPause *Create(void);
	static void SetPause(void);
	static bool GetPause(void) { return m_bPause; }

private:
	static bool m_bPause;	// ポーズの状態
};

#endif