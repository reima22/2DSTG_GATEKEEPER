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
#include "xinput.h" 

//==============================================================================
// ライブラリのリンク
//==============================================================================
#pragma comment(lib, "d3d9.lib")	// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")	// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// コンポーネント(部品)使用に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xinput.lib")

//==============================================================================
// マクロ定義
//==============================================================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット
#define SCREEN_WIDTH	(1280)		// 画面の幅
#define SCREEN_HEIGHT	(720)		// 画面の高さ
#define SCREEN_CENTERX	(640)		// 画面の中央X軸
#define SCREEN_CENTERY	(360)		// 画面の中央Y軸
#define POS1_PLAYER		(D3DXVECTOR3(40.0f,300.0f,0.0f))	// ステージ1のプレイヤーのスタート位置
#define POS2_PLAYER		(D3DXVECTOR3(40.0f,660.0f,0.0f))	// ステージ2のプレイヤーのスタート位置
#define POS3_PLAYER		(D3DXVECTOR3(50.0f,660.0f,0.0f))	// ステージ3のプレイヤーのスタート位置
#define MAX_LIFE		(3)			// 体力の最大値
#define SPEEDUP			(1.0f)		// 移動の加速度
#define JUMP_POWER		(-20.0f)	// ジャンプ力s
#define DROP_SPEEDUP	(1.4f)		// 落下の加速度
#define MAX_GRAVITY		(30.0f)		// 重力の最大値
#define GRAVITY_ENEMY	(25.0f)		// 敵の重力の最大値
#define SPEEDDOWN		(0.2f)		// 減衰係数
#define ENEMY_SIZEX		(20)		// 敵の幅
#define ENEMY_SIZEY		(40)		// 敵の高さ
#define COIN_STAGE1		(48)		// ステージ1のコイン数
#define COIN_STAGE2		(70)		// ステージ2のコイン数
#define COIN_STAGE3		(54)		// ステージ3のコイン数
#define BONUS_STAGE1	(150000)	// ステージ1のノーダメージボーナス
#define BONUS_STAGE2	(100000)	// ステージ2のノーダメージボーナス
#define BONUS_STAGE3	(300000)	// ステージ3のノーダメージボーナス

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