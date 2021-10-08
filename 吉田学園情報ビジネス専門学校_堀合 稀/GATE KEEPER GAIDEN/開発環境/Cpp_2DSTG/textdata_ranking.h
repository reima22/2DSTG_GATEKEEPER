#pragma once
//=============================================================================
//
// テキストデータ（ランキング）管理 [textdata_ranking.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_RANING_H_
#define _TEXTDATA_RANING_H_

#include "main.h"
#include "ranking.h"
#include "textdata.h"

//==============================================================================
// テキスト管理（ランキング）クラス
//==============================================================================
class CTextDataRanking : public CTextData
{
public:
	CTextDataRanking();
	~CTextDataRanking();

	static void LoadData(void);
	static void SaveData(void);

	static int GetData(int nRankNum) { return m_aRankingData[nRankNum]; }

private:
	static int m_aRankingData[RANK_NUM];
};

#endif
