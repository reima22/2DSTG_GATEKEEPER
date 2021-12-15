//==============================================================================
//
// 選択カーソルの描画〔select_cursor.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _SELECT_CURSOR_H_
#define _SELECT_CURSOR_H_
#include "main.h"

typedef enum
{
	CURSORTYPE_PLAYER1 = 0,
	CURSORTYPE_PLAYER2,
	CURSORTYPE_CPU,
	CURSORTYPE_MAX
}CURSORTYPE;

typedef enum
{
	CURSORSTATE_NORMAL = 0,		// 通常
	CURSORSTATE_BLINK,			// 点滅
	CURSORSTATE_ERASE,			// 消滅
	CURSORSTATE_MAX
}CURSORSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bUse;				// 使用状態
	CURSORTYPE cursorType;	// カーソルの種別
	int nCntBlink;			// カーソルの点滅カウント
	CURSORSTATE state;		// カーソルの状態
}CursorInfo;

typedef struct
{
	CursorInfo cursor[CURSORTYPE_MAX];
	int nMoveStartCnt;
	bool bBlinkEnd;			// 点滅状態終了
	int nCharNum;			// キャラ選択回数
}Cursor;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitCursor(void);			// 選択カーソルの初期化処理
void UninitCursor(void);			// 選択カーソルの終了処理
void UpdateCursor(void);			// 選択カーソルの更新処理
void DrawCursor(void);				// 選択カーソルの描画処理
void SetVertexCursor(int nIdx);		// 頂点情報更新
void CursorManager(void);			// カーソルの制御
Cursor GetCursorS(void);			// 選択カーソルの取得
void CursorState(void);				// カーソルの状態

#endif