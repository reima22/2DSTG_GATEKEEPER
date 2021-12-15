
//=============================================================================
//
// ゲームモード処理 [game.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define LIFE_POS		(D3DXVECTOR3(40.0f, 40.0f, 0.0f))		// ライフ配置開始位置
#define SCORE_POS		(D3DXVECTOR3(1100.0f, 690.0f, 0.0f))	// スコア配置開始位置
#define DEFEAT_POS		(D3DXVECTOR3(730.0f, 690.0f, 0.0f))		// 撃破数の配置位置
#define INVASION_POS	(D3DXVECTOR3(870.0f, 690.0f, 0.0f))		// 侵入数の配置位置
#define STOCK_POS		(D3DXVECTOR3(600.0f, 680.0f, 0.0f))		// 残機数の配置位置
#define RESULT_GAMEEND	(D3DXVECTOR3(640.0f, 200.0f, 0.0f))		// ゲーム結果の描写

// 前方宣言
class CCharacter;
class CSceneX;
class CMeshfield;

//==============================================================================
// ゲームモードクラス
//==============================================================================
class CGame : public CScene
{
public:
	CGame();
	~CGame();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CGame *Create(void);
	static CCharacter *GetChara(void) { return m_pCharaPlayer; }
	static int GetEndInvasion(void) { return m_nEndInvasion; }
	static bool GetStartGame(void) { return m_bGameStart; }

	void SetEnemyPattern(int nType);
	static bool GetSetEnemy(void) { return m_bSetEnemy; }		// 敵の配置判定
	static bool GetMotherEnemy(void) { return m_bMotherEnemy; }	// 母艦戦移行時

	void Ending(void);						// ゲーム終了演出
	void CntDownEnd(void);					// ゲーム終了までのカウントダウン

	static CMeshfield *GetMeshfield(void) { return m_pMeshfield; }

private:
	static int m_nClearDefeat;				// ゲームクリアの撃破数
	static int m_nEndInvasion;				// ゲーム終了する侵入数
	static CCharacter *m_pCharaPlayer;		// プレイヤーキャラのポインタ
	static bool m_bGameStart;				// ゲームの開始判定

	static int m_nPhase;					// 経過したフェーズ数
	static int m_nSetType;					// 配置方法の種類
	static bool m_bSetEnemy;				// 敵配置方法の切り替え判定
	bool m_bSetSE;							// SE発生
	bool m_bSetMother;						// 母艦戦BGMの再生判定
	static bool m_bMotherEnemy;				// 母艦フェーズ

	static int m_nCntDownEnd;			// ゲーム終了までのカウント
	bool m_bSetLogo;

	static CMeshfield *m_pMeshfield;

	//static CSceneX *m_pSceneX;			// モデルXのポインタ
};

#endif