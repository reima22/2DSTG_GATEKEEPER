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

// 前方宣言
class CTextDataPlayer;		// プレイヤーデータクラス
class CTextDataObject;		// オブジェクトデータクラス
class CTextDataTexture;		// テクスチャデータクラス
class CTextDataPause;		// ポーズデータクラス
class CTextDataMeshfield;	// メッシュフィールドデータ

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
		TEXTDATA_OBJECT,		// 3Dオブジェクトデータ
		TEXTDATA_TEXTURE,		// テクスチャデータ
		TEXTDATA_PAUSE,			// ポーズデータ
		TEXTDATA_MESHFIELD,		// メッシュフィールド
		TEXTDATA_MAX
	}TEXTDATA;

	CTextData();
	~CTextData();

	static CTextData *Create(void);	// 生成

	static void LoadTextAll(void);
	static void UninitAll(void);

	static CTextDataPlayer *GetDataPlayer(void) { return m_pDataPlayer; }
	static CTextDataObject *GetDataObject(void) { return m_pDataObject; }
	static CTextDataTexture *GetDataTexture(void) { return m_pDataTexture; }
	static CTextDataPause *GetDataPause(void) { return m_pDataPause; }
	static CTextDataMeshfield *GetDataMeshfield(void) { return m_pDataMeshfield; }

protected:
	static char *m_pFileName[TEXTDATA_MAX];		// 読み込むファイル名パス
	static CTextDataPlayer *m_pDataPlayer;		// プレイヤーデータ
	static CTextDataObject *m_pDataObject;		// オブジェクトデータ
	static CTextDataTexture *m_pDataTexture;	// テクスチャデータ
	static CTextDataPause *m_pDataPause;		// ポーズデータ
	static CTextDataMeshfield *m_pDataMeshfield;// メッシュフィールドデータ
};

#endif