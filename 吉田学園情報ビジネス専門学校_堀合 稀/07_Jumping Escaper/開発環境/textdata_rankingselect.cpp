//==============================================================================
//
// テキストデータ（ランキング）管理〔textdata_ranking.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ranking_score.h"
#include "select.h"
#include "textdata.h"
#include "textdata_rankingselect.h"
#include "stdio.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataRankingSelect::CTextDataRankingSelect()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataRankingSelect::~CTextDataRankingSelect()
{

}

//==============================================================================
// 選択テキストの読み込み
//==============================================================================
void CTextDataRankingSelect::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_RANKINGSELECT], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		// スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 選択肢「リトライ」
			if (strcmp("SETSELECT_RETRY", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aRetry[TEXT_NUM] = {};
				char *pRetry = &aRetry[0];

				while (strcmp("END_SETSELECT_RETRY", pRetry) != 0)
				{
					fscanf(pFile, "%s", &aRetry[0]);

					// 使用テクスチャ
					if (strcmp("TEXTYPE", pRetry) == 0)
					{
						fscanf(pFile, "%s %d", &aRetry[0], &m_aIdxTex[RANKINGPOLY_RETRY]);
					}

					// 位置情報
					if (strcmp("POS", pRetry) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aRetry[0],
							&m_aPos[RANKINGPOLY_RETRY].x,
							&m_aPos[RANKINGPOLY_RETRY].y,
							&m_aPos[RANKINGPOLY_RETRY].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pRetry) == 0)
					{
						fscanf(pFile, "%s %f %f", &aRetry[0],
							&m_aSize[RANKINGPOLY_RETRY].x,
							&m_aSize[RANKINGPOLY_RETRY].y);
					}
				}
			}

			// 選択肢「終了」
			if (strcmp("SETSELECT_QUIT", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aStage[TEXT_NUM] = {};
				char *pStage = &aStage[0];

				while (strcmp("END_SETSELECT_QUIT", pStage) != 0)
				{
					fscanf(pFile, "%s", &aStage[0]);

					// 使用テクスチャ
					if (strcmp("TEXTYPE", pStage) == 0)
					{
						fscanf(pFile, "%s %d", &aStage[0], &m_aIdxTex[RANKINGPOLY_STAGE]);
					}

					// 位置情報
					if (strcmp("POS", pStage) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aStage[0],
							&m_aPos[RANKINGPOLY_STAGE].x,
							&m_aPos[RANKINGPOLY_STAGE].y,
							&m_aPos[RANKINGPOLY_STAGE].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pStage) == 0)
					{
						fscanf(pFile, "%s %f %f", &aStage[0],
							&m_aSize[RANKINGPOLY_STAGE].x,
							&m_aSize[RANKINGPOLY_STAGE].y);
					}
				}
			}

			// 選択肢「終了」
			if (strcmp("SETSELECT_QUIT", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aQuit[TEXT_NUM] = {};
				char *pQuit = &aQuit[0];

				while (strcmp("END_SETSELECT_QUIT", pQuit) != 0)
				{
					fscanf(pFile, "%s", &aQuit[0]);

					// 使用テクスチャ
					if (strcmp("TEXTYPE", pQuit) == 0)
					{
						fscanf(pFile, "%s %d", &aQuit[0], &m_aIdxTex[RANKINGPOLY_QUIT]);
					}

					// 位置情報
					if (strcmp("POS", pQuit) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aQuit[0],
							&m_aPos[RANKINGPOLY_QUIT].x,
							&m_aPos[RANKINGPOLY_QUIT].y,
							&m_aPos[RANKINGPOLY_QUIT].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pQuit) == 0)
					{
						fscanf(pFile, "%s %f %f", &aQuit[0],
							&m_aSize[RANKINGPOLY_QUIT].x,
							&m_aSize[RANKINGPOLY_QUIT].y);
					}
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
		return;
	}
}

//==============================================================================
// 選択テキストデータの破棄
//==============================================================================
void CTextDataRankingSelect::Unload(void)
{
	// 値の破棄
	for (int nCnt = 0; nCnt < RANKINGPOLY_MAX; nCnt++)
	{
		m_aIdxTex[nCnt] = 0;
		m_aPos[nCnt] = {};
		m_aSize[nCnt] = {};
	}
}

//==============================================================================
// 選択データの生成
//==============================================================================
CTextDataRankingSelect *CTextDataRankingSelect::Create(void)
{
	// ランキングデータの読み込み
	CTextDataRankingSelect *pDataRankingSelect;
	pDataRankingSelect = new CTextDataRankingSelect;

	// データのロード
	if (pDataRankingSelect != NULL)
	{
		pDataRankingSelect->LoadData();
	}

	return pDataRankingSelect;
}