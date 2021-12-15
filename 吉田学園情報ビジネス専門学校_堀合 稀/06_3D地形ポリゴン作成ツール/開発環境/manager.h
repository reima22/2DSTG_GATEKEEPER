//=============================================================================
//
// マネージャ [manager.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MANAGER_H_
#define _MANAGER_H_

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
class CTextData;		// テキストデータクラス
class CTexture;			// テクスチャクラス
class CCollision;		// 当たり判定クラス
class CExe;				// 実行クラス
class CInputMouse;		// マウス

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

	// クラスの取得
	static CRenderer *GetRenderer(void) { return m_pRenderer; };
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CGamepad *GetInputGamepad(void) { return m_pGamepad; }

	// モード取得・設定
	//static MODE GetMode(void) { return m_mode; }
	//static void SetMode(MODE mode);

	//// 音ポインタの取得
	//static CSound *GetSound(void) { return m_pSound; }

	//// BGMの再生設定
	//static bool GetBGMPlay(CSound::BGMTYPE type) { return m_bBGM[type]; }

	// カメラポインタの取得
	static CCamera *GetCamera(void) { return m_pCamera; }
	
	// テクスチャポインタの取得
	static CTexture *GetTexture(void) { return m_pTexture; }

	// 当たり判定の取得
	static CCollision *GetCollision(void) { return m_pCollision; }

	// 実行モードの取得
	static CExe *GetExe(void) { return m_pExe; }
	
	// マウスの取得
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }

private:
	static CRenderer* m_pRenderer;				// レンダリングクラス
	static CInputKeyboard *m_pInputKeyboard;	// キーボードクラス
	static CGamepad *m_pGamepad;				// ゲームパッドクラス

	static CCamera *m_pCamera;					// カメラのポインタ
	static CLight *m_pLight;					// ライトのポインタ

	static CTextData *m_pTextData;				// テキストデータクラスのポインタ
	static CTexture *m_pTexture;				// テクスチャクラスのポインタ
	static CCollision *m_pCollision;			// 当たり判定クラスのポインタ
	static CExe *m_pExe;						// 実行クラス
	static CInputMouse *m_pInputMouse;			// マウス入力
};

#endif