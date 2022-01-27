//==============================================================================
//
// テキストデータ管理〔textdata.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_meshfield.h"
#include "textdata_object.h"
#include "textdata_pause.h"
#include "textdata_player.h"
#include "textdata_rankingselect.h"
#include "textdata_Terrain.h"
#include "textdata_texture.h"
#include "stdio.h"

// 静的メンバ変数宣言
char *CTextData::m_pFileName[TEXTDATA_MAX] =
{
	{ "data/TEXT/systemdata.txt" },			// ルール管理
	{ "data/TEXT/playermen.txt" },			// プレイヤーデータ
	{ "data/TEXT/texturedata.txt" },		// テクスチャデータ
	{ "data/TEXT/pausedata.txt" },			// ポーズポリゴン情報
	{ "data/TEXT/ranking_selectdata.txt" },	// ランキング選択データ情報
	{ "data/TEXT/terraindata.txt" },		// メッシュ地形データ
	{ "data/TEXT/meshfielddata.txt" },		// 起伏のあるメッシュデータ
	{ "data/TEXT/sounddata.txt" },			// サウンドデータ
};

char *CTextData::m_pFileNameObj[CTextDataObject::STAGEINFO_MAX] =
{
	{ "data/TEXT/StageObject/00_valley_objdata.txt" },		// 渓谷ステージオブジェクトデータ
	{ "data/TEXT/StageObject/01_sea_objdata.txt" },			// 海洋ステージオブジェクトデータ
	{ "data/TEXT/StageObject/02_lava_objdata.txt" },		// 溶岩ステージオブジェクトデータ
};

char *CTextData::m_pFileNameRanking[CTextDataObject::STAGEINFO_MAX] =
{
	{ "data/TEXT/RANKING/00_valley_ranking.txt" },		// 渓谷ステージランキングデータ
	{ "data/TEXT/RANKING/01_ocean_ranking.txt" },		// 海洋ステージランキングデータ
	{ "data/TEXT/RANKING/02_lava_ranking.txt" },		// 溶岩ステージランキングデータ
};

CTextDataPlayer *CTextData::m_pDataPlayer = NULL;					// プレイヤーデータのポインタ
CTextDataObject *CTextData::m_pDataObject[STAGEINFO_MAX] = {};		// ステージオブジェクトデータのポインタ
CTextDataTexture *CTextData::m_pDataTexture = NULL;					// テクスチャデータのポインタ
CTextDataPause *CTextData::m_pDataPause = NULL;						// ポーズデータのポインタ
CTextDataTerrain *CTextData::m_pDataTerrain = NULL;					// 死亡判定メッシュ地形のデータポインタ
CTextDataMeshfield *CTextData::m_pDataMeshfield = NULL;				// 起伏地形のデータポインタ
CTextDataRanking *CTextData::m_apDataRanking[STAGEINFO_MAX] = {};	// ランキングデータ
CTextDataRankingSelect *CTextData::m_pDataRankingSelect = NULL;		// ランキング選択データのポインタ
CTextDataSound *CTextData::m_pDataSound = NULL;						// サウンドデータポインタ

																	//==============================================================================
																	// コンストラクタ
																	//==============================================================================
CTextData::CTextData()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextData::~CTextData()
{

}

//==============================================================================
// 生成処理
//==============================================================================
CTextData *CTextData::Create(void)
{
	// ローカル変数宣言
	CTextData *pTextData;
	pTextData = new CTextData;	// インスタンス生成

								// テキストデータ
	if (pTextData != NULL)
	{
		// テキストデータの読み込み
		pTextData->LoadTextAll();
	}

	return pTextData;
}

//==============================================================================
// テキストデータの全読み込み
//==============================================================================
void CTextData::LoadTextAll(void)
{
	// プレイヤーデータの生成
	m_pDataPlayer = CTextDataPlayer::Create();

	// オブジェクトデータの生成
	for (int nCntStage = 0; nCntStage < CTextDataObject::STAGEINFO_MAX; nCntStage++)
	{
		m_pDataObject[nCntStage] = CTextDataObject::Create((STAGEINFO)nCntStage);
	}

	// テクスチャデータの生成
	m_pDataTexture = CTextDataTexture::Create();

	// ポーズデータの生成
	m_pDataPause = CTextDataPause::Create();

	// ランキング選択データの生成
	m_pDataRankingSelect = CTextDataRankingSelect::Create();

	// 地形データの生成
	m_pDataTerrain = CTextDataTerrain::Create();

	// メッシュデータ
	m_pDataMeshfield = CTextDataMeshfield::Create();
}

//==============================================================================
// テキストデータの全読み込み
//==============================================================================
void CTextData::UninitAll(void)
{
	// プレイヤーデータの破棄
	if (m_pDataPlayer != NULL)
	{
		m_pDataPlayer->Unload();
		m_pDataPlayer = NULL;
	}

	// オブジェクトデータの破棄
	for (int nCntObj = 0; nCntObj < CTextDataObject::STAGEINFO_MAX; nCntObj++)
	{
		if (m_pDataObject[nCntObj] != NULL)
		{
			m_pDataObject[nCntObj]->Unload();
			m_pDataObject[nCntObj] = NULL;
		}
	}

	// テクスチャデータの破棄
	if (m_pDataTexture != NULL)
	{
		m_pDataTexture->Unload();
		m_pDataTexture = NULL;
	}

	// ポーズデータの破棄
	if (m_pDataPause != NULL)
	{
		m_pDataPause->Unload();
		m_pDataPause = NULL;
	}

	// ランキング選択データの破棄
	if (m_pDataRankingSelect != NULL)
	{
		m_pDataRankingSelect->Unload();
		m_pDataRankingSelect = NULL;
	}

	// 死亡判定地形データの破棄
	if (m_pDataTerrain != NULL)
	{
		m_pDataTerrain->Unload();
		m_pDataTerrain = NULL;
	}

	// 起伏地形データの破棄
	if (m_pDataMeshfield != NULL)
	{
		m_pDataMeshfield->Unload();
		m_pDataMeshfield = NULL;
	}
}