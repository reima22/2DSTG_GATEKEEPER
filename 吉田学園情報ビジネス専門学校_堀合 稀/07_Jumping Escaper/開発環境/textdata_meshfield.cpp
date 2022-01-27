//==============================================================================
//
// テキストデータ（3Dオブジェクト）管理〔textdata_object.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "textdata.h"
#include "textdata_meshfield.h"
#include "stdio.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataMeshfield::CTextDataMeshfield()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataMeshfield::~CTextDataMeshfield()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataMeshfield::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM] = {};
	char *pLoadText = &aLoadText[0];

	int nModelCnt = 0;		// モデル数のカウント

	m_nNumMesh = 0;			// 配置するオブジェクト数

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_MESHFIELD], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

	  // スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 配置情報のカウント
			if (strcmp("FIELDSET", pLoadText) == 0)
			{
				char aObjectData[TEXT_NUM] = {};
				char *pObjectData = &aObjectData[0];

				while (strcmp("END_FIELDSET", pObjectData) != 0)
				{
					// 読み込み
					fscanf(pFile, "%s", &aObjectData[0]);
				}

				m_nNumMesh++;		// オブジェクト数の加算
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
		return;
	}

	// ファイル読み取り2周目
	pFile = NULL;

	// 各パラメータの動的確保
	m_pType = new int[m_nNumMesh];				// 種類
	m_pPos = new D3DXVECTOR3[m_nNumMesh];		// 位置
	m_pRot = new D3DXVECTOR3[m_nNumMesh];		// 角度
	m_pSize = new D3DXVECTOR2[m_nNumMesh];		// サイズ
	m_pWidth = new int[m_nNumMesh];				// 横幅の枚数
	m_pDepth = new int[m_nNumMesh];				// 奥行の枚数

	m_pIdxPoint = new int[m_nNumMesh];			// 頂点数
	m_pPosVtx = new D3DXVECTOR3*[m_nNumMesh];	// 頂点情報

	int nCntObject = 0;	// カウント

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_MESHFIELD], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		aLoadText[0] = {};

		// スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 配置情報の読み込み
			if (strcmp("FIELDSET", pLoadText) == 0)
			{
				char aMeshData[TEXT_NUM] = {};
				char *pMeshData = &aMeshData[0];

				while (strcmp("END_FIELDSET", pMeshData) != 0)
				{
					// 読み込み
					fscanf(pFile, "%s", &aMeshData[0]);

					// 種類
					if (strcmp("TEXTYPE", pMeshData) == 0)
					{
						fscanf(pFile, "%s %d", &aMeshData[0], &m_pType[nCntObject]);
					}

					// 位置
					if (strcmp("MAIN_POS", pMeshData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aMeshData[0],
							&m_pPos[nCntObject].x,
							&m_pPos[nCntObject].y,
							&m_pPos[nCntObject].z);
					}

					// 向き
					if (strcmp("ROT", pMeshData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aMeshData[0],
							&m_pRot[nCntObject].x,
							&m_pRot[nCntObject].y,
							&m_pRot[nCntObject].z);
					}

					// サイズ
					if (strcmp("SIZE", pMeshData) == 0)
					{
						fscanf(pFile, "%s %f %f", &aMeshData[0],
							&m_pSize[nCntObject].x,
							&m_pSize[nCntObject].y);
					}

					// 横幅の枚数
					if (strcmp("WIDTH", pMeshData) == 0)
					{
						fscanf(pFile, "%s %d", &aMeshData[0],
							&m_pWidth[nCntObject]);
					}

					// 奥行の枚数
					if (strcmp("DEPTH", pMeshData) == 0)
					{
						fscanf(pFile, "%s %d", &aMeshData[0],
							&m_pDepth[nCntObject]);

						m_pIdxPoint[nCntObject] = (m_pWidth[nCntObject] + 1)*(m_pDepth[nCntObject] + 1);

						// 頂点数の算出
						m_pPosVtx[nCntObject] = new D3DXVECTOR3[m_pIdxPoint[nCntObject]];

						for (int nCntIdx = 0; nCntIdx < m_pIdxPoint[nCntObject]; nCntIdx++)
						{
							m_pPosVtx[nCntObject][nCntIdx] = VECTOR3_NULL;
						}
					}

					// 頂点情報
					if (strcmp("VTXINFOSET", pMeshData) == 0)
					{
						// ローカル変数宣言
						char aVtxData[TEXT_NUM] = {};
						char *pVtxData = &aVtxData[0];
						int nCntIdx = 0;

						while (strcmp("END_VTXINFOSET", pVtxData) != 0)
						{
							// 読み込み
							fscanf(pFile, "%s", &aVtxData[0]);

							// 頂点座標
							if (strcmp("POS", pVtxData) == 0)
							{
								fscanf(pFile, "%s %f %f %f", &aVtxData[0], 
									&m_pPosVtx[nCntObject][nCntIdx].x, &m_pPosVtx[nCntObject][nCntIdx].y, &m_pPosVtx[nCntObject][nCntIdx].z);

								nCntIdx++;
							}
						}
					}
				}

				nCntObject++;		// カウント加算
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
// データの破棄
//==============================================================================
void CTextDataMeshfield::Unload(void)
{
	// 種類の破棄
	if (m_pType != NULL)
	{
		if (m_nNumMesh > 1)
		{
			delete[] m_pType;
			m_pType = NULL;
		}
		else
		{
			delete m_pType;
			m_pType = NULL;
		}
	}

	// 位置の開放
	if (m_pPos != NULL)
	{	
		if (m_nNumMesh > 1)
		{
			delete[] m_pPos;
			m_pPos = NULL;
		}
		else
		{
			delete m_pPos;
			m_pPos = NULL;
		}
	}

	// 角度の開放
	if (m_pRot != NULL)
	{
		if (m_nNumMesh > 1)
		{
			delete[] m_pRot;
			m_pRot = NULL;
		}
		else
		{
			delete m_pRot;
			m_pRot = NULL;
		}
	}

	// サイズの開放
	if (m_pSize != NULL)
	{
		if (m_nNumMesh > 1)
		{
			delete[] m_pSize;
			m_pSize = NULL;
		}
		else
		{
			delete m_pSize;
			m_pSize = NULL;
		}
	}

	// 横幅ポリゴン枚数の開放
	if (m_pWidth != NULL)
	{
		if (m_nNumMesh > 1)
		{
			delete[] m_pWidth;
			m_pWidth = NULL;
		}
		else
		{
			delete m_pWidth;
			m_pWidth = NULL;
		}
	}

	// 奥行ポリゴン枚数の開放
	if (m_pDepth != NULL)
	{
		if (m_nNumMesh > 1)
		{
			delete[] m_pDepth;
			m_pDepth = NULL;
		}
		else
		{
			delete m_pDepth;
			m_pDepth = NULL;
		}
	}

	// 頂点情報の開放
	if (m_pPosVtx != NULL)
	{
		for (int nCntIdx = 0; nCntIdx < m_nNumMesh; nCntIdx++)
		{
			delete[] m_pPosVtx[nCntIdx];
			m_pPosVtx[nCntIdx] = NULL;
		}

		if (m_nNumMesh > 1)
		{
			delete[] m_pPos;
			m_pPosVtx = NULL;
		}
		else
		{
			delete m_pPos;
			m_pPosVtx = NULL;
		}
	}

	// 頂点数の開放
	if (m_pIdxPoint != NULL)
	{
		if (m_nNumMesh > 1)
		{
			delete[] m_pIdxPoint;
			m_pIdxPoint = NULL;
		}
		else
		{
			delete m_pIdxPoint;
			m_pIdxPoint = NULL;
		}
	}
}

//==============================================================================
// オブジェクトデータの生成
//==============================================================================
CTextDataMeshfield *CTextDataMeshfield::Create(void)
{
	// プレイヤーデータの読み込み
	CTextDataMeshfield *pDataMeshfield;
	pDataMeshfield = new CTextDataMeshfield;

	// データのロード
	if (pDataMeshfield != NULL)
	{
		pDataMeshfield->LoadData();
	}

	return pDataMeshfield;
}