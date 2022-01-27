//==============================================================================
//
// テキストデータ（ポーズ）管理〔textdata_pause.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "textdata.h"
#include "textdata_pause.h"
#include "stdio.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataPause::CTextDataPause()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataPause::~CTextDataPause()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataPause::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_PAUSE], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

	  // スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// ポーズウィンドウ
			if (strcmp("SETBASE", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aBase[TEXT_NUM] = {};
				char *pBase = &aBase[0];

				while (strcmp("END_SETBASE", pBase) != 0)
				{
					fscanf(pFile, "%s", &aBase[0]);

					// 使用テクスチャ
					if (strcmp("TEXTYPE", pBase) == 0)
					{
						fscanf(pFile, "%s %d", &aBase[0], &m_aIdxTex[PAUSEPOLY_BASE]);
					}

					// 位置情報
					if (strcmp("POS", pBase) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aBase[0],
							&m_aPos[PAUSEPOLY_BASE].x,
							&m_aPos[PAUSEPOLY_BASE].y,
							&m_aPos[PAUSEPOLY_BASE].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pBase) == 0)
					{
						fscanf(pFile, "%s %f %f", &aBase[0],
							&m_aSize[PAUSEPOLY_BASE].x,
							&m_aSize[PAUSEPOLY_BASE].y);
					}
				}			
			}

			// 選択肢「コンテニュー」
			if (strcmp("SETSELECT_CONTINUE", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aContinue[TEXT_NUM] = {};
				char *pContinue = &aContinue[0];

				while (strcmp("END_SETSELECT_CONTINUE", pContinue) != 0)
				{
					fscanf(pFile, "%s", &aContinue[0]);

					// 使用テクスチャ
					if (strcmp("TEXTYPE", pContinue) == 0)
					{
						fscanf(pFile, "%s %d", &aContinue[0], &m_aIdxTex[PAUSEPOLY_CONTINUE]);
					}

					// 位置情報
					if (strcmp("POS", pContinue) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aContinue[0],
							&m_aPos[PAUSEPOLY_CONTINUE].x,
							&m_aPos[PAUSEPOLY_CONTINUE].y,
							&m_aPos[PAUSEPOLY_CONTINUE].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pContinue) == 0)
					{
						fscanf(pFile, "%s %f %f", &aContinue[0],
							&m_aSize[PAUSEPOLY_CONTINUE].x,
							&m_aSize[PAUSEPOLY_CONTINUE].y);
					}
				}			
			}

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
						fscanf(pFile, "%s %d", &aRetry[0], &m_aIdxTex[PAUSEPOLY_RETRY]);
					}

					// 位置情報
					if (strcmp("POS", pRetry) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aRetry[0],
							&m_aPos[PAUSEPOLY_RETRY].x,
							&m_aPos[PAUSEPOLY_RETRY].y,
							&m_aPos[PAUSEPOLY_RETRY].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pRetry) == 0)
					{
						fscanf(pFile, "%s %f %f", &aRetry[0],
							&m_aSize[PAUSEPOLY_RETRY].x,
							&m_aSize[PAUSEPOLY_RETRY].y);
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
						fscanf(pFile, "%s %d", &aQuit[0], &m_aIdxTex[PAUSEPOLY_QUIT]);
					}

					// 位置情報
					if (strcmp("POS", pQuit) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aQuit[0],
							&m_aPos[PAUSEPOLY_QUIT].x,
							&m_aPos[PAUSEPOLY_QUIT].y,
							&m_aPos[PAUSEPOLY_QUIT].z);
					}

					// サイズ情報
					if (strcmp("SIZE", pQuit) == 0)
					{
						fscanf(pFile, "%s %f %f", &aQuit[0],
							&m_aSize[PAUSEPOLY_QUIT].x,
							&m_aSize[PAUSEPOLY_QUIT].y);
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
// ポーズデータの破棄
//==============================================================================
void CTextDataPause::Unload(void)
{
	// 値の破棄
	for (int nCnt = 0; nCnt < PAUSEPOLY_MAX; nCnt++)
	{
		m_aIdxTex[nCnt] = 0;
		m_aPos[nCnt] = {};
		m_aSize[nCnt] = {};
	}
}

//==============================================================================
// ポーズデータの生成
//==============================================================================
CTextDataPause *CTextDataPause::Create(void)
{
	// プレイヤーデータの読み込み
	CTextDataPause *pDataPause;
	pDataPause = new CTextDataPause;

	// データのロード
	if (pDataPause != NULL)
	{
		pDataPause->LoadData();
	}

	return pDataPause;
}