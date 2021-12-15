//=============================================================================
//
// テキストデータ（システム）管理 [textdata_system.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_SYSTEM_H_
#define _TEXTDATA_SYSTEM_H_

#include "main.h"
#include "textdata.h"

//==============================================================================
// テキスト管理（system）クラス
//==============================================================================
class CTextDataSystem : public CTextData
{
public:
	// システムデータの種類
	typedef enum
	{
		SYSTEMDATA_DEFEAT_EASY = 0,			// 撃破規定数イージー
		SYSTEMDATA_DEFEAT_NORMAL,			// 撃破規定数ノーマル
		SYSTEMDATA_DEFEAT_HARD,				// 撃破規定数ハード
		SYSTEMDATA_INVASION_EASY,			// 侵入規定数イージー
		SYSTEMDATA_INVASION_NORMAL,			// 侵入規定数ノーマル
		SYSTEMDATA_INVASION_HARD,			// 侵入規定数ハード
		SYSTEMDATA_HEALITEM_CNT_EASY,		// 回復アイテム出現間隔撃破数イージー
		SYSTEMDATA_HEALITEM_CNT_NORMAL,		// 回復アイテム出現間隔撃破数ノーマル
		SYSTEMDATA_HEALITEM_CNT_HARD,		// 回復アイテム出現間隔撃破数ハード
		SYSTEMDATA_MAX
	}SYSTEM;

	CTextDataSystem();
	~CTextDataSystem();

	static void LoadData(void);

	static int SetData(int nDataNum) { return m_aSystemData[nDataNum]; }

private:
	static int m_aSystemData[SYSTEMDATA_MAX];
};

#endif