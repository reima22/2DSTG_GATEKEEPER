#pragma once
//=============================================================================
//
// テキストデータ（ランキング選択）管理 [textdata_rankingselect.h]
// Author : Tatasuki Matsuda
//
//============================================================================= 
#ifndef _TEXTDATA_RANINGSELECT_H_
#define _TEXTDATA_RANINGSELECT_H_

#include "main.h"
#include "textdata.h"

//==============================================================================
// テキスト管理（ランキング選択）クラス
//==============================================================================
class CTextDataRankingSelect : public CTextData
{
public:
	// 選択ポリゴン一覧
	typedef enum
	{
		RANKINGPOLY_RETRY = 0,	// リトライ
		RANKINGPOLY_STAGE,		// ステージ
		RANKINGPOLY_QUIT,		// 終了	
		RANKINGPOLY_MAX
	}RANKINGPOLY;

	CTextDataRankingSelect();
	~CTextDataRankingSelect();

	void LoadData(void);
	void Unload(void);

	static CTextDataRankingSelect *Create(void);

	int GetTexIdx(int nIdx) { return m_aIdxTex[nIdx]; }			// テクスチャインデックス取得
	D3DXVECTOR3 GetPosition(int nIdx) { return m_aPos[nIdx]; }	// 位置の取得
	D3DXVECTOR2 GetSize(int nIdx) { return m_aSize[nIdx]; }		// サイズの取得

private:
	D3DXVECTOR3 m_aPos[RANKINGPOLY_MAX];	// 各ポリゴンの配置
	D3DXVECTOR2 m_aSize[RANKINGPOLY_MAX];	// 各ポリゴンのサイズ
	int m_aIdxTex[RANKINGPOLY_MAX];			// テクスチャのインデックス
};
#endif

