//==============================================================================
//
// デバッグ表示処理〔debugproc.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "debugproc.h"
#include "game.h"
#include "gamepad.h"
#include "fog.h"
#include "main.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"
#include "scene.h"

// 静的メンバ変数宣言
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[DEBUG_TEXT_NUM];

//==============================================================================
// コンストラクタ
//==============================================================================
CDebugProc::CDebugProc()
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CDebugProc::~CDebugProc()
{
}

//==============================================================================
// 初期化処理
//==============================================================================
void CDebugProc::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// フォントの作成
	D3DXCreateFont(
		pDevice, 20, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	// 文字列バッファの初期化
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//==============================================================================
// 終了処理
//==============================================================================
void CDebugProc::Uninit(void)
{
	// フォント情報の破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//==============================================================================
// プリント処理
//==============================================================================
void CDebugProc::Print(const char *fmt,...)
{
	// ローカル変数宣言
	const char *pText = fmt;
	char aStr[DEBUG_TEXT_NUM];
	memset(&aStr[0], 0, sizeof(aStr));

	// 可変個実引数
	va_list args;

	// 開始
	va_start(args, fmt);

	for (int nCnt = 0; *pText != '\0'; pText++) 
	{
		// 切り出しと書換
		switch (*pText)
		{
		case '%':

			pText++;

			switch (*pText)
			{
			case 'd':	// 整数
				nCnt += wsprintf(&aStr[nCnt], "%d", va_arg(args, int));
				break;

			case 'f':	// 浮動小数
				nCnt += sprintf(&aStr[nCnt], "%.3f", va_arg(args, double));
				break;

			case 'c':	// 文字
				nCnt += wsprintf(&aStr[nCnt], "%c", va_arg(args, char));
				break;

			case 's':	// 文字列
				nCnt += wsprintf(&aStr[nCnt], "%s", va_arg(args, const char*));
				break;
				
			default:
				break;
			}
			break;

		default:	// 例外
			nCnt += wsprintf(&aStr[nCnt], "%c", (char*)pText[0]);
			break;
		}
	}

	va_end(args);

	// 文字列の連結
	strcat(m_aStr,aStr);
}

//==============================================================================
// 描画処理
//==============================================================================
void CDebugProc::Draw(void)
{
	// ローカル変数宣言
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	CPlayer *pPlayer = CGame::GetPlayer();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	int nCountFPS = GetFPSCnt();		// FPSカウントの取得
	int nNumAll = CScene::GetNumAll();
	int nMotion = 0;
	int nFrame = 0;
	int nKeyCnt = 0;
	D3DXVECTOR3 movePlayer;
	D3DXVECTOR3 posPlayer;
	D3DXVECTOR3 posSave;		// リスポーンポイント

	// NULLチェック
	if (pPlayer != NULL)
	{
		posPlayer = pPlayer->GetPosition();
		movePlayer = pPlayer->GetMove();
		posSave = pPlayer->GetPosSave();
	}

	int nlX = gamepad->GetPadlX();
	int nlY = gamepad->GetPadlY();

	CFog *pFog = CGame::GetFog();

	float fFogStart = 0.0f;
	float fFogEnd = 0.0f;
	
	if (pFog != NULL)
	{
		fFogStart = pFog->GetFogStart();
		fFogEnd = pFog->GetFogEnd();
	}

	Print("\n\n\n\n\n\nFPS:%d\nオブジェクト数:%d\n", nCountFPS,nNumAll);
	Print("左スティック傾き： lX:%d lY:%d\n", nlX, nlY);
	Print("プレイヤーの位置：X %f,Y %f,Z %f\n", posPlayer.x, posPlayer.y, posPlayer.z);
	Print("リスポーンポイント位置：X %f,Y %f,Z %f\n", posSave.x, posSave.y, posSave.z);
	Print("プレイヤーの移動力：%f %f %f\n", movePlayer.x, movePlayer.y, movePlayer.z);
	Print("フォグの開始点：%f \n", fFogStart);
	Print("フォグの終了点：%f \n", fFogEnd);

	// テキストの描画
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 255, 255));

	// 文字列バッファの初期化
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//==============================================================================
// 生成処理
//==============================================================================
CDebugProc *CDebugProc::Create(void)
{
	// インスタンス生成
	CDebugProc *pDebugProc;
	pDebugProc = new CDebugProc;

	// 初期化
	if (pDebugProc != NULL)
	{
		pDebugProc->Init();
	}

	return pDebugProc;
}