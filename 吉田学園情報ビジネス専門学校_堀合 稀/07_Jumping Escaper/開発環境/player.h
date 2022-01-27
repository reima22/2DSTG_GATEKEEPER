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
class CModel;			// モデルクラス
class CShadow;			// 影クラス
class CShadowS;			// ステンシルシャドウ
class CMotionPlayer;	// プレイヤーのモーションクラス

// マクロ定義
#define KEYNUM_MAX			(10)		// キーの最大数
#define MOVE_MOTION			(0.1f)		// 移動モーションを行う速度基準
#define RUN_MOTION			(2.0f)		// 走るモーションを行う速度基準
#define SPEED_DOWN			(0.2f)		// 減衰係数
#define GRAVITY_SCALE		(0.6f)		// 重力
#define WALK_SPEED			(0.5f)		// 歩く速度
#define RUN_SPEED			(1.0f)		// 走る速度
#define SPEED_RANK_CNT		(60)		// 速度ランクカウント
#define AIR_CONTROL			(30)		// 空中で操作可能な期間カウント
#define FALL_RETURN			(1000.0f)	// 落下により戻る
#define JUMP_LIMIT_FRAME	(10)		// ジャンプ可能フレームカウント
#define JUMP_CNT_MAX		(9)			// ジャンプカウント最大値
#define RUN_CNT_DOWN		(10)		// ダッシュカウントダウン値

//==============================================================================
// プレイヤー管理クラス
//==============================================================================
class CPlayer : public CScene3D
{
public:
	typedef enum
	{
		RESETPHASE_NONE = 0,	// 未実行
		RESETPHASE_WAIT,		// 待機状態
		RESETPHASE_ON,			// リセット実行
		RESETPHASE_MAX
	}RESETPHASE;

	CPlayer();
	~CPlayer();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(void);

	// 移動処理
	void PlayerMove(void);
	void PlayerControl(void);

	void PlayerWalk(void);			// 歩行処理
	void PlayerRun(void);			// 走行処理

	// 当たり判定
	void PlayerCollision(void);

	// 復活処理
	void PlayerReset(void);

	// クリア時のモーション設定
	void ClearMotion(void);

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// 向きの取得・設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// 過去の位置の取得
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	// 着地状態の取得・設定
	bool GetLandObj(void) { return m_bLandObj; }
	void SetLandObj(bool bLand) { m_bLandObj = bLand; }
	bool GetLandMesh(void) { return m_bLandMesh; }
	void SetLandMesh(bool bLand) { m_bLandMesh = bLand; }

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

	// リスポーン地点の取得
	D3DXVECTOR3 GetPosSave(void) { return m_posSave; }

	// モーション情報
	CMotionPlayer *GetMotionPlayer(void) { return m_pMotionPlayer; }

	// ゲーム終了フラグ
	bool GetGameClear(void) { return m_bSetGameClear; }
	void SetGameClear(bool bSetGameClear) { m_bSetGameClear = bSetGameClear; }

	bool GetGameOver(void) { return m_bSetGameOver; }
	void SetGameOver(bool bSetGameOver) { m_bSetGameOver = bSetGameOver; }

	// リセット状態の取得
	RESETPHASE GetResetPhase(void) { return m_resetPhase; }

	// リセット状態の切り替え
	void ResetOn(void)
	{ if (m_resetPhase == RESETPHASE_NONE) 
		m_resetPhase = RESETPHASE_WAIT; 
		m_pos = m_posNeutral;
	}

	// ジャンプ状態の取得・設定
	bool GetJump(void) { return m_bJump; }
	void SetJump(bool bJump) { m_bJump = bJump; }

private:
	// モデル情報
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き

	D3DXVECTOR3 m_posNeutral;			// 初期位置

	D3DXVECTOR2 m_shadowSize;			// 影の大きさ
	D3DXVECTOR3 m_move;					// 移動力
	D3DXVECTOR3 m_posOld;				// 過去の位置
	D3DXVECTOR3 m_rotDest;				// 過去の向き

	float m_fRadius;					// 当たり判定半径
	float m_fHeight;					// 当たり判定高さ

	bool m_bJump;						// ジャンプしている(滞空状態)
	
	int m_nJumpLimit;					// ジャンプ可能限界カウント

	int m_nCntAir;						// 滞空カウント
	bool m_bAirControl;					// 空中制御フラグ
	
	bool m_bLanding;					// 着地している判定
	bool m_bLandObj;					// オブジェクトに着地している
	bool m_bLandMesh;					// メッシュの着地している

	int m_nJumpCnt;						// ジャンプボタン長押しカウント

	int m_nDashCnt;						// ダッシュしている時のカウント
	float m_fDashPower;					// ダッシュ力

	D3DXVECTOR3 m_posSave;				// 中間地点座標

	CShadowS *m_pShadowS;				// ステンシルシャドウポインタ

	// モーション情報
	CMotionPlayer *m_pMotionPlayer;		// モーション情報
	bool m_bSetGameClear;				// ゲームクリア判定
	bool m_bSetGameOver;				// ゲームオーバー判定

	int m_nCntSandEffect;				// 砂煙エフェクトの発生カウント

	RESETPHASE m_resetPhase;			// リセットフェーズ
	int m_nCntReset;					// リセットフェーズのカウント

	D3DXVECTOR3 m_posLava;				// 溶岩落下位置
	bool m_bIntoLava;					// 溶岩へ落下
};
#endif