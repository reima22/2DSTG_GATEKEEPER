//=============================================================================
//
// プレイヤーモーション管理 [motion_player.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MOTION_PLAYER_H_
#define _MOTION_PLAYER_H_

#include "main.h"
#include "motion.h"

//==============================================================================
// プレイヤーモーションクラス
//==============================================================================
class CMotionPlayer : public CMotion
{
public:
	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	// ニュートラル
		MOTIONTYPE_RUN,			// 走行
		MOTIONTYPE_ACTION,		// アクションモーション
		MOTIONTYPE_JUMP,		// ジャンプ
		MOTIONTYPE_LANDING,		// 着地
		MOTIONTYPE_CLEAR,		// クリアポーズ
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	CMotionPlayer();
	~CMotionPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(MOTIONTYPE motionType);	// モーションの更新
	void Draw(void);					// モデル描画

	static CMotionPlayer *Create(void);

	// モーション状態の取得・設定
	MOTIONTYPE GetMotion(void) { return m_motionType; }
	void SetMotion(MOTIONTYPE motionType) { m_motionType = motionType; }

	// フレームカウントの取得・設定
	int GetFrameCnt(void) { return m_nFrameCnt; }
	void SetFrameCnt(int nCnt) { m_nFrameCnt = nCnt; }

	// キーカウントの取得・設定
	int GetKeyCnt(void) { return m_nKeyCnt; }
	void SetKeyCnt(int nCnt) { m_nKeyCnt = nCnt; }

	// 着地状態の取得・設定
	//bool GetLanding(void) { return m_bLand; }
	//void SetLanding(bool bLand) { m_bLand = bLand; }

private:
	// モーション情報
	MOTIONTYPE m_motionType;			// プレイヤーのモーション
	MOTIONTYPE m_motionTypeOld;			// 以前のモーション
};
#endif