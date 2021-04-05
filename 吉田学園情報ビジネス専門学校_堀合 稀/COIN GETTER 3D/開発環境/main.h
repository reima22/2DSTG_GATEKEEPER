//==============================================================================
//
// ウィンドウの表示(main.h)
// Author : Mare Horiai
//
//==============================================================================
#pragma once
#include <windows.h>
#include "d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION	(0x0800)
#include "dinput.h"						// 入力処理に必要
#include "xaudio2.h"					// サウンド再生に必要 

//==============================================================================
// ライブラリのリンク
//==============================================================================
#pragma comment(lib, "d3d9.lib")	// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")	// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// コンポーネント(部品)使用に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "winmm.lib")

//==============================================================================
// マクロ定義
//==============================================================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SCREEN_WIDTH	(1280)		// 画面の幅
#define SCREEN_HEIGHT	(720)		// 画面の高さ
#define SPEEDDOWN		(0.2f)		// 減衰係数
#define SCREEN_CENTERX	(640)		// 画面の中央X軸
#define SCREEN_CENTERY	(360)		// 画面の中央Y軸
#define FOUR_POINT		(4)			// 当たり判定の4個点
#define PLUS_GRAVITY	(0.6f)		// 加算される重力値
#define MAX_LIFE		(5)			// 体力の最大値
#define TEXT_LENGTH		(128)		// テキスト読み込みの最大数
#define DATA_LENGTH		(64)		// データ読み込みの最大数
#define VTX_MIN			(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAX			(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値
#define TITLE_BGM		(true)		// タイトルBGMのオン・オフ
#define GAME_BGM		(true)		// ゲームBGMのオン・オフ
#define RESULT_BGM		(true)		// リザルトBGMのオン・オフ
#define RANKING_BGM		(true)		// ランキングBGMのオン・オフ

//==============================================================================
// 頂点フォーマットに合わせた構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 1.0で固定
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

//==============================================================================
// モードの種類
//==============================================================================
typedef enum
{
	MODE_TITLE = 0,	// タイトル画面
	MODE_TUTORIAL,	// チュートリアル画面
	MODE_GAME,		// ゲーム画面
	MODE_RESULT,	// リザルト画面
	MODE_RANKING,	// ランキング画面
	MODE_MAX
}MODE;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
void DrawT(void);