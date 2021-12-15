//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

// 前方宣言
class CPolygon;

// マクロ定義
#define FEEDBACK_NUM		(2)			// フィードバックエフェクトに使用するポインタの数
#define FEEDBACK_LENGTH		(15.0f)		// エフェクト発生時にポリゴンが拡縮する長さ
#define FEEDBACK_FRAMECNT	(15)		// フィードバックエフェクトのフレーム数
#define DEFAULT_ALPHA		(0.1f)		// 初期α値
#define FEEDBACK_ALPHA		(1.0f)		// エフェクト発生時のポリゴンα値


// レンダリングクラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	//void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	//LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT; }

	// フィードバックエフェクトの発生
	void SetFeedbackEffect(void);

	float GetAlpha(void) { return m_fAlpha; }
	float GetLength(void) { return m_fLength; }

private:
	void DrawFPS(void);

	LPDIRECT3D9 m_pD3D = NULL;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
	LPD3DXFONT m_pFont = NULL;					// フォントへのポインタ

	LPDIRECT3DTEXTURE9 m_apTextureMT[FEEDBACK_NUM];	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[FEEDBACK_NUM];	// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_apBuffMT[FEEDBACK_NUM];	// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;						// テクスチャレンダリング用ビューポート

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;			// フィードバック用ポリゴン頂点バッファ
	float m_fLength;
	float m_fAngle;
	float m_fAlpha;									// α値
	int m_nAppealCnt;								// エフェクト強調に使うフレームカウント
	bool m_bAppeal;									// アピール状態
													//CPolygon *m_pPolygon;							// ポリゴンポインタ
};

#endif