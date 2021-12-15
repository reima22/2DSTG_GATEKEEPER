//==============================================================================
//
// テキストデータ（テクスチャ）管理〔textdata_texture.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_texture.h"
#include "stdio.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataTexture::CTextDataTexture()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataTexture::~CTextDataTexture()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataTexture::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];
	int nTexCnt = 0;

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_TEXTURE], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

	  // スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// テクスチャ数
			if (strcmp("NUM_TEXTURE", pLoadText) == 0)
			{
				fscanf(pFile, "%s %d", &aLoadText[0], &m_nNumTexture);

				// テクスチャ数分、動的確保
				m_pFileTextureName = new char*[m_nNumTexture];
				m_pIdx = new int[m_nNumTexture];

				// 2次元配列の確保
				for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
				{
					m_pFileTextureName[nCnt] = new char[TEXT_NUM];
				}
			}

			// ファイル名
			if (strcmp("TEXTURE_FILENAME", pLoadText) == 0)
			{
				// ファイル名
				fscanf(pFile, "%s %s", &aLoadText[0], &m_pFileTextureName[nTexCnt][0]);

				// インデックスの割り当て
				m_pIdx[nTexCnt] = nTexCnt;

				// カウント増加
				nTexCnt++;
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
// テクスチャデータの破棄
//==============================================================================
void CTextDataTexture::Unload(void)
{
	// テキストデータの破棄
	for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
	{
		if ((char*)m_pFileTextureName[nCnt] != NULL)
		{
			delete[] (char*)m_pFileTextureName[nCnt];
			m_pFileTextureName[nCnt] = NULL;
		}
	}

	// テクスチャファイル名の破棄
	delete[] m_pFileTextureName;
	m_pFileTextureName = NULL;
}

//==============================================================================
// テクスチャデータの生成
//==============================================================================
CTextDataTexture *CTextDataTexture::Create(void)
{
	// プレイヤーデータの読み込み
	CTextDataTexture *pDataTexture;
	pDataTexture = new CTextDataTexture;

	// データのロード
	if (pDataTexture != NULL)
	{
		pDataTexture->LoadData();
	}

	return pDataTexture;
}