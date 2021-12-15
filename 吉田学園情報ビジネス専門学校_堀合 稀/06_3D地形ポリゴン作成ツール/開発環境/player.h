//=============================================================================
//
// プレイヤー管理 [player.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

// 前方宣言
class CModel;	// モデルクラス
class CShadow;	// 影クラス
class CShadowS;	// ステンシルシャドウ
class CMotionPlayer;

// マクロ定義
#define KEYNUM_MAX		(10)		// キーの最大数
#define MOVE_MOTION		(0.1f)		// 移動モーションを行う速度基準
#define RUN_MOTION		(2.0f)		// 走るモーションを行う速度基準
#define SPEED_DOWN		(0.2f)		// 減衰係数
#define GRAVITY_SCALE	(0.6f)		// 重力
#define WALK_SPEED		(0.5f)		// 歩く速度
#define RUN_SPEED		(1.0f)		// 走る速度
#define SPEED_RANK_CNT	(60)		// 速度ランクカウント

//==============================================================================
// プレイヤー管理クラス
//==============================================================================
class CPlayer : public CScene3D
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(void);

	// 移動処理
	void MovePlayer(void);

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// 向きの取得・設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// 過去の位置の取得
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	// 着地状態の取得・設定
	bool GetLanding(void) { return m_bLand; }
	void SetLanding(bool bLand) { m_bLand = bLand; }

	// 過去の角度取得
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }

	// 移動力の取得
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	// 当たり判定半径の取得
	float GetRadius(void) { return m_fRadius; }

	// 当たり判定高さの取得
	float GetHeight(void) { return m_fHeight; }

	// ダッシュカウントの設定
	void SetDashCnt(int nCnt) { m_nDashCnt = nCnt; }

protected:
	// モデル情報
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_move;					// 移動力
	bool m_bLand;						// 着地している

private:
	D3DXVECTOR2 m_shadowSize;			// 影の大きさ
	D3DXVECTOR3 m_posOld;				// 過去の位置
	D3DXVECTOR3 m_rotDest;				// 過去の向き

	float m_fRadius;					// 当たり判定半径
	float m_fHeight;					// 当たり判定高さ
	bool m_bJumpAir;					// 空中ジャンプ制限
	bool m_bJump;						// ジャンプしている(滞空状態)
	int m_nJumpCnt;						// ジャンプボタン長押しカウント

	int m_nDashCnt;						// ダッシュしている時のカウント
	float m_fDashPower;					// ダッシュ力

	CShadowS *m_pShadowS;				// ステンシルシャドウポインタ

	// モーション情報
	CMotionPlayer *m_pMotionPlayer;		// モーション情報
};
#endif
