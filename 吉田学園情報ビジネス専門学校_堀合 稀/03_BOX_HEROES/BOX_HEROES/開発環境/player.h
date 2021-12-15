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
#define MODEL_PARTS		(20)	// モデルパーツの最大数
#define MODEL_TEX_NUM	(8)		// モデルのテクスチャ数
#define MAX_KEY			(8)		// キー数の最大数
#define FRAME_NUM		(50)	// フレームの最大数
#define MBF_NEU_MOVE	(3)		// モーションブレンドフレーム：ニュートラルから移動
#define MBF_MOVE_NEU	(15)	// モーションブレンドフレーム：移動からニュートラル
#define MBF_NEU_ACT		(15)	// モーションブレンドフレーム：ニュートラルからアクション
#define MBF_ACT_NEU		(40)	// モーションブレンドフレーム：アクションからニュートラル
#define MBF_LND_NEU		(20)	// モーションブレンドフレーム：着地からニュートラル

// プレイヤーのモデル種類
typedef enum
{
	MODELTYPE_0 = 0,	// バランス
	MODELTYPE_1,		// ジャンプ
	MODELTYPE_2,		// スピード
	MODELTYPE_MAX
}ModelType;

//==============================================================================
// プレイヤーの数
//==============================================================================
typedef enum
{
	PLAYERNUM_0 = 0,
	PLAYERNUM_1,
	PLAYERNUM_MAX
}PLAYERNUM;

//==============================================================================
// プレイヤーの種類
//==============================================================================
typedef enum
{
	P_TYPE_P1 = 0,	// 1P
	P_TYPE_P2,		// 2P
	P_TYPE_CPU,		// CPU
	P_TYPE_MAX
}P_TYPE;

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

// 近接攻撃の種類
typedef enum
{
	SHORT_NONE = 0,		// 素手
	SHORT_KNIFE,		// ナイフ
	SHORT_MAX	
}ShortRange;

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
	MOTIONTYPE_SHOT,		// 射撃モーション
	MOTIONTYPE_RANCHER,		// ランチャー射撃モーション
	MOTIONTYPE_SLASH,		// 斬撃モーション
	MOTIONTYPE_VICTORY,		// 勝利モーション
	//MOTIONTYPE_VICTORY1,	// 勝利モーション1
	//MOTIONTYPE_VICTORY2,	// 勝利モーション2
	MOTIONTYPE_MAX			// モーションの最大数
} MOTIONTYPE;

//==============================================================================
// 武器の保有状態
//==============================================================================
typedef enum
{
	WEAPON_NONE = 0,	// 素手
	WEAPON_KNIFE,		// ナイフ
	//WEAPON_SWORD,		// 剣
	WEAPON_HANDGUN,		// ハンドガン
	WEAPON_MACHINEGUN,	// マシンガン
	WEAPON_RUNCHER,		// ロケットランチャー
	WEAPON_MAX
}Weapon;

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
	int nBrendFrame;		// モーションブレンド用再生フレーム
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
	LPD3DXMESH pMeshModel;								// メッシュへのポインタ
	LPD3DXBUFFER pBuffMatModel;							// マテリアルへのポインタ
	DWORD nNumMatModel;									// マテリアルの数
	LPDIRECT3DTEXTURE9 pTextureModel[MODEL_TEX_NUM];
	D3DXVECTOR3 pos;									// 位置(オフセット)
	D3DXVECTOR3 posReset;								// モデルの初期位置
	D3DXVECTOR3 rot;									// 向き
	D3DXMATRIX mtxWorld;								// ワールドマトリックス
	int nIdxModelParent;								// 親モデルのインデックス
	char aFileName[128];								// 読み込むXデータ名
	D3DXVECTOR3 posMemo;								// 保存用位置情報
	D3DXVECTOR3 rotMemo;								// 保存用角度情報

	LPD3DXBUFFER pBuffMatShadow;	// マテリアルへのポインタ
	bool bUse;
}Model;

//==============================================================================
// 近接攻撃の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	D3DXMATRIX mtxWorld;			// 当たり判定のマトリックス
	int nParts;						// 発生するパーツ番号
	//D3DXVECTOR3 AttackCollision;	// 攻撃の当たり判定位置
	float fRadius;					// 判定半径
	int nStartFrame;				// 開始フレーム
	int nEndFrame;					// 終了フレーム
	int nCntFrameTotal;				// 攻撃フレーム数
	bool bUse;						// 攻撃の有効化
	float fGetDamage[4];
} Attack;


