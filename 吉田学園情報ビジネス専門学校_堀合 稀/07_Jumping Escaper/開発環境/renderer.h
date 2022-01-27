//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

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

	static CRenderer *Create(HWND hWnd, BOOL bWindow);					// 生成処理
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void SetBSColor(D3DXCOLOR col);								// 大元の背景色の設定

private:
	void DrawFPS(void);

	LPDIRECT3D9 m_pD3D = NULL;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
	LPD3DXFONT m_pFont = NULL;					// フォントへのポインタ

	D3DXCOLOR m_colBackScreen;			// 背景のカラー
};

#endif