//==============================================================================
//
// 敵の描画〔enemy.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ENEMY			(256)		// 敵の最大数
#define MAX_TYPE_ENEMY		(6)			// 敵の種類
#define OPEN_CNT			(160)		// オープニングのカウント上限
#define ENEMY_SIZEX			(20)		// 敵の幅
#define ENEMY_SIZEY			(20)		// 敵の高さ
#define BIGENEMY_SIZEX		(100)		// 大型の敵の幅
#define BIGENEMY_SIZEY		(50)		// 大型の敵の高さ
#define BIGENEMY_APPEAR		(150)		// ボス出現の撃破数

//==============================================================================
// 敵の状態
//==============================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// 通常状態
	ENEMYSTATE_DAMAGE,		// ダメージ状態
	ENEMYSTATE_MAX
} ENEMYSTATE;

//==============================================================================
// ボスの状態
//==============================================================================
typedef enum
{
	BIGENEMY_NORMAL = 0,	// 通常
	BIGENEMY_DEATH,			// 死亡
	BIGENEMY_MAX
} BIGENEMY;

//==============================================================================
// 敵の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nType;			// 種類
	bool bUse;			// 使用しているかどうか
	ENEMYSTATE state;	// 状態
	int nCounterState;	// 状態カウンター
	int nLife;			// 体力
	D3DXVECTOR3 move;	// 移動量
	int nRandAngle;		// 波形移動角度のランダム設定
	float fAngle;		// 波形移動
	int nShotCnt;		// 敵の弾発射カウント
} ENEMY;

//==============================================================================
// 敵全体の詳細情報
//==============================================================================
typedef struct
{
	int nDestroy;		// 敵の撃破数
	int nVanish;		// 敵の消滅数
	int nShotCnt;		// 敵の弾発射カウント
	int nStartCnt;		// オープニングカウント
	int nSetCounter;	// 敵の設定カウント
	int nUfoCnt;		// UFOの設定カウント
	int nUfoSet;		// UFOの設定タイミング
	int nBigEnemyCnt;	// ボスの設定カウント
	BIGENEMY boss;		// ボスの状態
} ENEMYINFO;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitEnemy(void);								// 敵の初期化処理
void UninitEnemy(void);									// 敵の終了処理
void UpdateEnemy(void);									// 敵の更新処理
void DrawEnemy(void);									// 敵の描画処理
void SetEnemy(D3DXVECTOR3 pos, int nType,int nLife);	// 敵の設定
void SetEnemyDes(void);									// 撃破数ごとの敵の配置
void SetVertexEnemy(int nIdx);							// 敵の頂点座標取得
void SetEnemyMove(int nIdx,int nCnt);					// 敵のタイプごとの移動量変化
void SetEnemyMoveDes(int nIdx);							// 撃破数ごとの敵の移動量変化
void SetBigEnemy(void);									// ボス敵の配置
ENEMY *GetEnemy(void);									// 敵の情報
bool HitEnemy(int nIdx, int nDagame);					// 敵の命中とダメージ判定
ENEMYINFO GetInfo(void);								// 敵全体の情報の取得

#endif
