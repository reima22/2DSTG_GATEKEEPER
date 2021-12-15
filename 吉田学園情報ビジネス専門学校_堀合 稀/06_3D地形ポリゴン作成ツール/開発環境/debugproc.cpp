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
#include "player.h"
#include "gamepad.h"
#include "meshfield.h"
#include "exe.h"
#include "camera.h"

// 静的メンバ変数宣言
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[DEBUG_TEXT_NUM];
char *CDebugProc::m_apOnOff[2] =
{
	"OFF",
	"ON"
};

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
	CPlayer *pPlayer = CExe::GetPlayer();

	// キーボード
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// マウス取得
	CInputMouse *pMouse = CManager::GetInputMouse();

	// カメラ
	CCamera *pCamera = CManager::GetCamera();

	CMeshfield *pMeshfield = CExe::GetMeshField();

	CExe *pExe = CManager::GetExe();
	CExe::EDITMODE mode = CExe::GetEditMode();

	char *pMode = pExe->GetModeChar();

	int nCountFPS = GetFPSCnt();			// FPSカウントの取得
	int nNumAll = CScene::GetNumAll();

	bool bTexCut = pMeshfield->GetTexCut();
	bool bWireFrame = pMeshfield->GetWireFrame();
	int nTexIdx = pMeshfield->GetTexIdx();
	float fEditRadius = pMeshfield->GetEditArea();
	int nCntRepeat = keyboard->GetCntRepeat(CInput::KEYINFO_MESH_UP);

	D3DXVECTOR3 movePlayer;
	D3DXVECTOR3 posPlayer;

	D3DXVECTOR3 posCameraR = pCamera->GetPositionR();
	D3DXVECTOR3 posCameraV = pCamera->GetPositionV();
	D3DXVECTOR3 rotCamera = pCamera->GetRotation();
	float fLength =  pCamera->GetLength();
	float fGroundLength = pCamera->GetGroundLength();


	// NULLチェック
	if (pPlayer != NULL)
	{
		posPlayer = pPlayer->GetPosition();
		movePlayer = pPlayer->GetMove();
	}

	// 汎用
	Print("FPS:%d\nオブジェクト数:%d\n", nCountFPS, nNumAll);
	Print("データの保存　　  :[F1]\n");
	Print("データの読み込み　:[F2]\n");
	Print("編集モード切り替え:[F3] %s\n", pMode);
	Print("プレイヤーの操作:[WASD]\n");
	Print("プレイヤーの位置　：X %f, Y %f, Z %f\n", posPlayer.x, posPlayer.y, posPlayer.z);
	Print("カメラの注視点位置：X %f, Y %f, Z %f\n", posCameraR.x, posCameraR.y, posCameraR.z);
	Print("カメラの視点位置　：X %f, Y %f, Z %f\n", posCameraV.x, posCameraV.y, posCameraV.z);
	Print("カメラの角度    　：X %f, Y %f, Z %f\n", rotCamera.x, rotCamera.y, rotCamera.z);
	Print("視点間の距離　　　：%f\n", fLength);
	Print("地上の距離　　　　：%f\n", fGroundLength);
	Print("カメラの上昇:[R] 下降:[F]\n");
	Print("カメラのリセット:[X]\n");

	Print("\nマウス　lX:%d lY:%d lZ:%d \n", 
		pMouse->GetMouseState());

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_LEFT)) == true)
	{
		Print("\n「左クリック」\n");
	}
	
	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_RIGHT)) == true)
	{
		Print("\n「右クリック」\n");
	}

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_WHEEL)) == true)
	{
		Print("\n「ホイ－ル」\n");
	}

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_BBACK)) == true)
	{
		Print("\n「ブラウザバック」\n");
	}

	if (pMouse->GetPress(pMouse->GetButton(CInputMouse::MOUSEINFO_MOVEON)) == true)
	{
		Print("\n「進む」\n");
	}

	// モード別
	switch (mode)
	{
	case CExe::EDITMODE_EDIT:
		Print("\nカメラの移動:[↑←↓→]\n");
		Print("編集エリア 拡大:[Y] 縮小:[H]\n");
		Print("編集エリアの半径:%f\n", fEditRadius);
		Print("\n頂点座標操作\n上昇:[U] 下降:[J]\n");
		Print("リセット:[M]\n");
		Print("\nテクスチャインデックス:%d\n後退:[V] 前進:[B]\n", nTexIdx);
		Print("テクスチャの分割:      [1] %s\n", m_apOnOff[(int)bTexCut]);
		Print("ワイヤーフレームの表示:[2] %s\n", m_apOnOff[(int)bWireFrame]);
		break;

	case CExe::EDITMODE_PREVIEW:

	
		break;
	}

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