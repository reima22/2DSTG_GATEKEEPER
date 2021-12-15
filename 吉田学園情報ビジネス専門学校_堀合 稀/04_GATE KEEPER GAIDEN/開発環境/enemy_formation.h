//=============================================================================
//
// 敵編隊生成処理 [enemy_formation.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ENEMY_FORMATION_H_
#define _ENEMY_FORMATION_H_

#include "main.h"
#include "character.h"
#include "enemy.h"

//=============================================================================
// 編隊生成クラス
//=============================================================================
class CEnemyFormation : public CEnemy
{
public:
	// 編成パターン
	typedef enum
	{
		TEAM_PATTERN_RANDOM = 0,	// ランダム配置
		TEAM_PATTERN_SIDE,			// 両サイドに出現
		TEAM_PATTERN_UNDER,			// 下を通る
		TEAM_PATTERN_MOTHER_0,		// 母艦からの配置
		TEAM_PATTERN_MOTHER_1,
		TEAM_PATTERN_MOTHER_2,
		TEAM_PATTERN_MOTHER_PARTS,	// 母艦パーツ左右から
		TEAM_PATTERN_MAX
	}TEAM_PATTERN;

	CEnemyFormation();
	~CEnemyFormation();

	static void SetFormationEnemy(		// 隊列編成配置処理	
		D3DXVECTOR3 pos,
		OBJTYPE objType,
		CTexture::TEXTYPE type,
		D3DXVECTOR3 moveRot,
		float fMove,
		int nSetNum,
		int nInterval,
		TEAM_PATTERN team);

	static void SetFormRandom(TEAM_PATTERN team);					// 隊列編成ランダム配置処理
	static void FormInterval(int nIntervalCnt);						// インターバルのカウント
	static bool GetIntervalBool(void) { return m_bFormSetInt; }

	static void SetInterval(int nInterval) { m_nInterval = nInterval; }

	// 一時中断の取得
	static bool GetLock(void) { return m_bLock; }

	static void SetParamReset(void) { m_bSetParam = false; }

private:
	static int m_nFormNumInterval;		// 編成隊内の個別敵毎の生成カウント
	static int m_nFormSetInterval;		// 編隊生成のインターバル
	static bool m_bFormSetInt;			// インターバル判定

	static D3DXVECTOR3 m_pos;			// 配置する位置
	static D3DXVECTOR3 m_moveRot;		// 移動方向
	static float m_fMove;				// 移動力(基本1.0f)
	static CTexture::TEXTYPE m_type;	// 種類
	static int m_nInterval;				// 一体毎に生成する間隔
	static bool m_bSetParam;			// 生成パラメータ設定判定
	static bool m_bLock;				// 生成を一時中断
};

#endif
