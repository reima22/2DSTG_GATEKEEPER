//=============================================================================
//
// モーション管理 [motion.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

// 前方宣言
class CModel;	// モデルクラス
//class CShadow;	// 影クラス
//class CShadowS;	// ステンシルシャドウ

// マクロ定義
#define MOVE_MOTION		(0.1f)		// 移動モーションを行う速度基準
#define RUN_MOTION		(2.0f)		// 走るモーションを行う速度基準

//==============================================================================
// モーション管理クラス
//==============================================================================
class CMotion
{
public:
	// キー要素
	typedef enum
	{
		KEY_POS_X = 0,	// posX
		KEY_POS_Y,		// posY
		KEY_POS_Z,		// posZ
		KEY_ROT_X,		// rotX
		KEY_ROT_Y,		// rotY
		KEY_ROT_Z,		// rotZ
		KEY_MAX
	}KEY;

	CMotion();
	~CMotion();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;

	// フレームカウントの取得・設定
	int GetFrameCnt(void) { return m_nFrameCnt; }
	void SetFrameCnt(int nCnt) { m_nFrameCnt = nCnt; }

	// キーカウントの取得・設定
	int GetKeyCnt(void) { return m_nKeyCnt; }
	void SetKeyCnt(int nCnt) { m_nKeyCnt = nCnt; }

protected:
	// モデル情報
	int m_nNumParts;			// パーツ数

	CModel **m_apModel;			// モデルへのポインタ
	int *m_aIndexParent;		// 親モデルのインデックス
	float **m_aOffset;			// 親モデルからのオフセット

	// モーション情報
	float ****m_aKey;			// キー要素
	int *m_nNumKey;				// キーの数
	bool m_bLoop;				// ループするか
	int **m_pFrame;				// フレーム数

	D3DXVECTOR3 *m_posMemo;		// ブレンド保存用位置情報
	D3DXVECTOR3 *m_rotMemo;		// ブレンド保存用向き情報

	bool m_bBlendMotion;		// モーションブレンドが起きたか

	int m_nKeyCnt;				// キーのカウンタ
	int m_nFrameCnt;			// フレームの
};
#endif
