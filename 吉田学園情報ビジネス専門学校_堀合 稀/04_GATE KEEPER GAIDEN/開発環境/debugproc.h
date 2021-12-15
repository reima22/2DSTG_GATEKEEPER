//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

// マクロ定義
#define DEBUG_TEXT_NUM	(1024)	// 文字列の長さ

//==============================================================================
// デバッグ表示クラス
//==============================================================================
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);

	static void Print(const char *fmt, ...);
	static void Draw(void);

	static CDebugProc *Create(void);

private:
	static LPD3DXFONT m_pFont;		// フォントのポインタ
	static char m_aStr[DEBUG_TEXT_NUM];	// 文字列を格納するバッファ
};

#endif
