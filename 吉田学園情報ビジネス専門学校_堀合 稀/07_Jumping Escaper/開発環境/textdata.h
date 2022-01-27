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
#define TEXT_NUM	(128)		// テキストデータバイト数

// 前方宣言
class CTextDataPlayer;			// プレイヤーデータクラス
class CTextDataObject;			// オブジェクトデータクラス
class CTextDataTexture;			// テクスチャデータクラス
class CTextDataPause;			// ポーズデータクラス
class CTextDataRankingSelect;	// ランキング選択データクラス
class CTextDataTerrain;			// メッシュデータクラス
class CTextDataMeshfield;		// メッシュフィールドデータ
class CTextDataRanking;			// ランキングデータ
class CTextDataSound;			// サウンドデータクラス

//==============================================================================
// テキスト管理クラス
//==============================================================================
class CTextData
{
public:
	// テキストデータの種類
	typedef enum
	{
		TEXTDATA_SYSTEM = 0,	// ゲームルールデータ
		TEXTDATA_PLAYER,		// プレイヤーのデータ
		TEXTDATA_TEXTURE,		// テクスチャデータ
		TEXTDATA_PAUSE,			// ポーズデータ
		TEXTDATA_RANKINGSELECT,	// ランキング選択データ
		TEXTDATA_TERRAIN,		// メッシュ地形データ
		TEXTDATA_MESHFIELD,		// メッシュフィールドデータ
		TEXTDATA_SOUND,			// サウンドデータ
		TEXTDATA_MAX
	}TEXTDATA;

	// ステージ情報
	typedef enum
	{
		STAGEINFO_VALLEY = 0,	// 渓谷ステージ情報
		STAGEINFO_SEA,			// 海洋ステージ情報
		STAGEINFO_LAVA,			// 溶岩ステージ情報
		STAGEINFO_MAX
	}STAGEINFO;

	CTextData();
	~CTextData();

	static CTextData *Create(void);	// 生成

	static void LoadTextAll(void);
	static void UninitAll(void);

	// データの取得
	static CTextDataPlayer *GetDataPlayer(void) { return m_pDataPlayer; }
	static CTextDataObject *GetDataObject(int nStage) { return m_pDataObject[nStage]; }
	static CTextDataTexture *GetDataTexture(void) { return m_pDataTexture; }
	static CTextDataPause *GetDataPause(void) { return m_pDataPause; }
	static CTextDataTerrain *GetDataTerrain(void) { return m_pDataTerrain; }
	static CTextDataMeshfield *GetDataMeshfield(void) { return m_pDataMeshfield; }
	static CTextDataRanking	*GetDataRanking(int nStage) { return m_apDataRanking[nStage]; }
	static CTextDataRankingSelect *GetDataSelectRanking(void) { return m_pDataRankingSelect; }

protected:
	static char *m_pFileName[TEXTDATA_MAX];			// 読み込むファイル名パス
	static char *m_pFileNameObj[STAGEINFO_MAX];		// 読み取るオブジェクトファイル名パス
	static char *m_pFileNameRanking[STAGEINFO_MAX];	// 読み取るランキングファイル名パス

private:
	static CTextDataPlayer *m_pDataPlayer;						// プレイヤーデータ
	static CTextDataObject *m_pDataObject[STAGEINFO_MAX];		// オブジェクトデータ
	static CTextDataTexture *m_pDataTexture;					// テクスチャデータ
	static CTextDataPause *m_pDataPause;						// ポーズデータ
	static CTextDataTerrain *m_pDataTerrain;					// メッシュ地形データポインタ
	static CTextDataMeshfield *m_pDataMeshfield;				// メッシュフィールドデータ
	static CTextDataRanking *m_apDataRanking[STAGEINFO_MAX];	// ランキングデータ
	static CTextDataRankingSelect *m_pDataRankingSelect;		// ランキング選択データ

	static CTextDataSound *m_pDataSound;						// サウンドデータポインタ
};

#endif