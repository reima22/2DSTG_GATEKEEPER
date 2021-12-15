//=============================================================================
//
// マネージャ [manager.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "sound.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;		// レンダラークラス
class CInputKeyboard;	// キーボードクラス
class CGamepad;			// ゲームパッドクラス
class CTitle;			// タイトルクラス
class CTutorial;		// チュートリアルクラス
class CGame;			// ゲームクラス
class CResult;			// リザルトクラス
class CRanking;			// ランキングクラス
class CCamera;			// カメラクラス
class CLight;			// ライトクラス
class CTexture;			// テクスチャクラス

//=============================================================================
// マネージャクラス
//=============================================================================
class CManager
{
public:
	// ゲームモードの列挙
	typedef enum
	{
		MODE_TITLE = 0,	// タイトル
		MODE_TUTORIAL,	// チュートリアル
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_RANKING,	// ランキング
		MODE_MAX
	}MODE;

	CManager();
	 ~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void) { return m_pRenderer; };
	static CInputKeyboard *GetInputKeyboard(void);
	static CGamepad *GetInputGamepad(void);

	static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; }

	// BGMの再生設定
	static bool GetBGMPlay(CSound::BGMTYPE type) { return m_bBGM[type]; }

	// カメラポインタの取得
	static CCamera *GetCamera(void) { return m_pCamera; }

	// テクスチャポインタの取得
	static CTexture *GetTexture(void) { return m_pTexture; }

private:
	static CRenderer* m_pRenderer;				// レンダリングクラス
	static CInputKeyboard *m_pInputKeyboard;	// キーボードクラス
	static CGamepad *m_pGamepad;				// ゲームパッドクラス

	int m_nCntEnemyPop;							// 敵出現カウント

	static CTitle *m_pTitle;		// タイトルポインタ
	static CTutorial *m_pTutorial;	// チュートリアルポインタ
	static CGame *m_pGame;			// ゲームポインタ
	static CResult *m_pResult;		// リザルトポインタ
	static CRanking *m_pRanking;	// ランキングポインタ
	static MODE m_mode;				// シーンモード

	static CCamera *m_pCamera;		// カメラのポインタ
	static CLight *m_pLight;		// ライトのポインタ

	static CTexture *m_pTexture;	// テクスチャのポインタ

	static bool m_bBGM[CSound::BGMTYPE_MAX];
};

#endif