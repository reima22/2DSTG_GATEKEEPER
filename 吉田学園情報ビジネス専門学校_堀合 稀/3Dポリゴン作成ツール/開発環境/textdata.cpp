//==============================================================================
//
// テキストデータ管理〔textdata.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "stdio.h"
#include "meshfield.h"
#include "exe.h"

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
		pTextData->LoadText();
	}

	return pTextData;
}

//==============================================================================
// テキストデータの読み込み
//==============================================================================
void CTextData::LoadText(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM] = {};
	char *pLoadText = &aLoadText[0];
	int nTexCnt = 0;

	// データの読み込み
	pFile = fopen("data/TEXT/meshtooldata.txt", "r");

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

			// 配置情報のカウント
			if (strcmp("FIELDSET", pLoadText) == 0)
			{
				char aObjectData[TEXT_NUM] = {};
				char *pObjectData = &aObjectData[0];

				while (strcmp("END_FIELDSET", pObjectData) != 0)
				{
					// 項目読み込み
					fscanf(pFile, "%s", pObjectData);

					// 種類
					if (strcmp("TEXTYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nType);
					}

					// 種類
					if (strcmp("TEXTYPE_SUB", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nTypeSub);
					}

					// 位置
					if (strcmp("POS", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aObjectData[0],
							&m_pos.x,
							&m_pos.y,
							&m_pos.z);
					}

					// 向き
					if (strcmp("ROT", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aObjectData[0],
							&m_rot.x,
							&m_rot.y,
							&m_rot.z);
					}

					// サイズ
					if (strcmp("SIZE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f", &aObjectData[0],
							&m_size.x,
							&m_size.y);
					}

					// 横幅の枚数
					if (strcmp("WIDTH", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0],
							&m_nWidth);
					}

					// 奥行の枚数
					if (strcmp("DEPTH", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0],
							&m_nDepth);

						m_nIdxPoint = (m_nWidth + 1)*(m_nDepth + 1);
					}

					// 波の高さ
					if (strcmp("HEIGHT_WAVE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fHeightWave);
					}

					// 波の間隔
					if (strcmp("DISTANCE_WAVE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fDistanceWave);
					}

					// 波の速さ
					if (strcmp("SPEED_WAVE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fSpeedWave);
					}

					// 描画合成方法
					if (strcmp("SYNTHETICTYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nSyntheticType);
					}

					// 波の方向
					if (strcmp("WAVETYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_nWaveType);
					}

					// 描画合成方法
					if (strcmp("CUTTEX", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], (int*)&m_bCutTex);
					}

					// テクスチャの流れる方向
					if (strcmp("TEXMOVEROT_0", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMoveRot[0]);
					}
					// テクスチャの流れる方向
					if (strcmp("TEXMOVE_0", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMove[0]);
					}
					// テクスチャの流れる方向
					if (strcmp("TEXMOVEROT_1", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMoveRot[1]);
					}
					// テクスチャの流れる方向
					if (strcmp("TEXMOVE_1", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_fTexMove[1]);
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

// テキストデータの破棄
void CTextData::UnloadText(void)
{
	if (m_pFileTextureName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
		{
			delete[] m_pFileTextureName[nCnt];
		}

		delete[] m_pFileTextureName;
		m_pFileTextureName = NULL;
	}

	if (m_pIdx != NULL)
	{
		delete[] m_pIdx;
		m_pIdx = NULL;
	}
}

// テキストデータの書き出し
void CTextData::SaveText(void)
{
	// ローカル変数宣言
	FILE *pFile;
	CMeshfield *pMeshfield = CExe::GetMeshField();

	// データの読み込み
	pFile = fopen("data/TEXT/output.txt", "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "# 書き出したメッシュ情報\n");

		// テクスチャ0のインデックス
		fprintf(pFile, "TEXTYPE = %d\n", pMeshfield->GetTexIdx(0));

		// テクスチャ1のインデックス
		fprintf(pFile, "TEXTYPE_SUB = %d\n", pMeshfield->GetTexIdx(1));

		// 位置
		fprintf(pFile, "POS = %.1f %.1f %.1f\n",
			pMeshfield->GetPosition().x,
			pMeshfield->GetPosition().y,
			pMeshfield->GetPosition().z);

		// 角度
		fprintf(pFile, "ROT = %.1f %.1f %.1f\n",
			pMeshfield->GetRotation().x,
			pMeshfield->GetRotation().y,
			pMeshfield->GetRotation().z);

		// サイズ
		fprintf(pFile, "SIZE = %.1f %.1f\n",
			pMeshfield->GetWidthSize(),
			pMeshfield->GetDepthSize());

		// ポリゴンの枚数
		fprintf(pFile, "WIDTH = %d\n", pMeshfield->GetWidthNum());
		fprintf(pFile, "DEPTH = %d\n", pMeshfield->GetDepthNum());

		// 波の高さ
		fprintf(pFile, "HEIGHT_WAVE = %.3f\n",pMeshfield->GetHeightWave());

		// 波の間隔
		fprintf(pFile, "DISTANCE_WAVE = %.3f\n", pMeshfield->GetDistanceWave());

		// 波の早さ
		fprintf(pFile, "SPEED_WAVE = %.2f\n", pMeshfield->GetSpeedWave());

		// テクスチャ描画合成方法
		fprintf(pFile, "SYNTHETICTYPE = %d\n", pMeshfield->GetSyntheticType());

		// 波の動き
		fprintf(pFile, "WAVETYPE = %d\n", pMeshfield->GetWaveType());

		// テクスチャの分割
		fprintf(pFile, "CUTTEX = %d\n", (int)pMeshfield->GetCutTex());

		// テクスチャの流れる方向,0
		fprintf(pFile, "TEXMOVEROT_0 = %.2f\n", pMeshfield->GetTexMoveRot(0));

		// テクスチャの流れる速さ,0
		fprintf(pFile, "TEXMOVE_0 = %.2f\n", pMeshfield->GetTexMove(0));

		// テクスチャの流れる方向,1
		fprintf(pFile, "TEXMOVEROT_1 = %.2f\n", pMeshfield->GetTexMoveRot(1));

		// テクスチャの流れる速さ,1
		fprintf(pFile, "TEXMOVE_1 = %.2f\n", pMeshfield->GetTexMove(1));
	}
	else
	{
		/*読み込み失敗*/
	}
}