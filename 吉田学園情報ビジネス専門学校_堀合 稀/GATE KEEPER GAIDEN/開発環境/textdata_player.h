//=============================================================================
//
// テキストデータ（プレイヤー）管理 [textdata_player.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_PLAYER_H_
#define _TEXTDATA_PLAYER_H_

#include "main.h"
#include "textdata.h"

//==============================================================================
// テキスト管理（プレイヤー）クラス
//==============================================================================
class CTextDataPlayer : public CTextData
{
public:
	// プレイヤーデータの種類
	typedef enum
	{
		PLAYERDATA_STOCK = 0,				// 開始時の残機数
		PLAYERDATA_LIFE_START,				// 開始時のライフ
		PLAYERDATA_LIFE_MAX,				// ライフの最大値
		PLAYERDATA_UNLOCK_BULLET_WAVE,		// 弾取得状態(波形)
		PLAYERDATA_UNLOCK_BULLET_MISSILE,	// 弾取得状態(ミサイル)
		PLAYERDATA_UNLOCK_BULLET_REVERSE,	// 弾取得状態(逆波形)
		PLAYERDATA_MAX
	}PLAYERDATA;

	typedef enum
	{
		UNLOCKDATA_WAVE,		// 弾取得状態(波形)
		UNLOCKDATA_MISSILE,	// 弾取得状態(ミサイル)
		UNLOCKDATA_REVERSE,	// 弾取得状態(逆波形)
		UNLOCKDATA_MAX
	}UNLOCKDATA;

	CTextDataPlayer();
	~CTextDataPlayer();

	static void LoadData(void);
	static int SetData(int nDataNum) { return m_aPlayerData[nDataNum]; }
	static float SetSpeed(void) { return m_fSpeedData; }
	static bool GetUnlockInfo(UNLOCKDATA unlockData) { return m_bSetBullet[unlockData]; }

private:
	static int m_aPlayerData[PLAYERDATA_MAX];
	static float m_fSpeedData;
	static bool m_bSetBullet[UNLOCKDATA_MAX];
};

#endif
