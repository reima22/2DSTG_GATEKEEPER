//=============================================================================
//
// テキストデータ（プレイヤー）管理 [textdata_player.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_PLAYER_H_
#define _TEXTDATA_PLAYER_H_

#include "main.h"
#include "player.h"
#include "textdata.h"
#include "motion.h"
#include "motion_player.h"

//==============================================================================
// テキスト管理（プレイヤー）クラス
//==============================================================================
class CTextDataPlayer : public CTextData
{
public:
	// プレイヤーデータの種類
	typedef enum
	{
		
		PLAYERDATA_MAX
	}PLAYERDATA;

	typedef enum
	{
		INFO_POS_X = 0, // posX
		INFO_POS_Y,		// posY
		INFO_POS_Z,		// posZ
		INFO_ROT_X,		// rotX
		INFO_ROT_Y,		// rotY
		INFO_ROT_Z,		// rotZ
		INFOMAX
	}INFO;

	CTextDataPlayer();
	~CTextDataPlayer();

	// テキストの読み込み
	void LoadData(void);
	void Unload(void);
	
	// プレイヤーデータの生成
	static CTextDataPlayer *Create(void);

	// 読み込んだ値の取得
	D3DXVECTOR3 GetPosition(void) { return m_posPlayer; }					// プレイヤーの位置
	D3DXVECTOR3 GetRotation(void) { return m_rotPlayer; }					// プレイヤーの角度
	D3DXVECTOR2 GetShadowSize(void) { return m_shadowSize; }				// 影のサイズ
	char *GetFileName(int nIdx) { return m_pFileModelName[nIdx]; }			// ファイル名
	int GetPartsNum(void) { return m_nNumParts; }							// パーツ数
	int GetIndex(int nIdx) { return m_pIdx[nIdx]; }							// インデックス
	int GetParent(int nIdx) { return m_pParentIdx[nIdx]; }					// 親のインデックス
	float GetOffset(int nIdx, int nInfo) { return m_pOffset[nIdx][nInfo]; }	// オフセット情報
	float GetRadius(void) { return m_fRadius; }								// 当たり判定半径
	float GetHeight(void) { return m_fHeight; }								// 当たり判定高さ

	// モーション情報
	bool GetLoop(CMotionPlayer::MOTIONTYPE motionType) { return m_bLoop[motionType]; }
	int GetNumKey(CMotionPlayer::MOTIONTYPE motionType) { return m_nNumKey[motionType]; }
	
	float GetKeyInfo(CMotionPlayer::MOTIONTYPE motionType,
		int nKey, int nParts, CMotion::KEY keyInfo) { return m_pKey[motionType][nKey][nParts][keyInfo]; }
	
	int GetFrame(CMotionPlayer::MOTIONTYPE motionType,int nKey) { return m_pFrame[motionType][nKey]; }

private:
	int m_nNumModel;					// モデル数
	
	// プレイヤー情報
	D3DXVECTOR3 m_posPlayer;			// プレイヤーの位置
	D3DXVECTOR3 m_rotPlayer;			// プレイヤーの角度
	float m_fRadius;					// 当たり判定周囲半径
	float m_fHeight;					// 当たり判定高さ
	D3DXVECTOR2 m_shadowSize;			// 影の大きさ

	// モデル情報
	char **m_pFileModelName;			// Xファイル名ポインタ
	char **m_pFileModelNameLoad;		// ファイル名変数

	int m_nNumParts;					// パーツ数

	int *m_pIdx;						// インデックス
	int *m_pParentIdx;					// 親のインデックス
	float **m_pOffset;					// オフセット情報

	int m_nModelCnt;					// モデルファイル名カウンタ
	int m_nPartsCnt;					// 読み込んだパーツの数カウンタ

	// モーション情報
	int m_nNumKey[CMotionPlayer::MOTIONTYPE_MAX];		// キー数
	float ***m_pKey[CMotionPlayer::MOTIONTYPE_MAX];	// キー要素
	bool m_bLoop[CMotionPlayer::MOTIONTYPE_MAX];		// モーションがループするか
	int *m_pFrame[CMotionPlayer::MOTIONTYPE_MAX];		// モーションフレーム		
												
	int m_nKeyCnt;								// 読み取るキー数カウント
	int m_nMotionCnt;							// 読み取るモーションカウント
};

#endif
