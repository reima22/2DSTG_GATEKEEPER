//==============================================================================
//
// リザルト画面の描画〔result.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitResult(void);	// リザルト画面の初期化処理
void UninitResult(void);	// リザルト画面の終了処理
void UpdateResult(void);	// リザルト画面の更新処理
void DrawResult(void);		// リザルト画面の描画処理
void SetVtxResult(int nCnt);

#endif