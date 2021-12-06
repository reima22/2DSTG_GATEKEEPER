//=============================================================================
//
// テキストデータ管理 [textdata.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_H_
#define _TEXTDATA_H_

#include "main.h"

// マクロ定義
#define TEXT_NUM	(128)	// テキストデータバイト数

class CTextDataMeshfield;

//==============================================================================
// テキスト管理クラス
//==============================================================================
class CTextData
{
public:
	// テキストデータの種類
	typedef enum
	{
		TEXTDATA_RANKING = 0,	// ランキングのデータ
		TEXTDATA_SYSTEM,		// ゲームルールデータ
		TEXTDATA_PLAYER,		// プレイヤーのデータ
		TEXTDATA_ENEMY,			// 敵データ
		TEXTDATA_MESHFIELD,		// メッシュデータ
		TEXTDATA_MAX
	}TEXTDATA;

	CTextData();
	~CTextData();

	static void LoadTextAll(void);
	static void UnloadTextAll(void);

	static CTextDataMeshfield *GetMeshfield(void) { return m_pDataMeshfield; }

protected:
	static char *m_pFileName[TEXTDATA_MAX];	// 読み込むファイル名パス

private:
	static CTextDataMeshfield *m_pDataMeshfield;
};

#endif