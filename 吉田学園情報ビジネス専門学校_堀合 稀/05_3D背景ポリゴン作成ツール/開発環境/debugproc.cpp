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
#include "main.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "meshfield.h"
#include "exe.h"

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

	CMeshfield *pMeshfield = CExe::GetMeshField();

	int nCountFPS = GetFPSCnt();	// FPSカウントの取得

	char *pWaveType = pMeshfield->GetWaveTypeChar();
	char *pSyntheticType = pMeshfield->GetSyntheticTypeChar();
	char *pTex = pMeshfield->GetTexChar();
	char *pWireONOFF = pMeshfield->GetWireONOFF();

	float fHeightWave = pMeshfield->GetHeightWave();
	float fDistanceWave = pMeshfield->GetDistanceWave();
	float fSpeedWave = pMeshfield->GetSpeedWave();
	bool bCutTex = pMeshfield->GetCutTex();
	bool bWireFrame = pMeshfield->GetWireFrame();
	int nTexInfo = pMeshfield->GetTex();
	float fTexMoveRot = pMeshfield->GetTexMoveRot(nTexInfo);
	float fTexMove = pMeshfield->GetTexMove(nTexInfo);
	int nTexIdx = pMeshfield->GetTexIdx(nTexInfo);

	Print("FPS:%d\n", nCountFPS);
	Print("カメラの移動：[WASD]\n");
	Print("視点の回転　左:[Z] 右:[C]\n");
	Print("ズームイン:[T] ズームアウト:[G]\n");
	Print("メッシュ情報の書き出し: [F1]\n");
	Print("波の切り替え:              [1] 波の種類:%s\n", pWaveType);
	Print("合成切り替え:              [2] 合成種類:%s\n", pSyntheticType);
	Print("分割切り替え:              [3] 分割有無:%d\n", (int)bCutTex);
	Print("操作テクスチャ切り替え:    [4]\n");
	Print("ワイヤーフレームの切り替え:[5] %s\n", pWireONOFF);
	Print("波の高さ +:[Y] -:[H]\n");
	Print("波形間隔 +:[U] -:[J]\n");
	Print("波の早さ +:[I] -:[K]\n");
	Print("テクスチャインデックスの操作: -:[N] +:[M]\n");
	Print("テクスチャの流れる方向: +:[Q] -:[E]\n");
	Print("テクスチャの流れる速さ: +:[V] -:[B]\n");
	Print("波形の高さ:%f\n", fHeightWave);
	Print("波形の間隔:%f\n", fDistanceWave);
	Print("波形の早さ:%f\n", fSpeedWave);
	Print("操作中のテクスチャ:%s\n", pTex);
	Print("現在のテクスチャインデックス:%d\n", nTexIdx);
	Print("テクスチャの流れる方向:%f\n", fTexMoveRot);
	Print("テクスチャの流れる速さ:%f\n", fTexMove);

	// テキストの描画
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

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