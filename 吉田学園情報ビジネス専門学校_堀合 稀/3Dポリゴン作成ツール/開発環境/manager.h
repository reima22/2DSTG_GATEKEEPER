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
class CCamera;			// カメラクラス
class CLight;			// ライトクラス
class CTextData;		// テキストデータクラス
class CTexture;			// テクスチャクラス
class CExe;				// 実行クラス

//=============================================================================
// マネージャクラス
//=============================================================================
class CManager
{
public:
	CManager();
	 ~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// クラスポインタの取得
	static CRenderer *GetRenderer(void) { return m_pRenderer; };				// レンダラーポインタの取得
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }	// キーボードの取得
	static CCamera *GetCamera(void) { return m_pCamera; }						// カメラポインタの取得	
	static CTexture *GetTexture(void) { return m_pTexture; }					// テクスチャポインタの取得
	static CTextData *GetTextData(void) { return m_pTextData; }

private:
	static CRenderer* m_pRenderer;				// レンダリングクラス
	static CInputKeyboard *m_pInputKeyboard;	// キーボードクラス
	static CCamera *m_pCamera;					// カメラのポインタ
	static CLight *m_pLight;					// ライトのポインタ
	static CTextData *m_pTextData;				// テキストデータクラスのポインタ
	static CTexture *m_pTexture;				// テクスチャクラスのポインタ

	static CExe *m_pExe;						// 実行クラス
};

#endif