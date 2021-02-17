//==============================================================================
//
// 3Dモデルの描画〔player.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS		(14)	// モデルパーツの最大数
#define MAX_KEY			(8)		// キー数の最大数
#define FRAME_NUM		(50)	// フレームの最大数

//==============================================================================
// プレイヤーの状態
//==============================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	// 出現
	PLAYERSTATE_NORMAL,		// 通常
	PLAYERSTATE_DAMAGE,		// ダメージ
	PLAYERSTATE_WAIT,		// 出現待ち
	PLAYERSTATE_DEATH,		// 死亡
	PLAYERSTATE_CLEAR,		// ゲームクリア
	PLAYERSTATE_GAMEOVER,	// ゲームオーバー
	PLAYERSTATE_MAX
} PLAYERSTATE;

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MOVE,		// 移動モーション
	MOTIONTYPE_ACTION,		// アクションモーション
	MOTIONTYPE_JUMP,		// ジャンプモーション
	MOTIONTYPE_LANDING,		// 着地モーション
	MOTIONTYPE_DAMAGE,		// やられモーション
	MOTIONTYPE_MAX			// モーションの最大数
} MOTIONTYPE;

//==============================================================================
// キーの構造体
//==============================================================================
typedef struct
{
	float fPosX;	// 位置X
	float fPosY;	// 位置Y
	float fPosZ;	// 位置Z
	float fRotX;	// 向きX
	float fRotY;	// 向きY
	float fRotZ;	// 向きZ
} KEY;

//==============================================================================
// キー情報の構造体
//==============================================================================
typedef struct
{
	int nFrame;				// 再生フレーム			
	KEY aKey[MODEL_PARTS];	// 各モデルのキー要素 
} KEY_SET;

//==============================================================================
// モージョン情報の構造体
//==============================================================================
typedef struct
{
	bool bLoop;				// ループするか
	int nNumKey;			// キーの総数
	KEY_SET aKeySet[10];	// キー情報
} MOTION_INFO;

//==============================================================================
// モデルの構造体
//==============================================================================
typedef struct
{
	LPD3DXMESH pMeshModel;		// メッシュへのポインタ
	LPD3DXBUFFER pBuffMatModel;	// マテリアルへのポインタ
	DWORD nNumMatModel;			// マテリアルの数
	D3DXVECTOR3 pos;			// 位置(オフセット)
	D3DXVECTOR3 posReset;		// モデルの初期位置
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	int nIdxModelParent;		// 親モデルのインデックス
	char aFileName[128];		
}Model;

//==============================================================================
// プレイヤーの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	PLAYERSTATE state;
	int nLife;
	int nIdx;
	Model aModel[MODEL_PARTS];					// モデル(パーツ)
	int nNumModel/* = MODEL_PARTS*/;			// モデル(パーツ)数
	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	// モーション情報
	int nNumMotion = MOTIONTYPE_MAX;			// モーション数
	MOTIONTYPE motionType;						// モーションタイプ
	MOTIONTYPE motionTypeOld;					// 直前のモーションタイプ
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー数
	int nKey;									// キー№
	int nCounterMotion;							// モーションのカウンター
} Player;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitPlayer(void);	// プレイヤーの初期化処理
void UninitPlayer(void);	// プレイヤーの終了処理
void UpdatePlayer(void);	// プレイヤーの更新処理
void DrawPlayer(void);		// プレイヤーの描画処理
Player *GetPlayer(void);	// プレイヤーの取得
void UpdateMotion(MOTIONTYPE motionType);	// モーションの更新
void LoadMotion(void);						// .txtフォルダの読み込み

#endif