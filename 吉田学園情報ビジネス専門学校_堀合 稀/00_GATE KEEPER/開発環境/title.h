//==============================================================================
//
// タイトル画面の描画〔title.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitTitle(void);	// タイトル画面の初期化処理
void UninitTitle(void);		// タイトル画面の終了処理
void UpdateTitle(void);		// タイトル画面の更新処理
void DrawTitle(void);		// タイトル画面の描画処理

#endif