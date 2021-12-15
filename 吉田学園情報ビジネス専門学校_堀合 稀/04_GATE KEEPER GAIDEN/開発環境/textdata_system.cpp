//==============================================================================
//
// テキストデータ（プレイヤー）管理〔textdata_player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_system.h"
#include "stdio.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
int CTextDataSystem::m_aSystemData[SYSTEMDATA_MAX];

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataSystem::CTextDataSystem()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataSystem::~CTextDataSystem()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataSystem::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];

	// 変数の初期化
	for (int nCnt = 0; nCnt < SYSTEMDATA_MAX; nCnt++)
	{
		m_aSystemData[nCnt] = 0;
	}

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_SYSTEM], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		while (strcmp("SYSTEMDATA_END", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 項目に対応した変数への格納
			if (strcmp("DEFEAT_EASY", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_DEFEAT_EASY]);
			}
			else if (strcmp("DEFEAT_NORMAL", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_DEFEAT_NORMAL]);
			}
			else if (strcmp("DEFEAT_HARD", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_DEFEAT_HARD]);
			}
			else if (strcmp("INVASION_EASY", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_INVASION_EASY]);
			}
			else if (strcmp("INVASION_NORMAL", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_INVASION_NORMAL]);
			}
			else if (strcmp("INVASION_HARD", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_INVASION_HARD]);
			}
			else if (strcmp("ITEMSET_COUNT_EASY", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_HEALITEM_CNT_EASY]);
			}
			else if (strcmp("ITEMSET_COUNT_NORMAL", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_HEALITEM_CNT_NORMAL]);
			}
			else if (strcmp("ITEMSET_COUNT_HARD", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aSystemData[SYSTEMDATA_HEALITEM_CNT_HARD]);
			}
		}

		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}