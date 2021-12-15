//==============================================================================
//
// テキストデータ（3Dオブジェクト）管理〔textdata_object.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
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

	//// データの読み込み
	//pFile = fopen(m_pFileName[TEXTDATA_MESHFIELD], "r");

	//if (pFile != NULL)
	//{ //ファイル展開可能

	//  // スクリプト読み込み終わりまで
	//	while (strcmp("END_SCRIPT", pLoadText) != 0)
	//	{
	//		// 項目読み込み
	//		fscanf(pFile, "%s", pLoadText);

	//		// 配置情報のカウント
	//		if (strcmp("FIELDSET", pLoadText) == 0)
	//		{
	//			char aObjectData[TEXT_NUM] = {};
	//			char *pObjectData = &aObjectData[0];

	//			while (strcmp("END_FIELDSET", pObjectData) != 0)
	//			{
	//				// 読み込み
	//				fscanf(pFile, "%s", &aObjectData[0]);
	//			}

	//			m_nNumMesh++;		// オブジェクト数の加算
	//		}
	//	}

	//	// ファイルを閉じる
	//	fclose(pFile);
	//}
	//else
	//{ // ファイル展開不可
	//	return;
	//}

	//// ファイル読み取り
	//pFile = NULL;

	// 各パラメータの動的確保
	//m_pPosVtx = new D3DXVECTOR3*[m_nNumMesh];	// 頂点情報

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
						fscanf(pFile, "%s %d", &aMeshData[0], &m_nType);
					}

					// 位置
					if (strcmp("POS", pMeshData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aMeshData[0],
							&m_pos.x,
							&m_pos.y,
							&m_pos.z);
					}

					// 向き
					if (strcmp("ROT", pMeshData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aMeshData[0],
							&m_rot.x,
							&m_rot.y,
							&m_rot.z);
					}

					// サイズ
					if (strcmp("SIZE", pMeshData) == 0)
					{
						fscanf(pFile, "%s %f %f", &aMeshData[0],
							&m_size.x,
							&m_size.y);
					}

					// 横幅の枚数
					if (strcmp("WIDTH", pMeshData) == 0)
					{
						fscanf(pFile, "%s %d", &aMeshData[0],
							&m_nWidth);
					}

					// 奥行の枚数
					if (strcmp("DEPTH", pMeshData) == 0)
					{
						fscanf(pFile, "%s %d", &aMeshData[0],
							&m_nDepth);

						m_nIdxPoint = (m_nWidth + 1)*(m_nDepth + 1);

						// 頂点動的確保
						m_pPosVtx = new D3DXVECTOR3[m_nIdxPoint];

						for (int nCntIdx = 0; nCntIdx < m_nIdxPoint; nCntIdx++)
						{
							// 値の初期化
							m_pPosVtx[nCntIdx] = VECTOR3_NULL;
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
									&m_pPosVtx[nCntIdx].x, &m_pPosVtx[nCntIdx].y, &m_pPosVtx[nCntIdx].z);

								nCntIdx++;
							}
						}
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
// データの破棄
//==============================================================================
void CTextDataMeshfield::Unload(void)
{
	//// ファイル名ポインタ
	//if (m_pFileObjectName != NULL)
	//{
	//	delete[] m_pFileObjectName;
	//	m_pFileObjectName = NULL;
	//}

	//// ファイル名読み込みポインタの破棄
	//if (m_pFileObjectNameLoad != NULL)
	//{
	//	for (int nCnt = 0; nCnt < m_nNumFile; nCnt++)
	//	{
	//		delete[] m_pFileObjectNameLoad[nCnt];
	//		m_pFileObjectNameLoad[nCnt] = NULL;
	//	}

	//	delete[] m_pFileObjectNameLoad;
	//	m_pFileObjectNameLoad = NULL;
	//}

	//// 種類の破棄
	//if (m_pType != NULL)
	//{
	//	delete[] m_pType;
	//	m_pType = NULL;
	//}

	//// 位置・角度の破棄
	//if (m_pObjectInfo != NULL)
	//{
	//	for (int nCnt = 0; nCnt < m_nNumObject; nCnt++)
	//	{
	//		delete[] m_pObjectInfo[nCnt];
	//		m_pObjectInfo[nCnt] = NULL;
	//	}

	//	delete[] m_pObjectInfo;
	//	m_pObjectInfo = NULL;
	//}

	//// 影の大きさ
	//if (m_pShadowWidth != NULL)
	//{
	//	delete[] m_pShadowWidth;
	//	m_pShadowWidth = NULL;
	//}
	//if (m_pShadowDepth != NULL)
	//{
	//	delete[] m_pShadowDepth;
	//	m_pShadowDepth = NULL;
	//}

	//// 中間地点の可否
	//if (m_pSavePoint != NULL)
	//{
	//	delete[] m_pSavePoint;
	//	m_pSavePoint = NULL;
	//}
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

//==============================================================================
// メッシュデータの保存
//==============================================================================
void CTextDataMeshfield::SaveData(CMeshfield *pMesh)
{
	if (pMesh != NULL)
	{
		// ローカル変数宣言
		FILE *pFile;

		// データ書き込み
		pFile = fopen("data/TEXT/meshfielddata.txt", "w");

		if (pFile != NULL)
		{
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "# \n");
			fprintf(pFile, "# 『メッシュフィールドデータ』スクリプトファイル [meshfielddata.txt]\n");
			fprintf(pFile, "# Author : Horiai Mare\n");
			fprintf(pFile, "# \n");
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "SCRIPT\n\n");

			
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "# メッシュフィールドの配置\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			fprintf(pFile, "FIELDSET\n");

			// テクスチャのインデックス
			fprintf(pFile, "	TEXTYPE = %d\n", pMesh->GetTexIdx());

			// メッシュの位置
			fprintf(pFile, "	MAIN_POS = %.1f %.1f %.1f\n",pMesh->GetPosition().x, pMesh->GetPosition().y, pMesh->GetPosition().z);

			// メッシュの角度
			fprintf(pFile, "	ROT = %.1f %.1f %.1f\n", pMesh->GetRotation().x, pMesh->GetRotation().y, pMesh->GetRotation().z);

			// メッシュのサイズ
			fprintf(pFile, "	SIZE = %.1f %.1f\n", pMesh->GetWidthSize(), pMesh->GetDepthSize());

			// メッシュの枚数
			fprintf(pFile, "	WIDTH = %d\n", pMesh->GetWidthNum());
			fprintf(pFile, "	DEPTH = %d\n", pMesh->GetDepthNum());



			// 頂点設定開始
			fprintf(pFile, "	VTXINFOSET\n");

			for (int nCnt = 0; nCnt < pMesh->GetIdxPoint(); nCnt++)
			{
				D3DXVECTOR3 posVtx = pMesh->GetPosVtx(nCnt);

				fprintf(pFile, "		POS = %.1f %.1f %.1f\n", posVtx.x, posVtx.y, posVtx.z);
			}

			fprintf(pFile, "	END_VTXINFOSET\n\n");

			fprintf(pFile, "END_FIELDSET\n\n");

			fprintf(pFile, "END_SCRIPT");

			fclose(pFile);
		}
	}
}