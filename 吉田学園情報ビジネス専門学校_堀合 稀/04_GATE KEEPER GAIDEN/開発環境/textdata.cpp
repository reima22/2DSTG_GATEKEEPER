//==============================================================================
//
// テキストデータ管理〔textdata.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_player.h"
#include "textdata_enemy.h"
#include "textdata_system.h"
#include "textdata_meshfield.h"
#include "stdio.h"

// 静的メンバ変数宣言
char *CTextData::m_pFileName[TEXTDATA_MAX] =
{
	{ "data/TEXT/ranking.txt" },		// ランキング
	{ "data/TEXT/systemdata.txt" },		// ルール管理
	{ "data/TEXT/playerdata.txt" },		// プレイヤーデータ
	{ "data/TEXT/enemydata.txt" },		// 敵データ
	{ "data/TEXT/meshfielddata.txt" },		// メッシュデータ
};
CTextDataMeshfield *CTextData::m_pDataMeshfield = NULL;

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
// テクスチャデータの全読み込み
//==============================================================================
void CTextData::LoadTextAll(void)
{
	// プレイヤーデータの読み込み
	CTextDataPlayer::LoadData();

	// 敵データの読み込み
	CTextDataEnemy::LoadData();

	// 敵データのカウント設定
	CTextDataEnemy::CreateCntSet();

	// ルールのデータ読み込み
	CTextDataSystem::LoadData();

	// メッシュフィールド
	m_pDataMeshfield = CTextDataMeshfield::Create();
}

//==============================================================================
// テクスチャデータの全破棄
//==============================================================================
void CTextData::UnloadTextAll(void)
{
	// メッシュフィールド
	m_pDataMeshfield->UnloadData();
}