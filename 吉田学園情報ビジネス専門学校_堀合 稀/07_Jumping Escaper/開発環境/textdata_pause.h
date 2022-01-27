//=============================================================================
//
// テキストデータ（ポーズ）管理 [textdata_pause.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_PAUSE_H_
#define _TEXTDATA_PAUSE_H_

#include "main.h"
#include "textdata.h"

//==============================================================================
// テキスト管理（ポーズ）クラス
//==============================================================================
class CTextDataPause : public CTextData
{
public:
	// ポーズポリゴン一覧
	typedef enum
	{
		PAUSEPOLY_BASE = 0,	// ポーズウィンドウ
		PAUSEPOLY_CONTINUE,	// コンテニュー
		PAUSEPOLY_RETRY,	// リトライ
		PAUSEPOLY_QUIT,		// 終了
		PAUSEPOLY_MAX
	}PAUSEPOLY;

	CTextDataPause();
	~CTextDataPause();

	void LoadData(void);
	void Unload(void);

	static CTextDataPause *Create(void);

	int GetTexIdx(int nIdx) { return m_aIdxTex[nIdx]; }			// テクスチャインデックス取得
	D3DXVECTOR3 GetPosition(int nIdx) { return m_aPos[nIdx]; }	// 位置の取得
	D3DXVECTOR2 GetSize(int nIdx) { return m_aSize[nIdx]; }		// サイズの取得

private:
	D3DXVECTOR3 m_aPos[PAUSEPOLY_MAX];	// 各ポリゴンの配置
	D3DXVECTOR2 m_aSize[PAUSEPOLY_MAX];	// 各ポリゴンのサイズ
	int m_aIdxTex[PAUSEPOLY_MAX];		// テクスチャのインデックス
};
#endif