// 攻撃球体
typedef struct
{
	int nParts;
	int nStartFrame;
	int nEndFrame;
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	D3DXCOLOR col;			// 色		
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	LPD3DXMESH pMeshSphere;
	LPD3DXBUFFER pBuffMatSphere;	// マテリアルへのポインタ
	float fRadius;
	int nSlice;
	int nStack;
	int nCntFrameTotal;		// 攻撃フレーム数
	bool bUse;				// 使用状態
} AttackSphere;

// 当たり判定の構造体
//typedef struct
//{
//	float fFront;		// 正面
//	float fBack;		// 後方
//	float fLeft;		// 左
//	float fRight;		// 右
//	float fFrontLeft;	// 左前
//	float fFrontRight;	// 右前
//	float fBackLeft;	// 左後方
//	float fBackRight;	// 右後方
//}Collision;

//==============================================================================
// プレイヤーの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 posOld;							// 過去の位置
	D3DXVECTOR3 rot;							// 角度
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 move;							// 移動力
	D3DXVECTOR3 rotDest;						// 過去の角度
	PLAYERSTATE state;							// プレイヤーの状態
	D3DXVECTOR3 vtxMinPlayer, vtxMaxPlayer;		// モデルの各座標の最大値・最小値
	float fLife;								// 体力
	int nIdx;									// 影の割り当て番号
	Model aModel[MODEL_PARTS];					// モデル(パーツ)
	int nNumModel;								// モデル(パーツ)数
	int nNumWeapon;								// モデル(武器)数
	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	// モーション情報
	int nNumMotion = MOTIONTYPE_MAX;			// モーション数
	MOTIONTYPE motionType;						// モーションタイプ
	MOTIONTYPE motionTypeOld;					// 直前のモーションタイプ
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー数
	int nKey;									// キー№
	int nCounterMotion;							// モーションのカウンター
	bool bJump;									// ジャンプしているか
	bool bOnBlock;								// 何かに乗っているか
	int nCntState;								// 状態変化のカウンター
	float fInRadius;							// モデル参照の内半径
	float fOutRadius;							// プレイヤーの外半径
	float fHeight;								// プレイヤーの高さ
	int nJumpCnt;								// ジャンプ力のカウント
	int nMoveCnt;								// 移動モーションカウント
	bool bMotionChange;							// モーション変化時の判定
	int nBrendFrame;							// モーションブレンド時のフレーム数
	P_TYPE p_type;								// 1P、2P、CPUの区別
	int nFrameTotal[MOTIONTYPE_MAX];			// 各モーションの合計フレーム数
	Attack attack;								// 近接攻撃の構造体
	ModelType modelType;
	ShortRange sr;								// 近接攻撃の種類
	AttackSphere as[SHORT_MAX];
	D3DXVECTOR3 aPoint[4];
	D3DXVECTOR3 aVec[4];
	D3DXVECTOR3 posShadow;
	Weapon weapon;
	int nBullet;								// 保有弾数
	float fMove;								// テキスト読み取り用移動力
	float fJump;								// テキスト読み取り用ジャンプ力
} Player;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitPlayer(void);				// プレイヤーの初期化処理
void UninitPlayer(void);				// プレイヤーの終了処理
void UpdatePlayer(void);				// プレイヤーの更新処理
void DrawPlayer(void);					// プレイヤーの描画処理
Player *GetPlayer(void);				// プレイヤーの取得
void UpdateMotion(						// モーションの更新
	MOTIONTYPE motionType ,				
	int nCntPlayer);					
void LoadMotion(void);					// .txtフォルダの読み込み
void StateChange(int nCnt);				// 状態変化
void MovePlayer(int nCnt);				// プレイヤーの操作
void CollisionDamage(int nCnt,int nTarget, D3DXVECTOR3 pos);
void AttackParts(int nCnt);
void SetPosShadow(int nPlayer,D3DXVECTOR3 pos);
void WeaponSet(int nCntPlayer, Weapon weapon);		// 武器の装備
void WeaponAction(int nCntPlayer, Weapon weapon);	// 武器ごとのアクション管理
bool CollisionBullet(D3DXVECTOR3 pos ,int nType,int nPlayer);

#endif