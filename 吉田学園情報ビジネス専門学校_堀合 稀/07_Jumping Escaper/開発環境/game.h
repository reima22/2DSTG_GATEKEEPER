
//=============================================================================
//
// ゲームモード処理 [game.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "mode.h"

// 前方宣言
class CBonus;			// ボーナスクラス
//class CCheckpointLogo;	// チェックポイントロゴクラス
class CGamesetLogo;		// ゲーム結果ロゴ
//class CFall;			// 落下数クラス
class CFog;				// フォグ効果クラス
class CMeshfield;		// メッシュフィールドクラス
class CObject;			// オブジェクトクラス
class CPause;			// ポーズクラス
class CPlayer;			// プレイヤークラス
//class CStartCntdown;	// 開始時カウントダウンクラス
class CTerrain;			// メッシュ
//class CTimer;			// タイマークラス
class CUi;				// UIクラス

//==============================================================================
// ゲームモードクラス
//==============================================================================
class CGame : public CMode
{
public:
	// ゲームクリアフェイズ
	typedef enum
	{
		CLEARPHASE_NONE = 0,	// クリア判定なし
		CLEARPHASE_WAIT,		// 待機
		CLEARPHASE_PAUSE,		// クリアポーズ演出
		CLEARPHASE_FADE,		// フェードアウト
		CLEARPHASE_MAX
	}CLEARPHASE;

	// ゲームオーバーフェイズ
	typedef enum
	{
		GAMEOVERPHASE_NONE = 0,		// 判定無し
		GAMEOVERPHASE_WAIT,			// 待機
		GAMEOVERPHASE_DIRECTION,	// 演出
		GAMEOVERPHASE_FADE,			// フェードアウト
		GAMEOVERPHASE_MAX
	}GAMEOVERPHASE;

	CGame(int nPriority = PRIORITY_DEFAULT);
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGame *Create(void);

	void SetStageObject(void);	// ステージオブジェクトの設定

	void EndGame(void);		// ゲーム終了
	void GameClear(void);	// ゲームクリア処理
	void GameOver(void);	// ゲームオーバー処理

	static CPlayer *GetPlayer(void) { return m_pPlayer; }						// プレイヤーポインタ
	static CTerrain *GetTerrain(void) { return m_pTerrain; }					// 死亡判定メッシュポインタ
	static CObject *GetPointObject(int nCnt) { return m_ppObject[nCnt]; }		// オブジェクトポインタ
	static CMeshfield *GetMeshField(int nIdx) { return m_ppMeshField[nIdx]; }	// メッシュフィールドポインタ
	static CFog *GetFog(void) { return m_pFog; }								// フォグ取得
	static CUi *GetUi(void) { return m_pUi; }									// UIの取得

	// ゲーム終了フラグの取得・設定
	static bool GetGameEnd(void) { return m_bGameEnd; }
	static void SetGameEnd(bool bGameEnd) { m_bGameEnd = bGameEnd; }

	// ゲームクリアのフェイズの取得・設定
	static CLEARPHASE GetClearPhase(void) { return m_clearPhase; }
	static void SetClearPhase(CLEARPHASE clearPhase) { m_clearPhase = clearPhase; }

	// ゲームオーバーフェイズの取得・設定
	static GAMEOVERPHASE GetOverPhase(void) { return m_gameoverPhase; }
	static void SetOverPhase(GAMEOVERPHASE overPhase) { m_gameoverPhase = overPhase; }

private:
	int m_nNumObject;						// オブジェクト数
	int m_nNumMeshfield;					// メッシュフィールドの数
	static CPause *m_pPause;				// ポーズのポインタ
	static CPlayer *m_pPlayer;				// プレイヤーのポインタ

	static CTerrain *m_pTerrain;			// メッシュ	

	static CObject **m_ppObject;				// オブジェクトのポインタ
	static CMeshfield **m_ppMeshField;		// オリジナルメッシュ

	static CFog *m_pFog;					// フォグ効果クラス

	static bool m_bGameEnd;					// ゲーム終了のフラグ
	static int m_nClearDirectingCnt;		// クリア演出カウント

	static CLEARPHASE m_clearPhase;			// クリア時のフェイズ分割
	static GAMEOVERPHASE m_gameoverPhase;	// ゲームオーバー時のフェーズ分割

	static CGamesetLogo *m_pGamesetLogo;	// ゲーム終了時のロゴポインタ

	static CUi *m_pUi;						// UIのポインタ
};

#endif