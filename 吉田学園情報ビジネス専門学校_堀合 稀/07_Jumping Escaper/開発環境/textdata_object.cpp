//==============================================================================
//
// テキストデータ（3Dオブジェクト）管理〔textdata_object.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "textdata.h"
#include "textdata_object.h"
#include "stdio.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataObject::CTextDataObject()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataObject::~CTextDataObject()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataObject::LoadData(STAGEINFO stage)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM] = {};
	char *pLoadText = &aLoadText[0];

	int nModelCnt = 0;		// モデル数のカウント
	m_nNumFile = 0;			// ファイル数
	m_nNumObject = 0;		// 配置するオブジェクト数

	// データの読み込み
	pFile = fopen(m_pFileNameObj[stage], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

	  // スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 読み込むモデル数
			if (strcmp("NUM_MODEL", pLoadText) == 0)
			{
				fscanf(pFile, "%s %d", &aLoadText[0], &m_nNumFile);

				// ファイル数分、動的確保
				m_pFileObjectName = new char*[m_nNumFile];

				m_pFileObjectNameLoad = new char*[m_nNumFile];

				// 2次元配列の確保
				for (int nCnt = 0; nCnt < m_nNumFile; nCnt++)
				{
					m_pFileObjectNameLoad[nCnt] = new char[TEXT_NUM];

					m_pFileObjectName[nCnt] = &m_pFileObjectNameLoad[nCnt][0];
				}
			}

			// モデルファイル名
			if (strcmp("MODEL_FILENAME", pLoadText) == 0)
			{
				// モデル名
				fscanf(pFile, "%s %s", &aLoadText[0], &m_pFileObjectNameLoad[nModelCnt][0]);

				// カウント増加
				nModelCnt++;
			}

			// 配置情報のカウント
			if (strcmp("OBJECTSET", pLoadText) == 0)
			{
				char aObjectData[TEXT_NUM] = {};
				char *pObjectData = &aObjectData[0];

				while (strcmp("END_OBJECTSET", pObjectData) != 0)
				{
					// 読み込み
					fscanf(pFile, "%s", &aObjectData[0]);
				}

				m_nNumObject++;		// オブジェクト数の加算
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
		return;
	}

	// 再読み込み開始
	pFile = NULL;

	// 各パラメータの動的確保
	m_pType = new int[m_nNumObject];							// 種類
	m_pPos = new D3DXVECTOR3[m_nNumObject];						// 位置
	m_pRot = new D3DXVECTOR3[m_nNumObject];						// 角度
	m_pShadowSize = new D3DXVECTOR2[m_nNumObject];				// 影のサイズ

	m_pObjAttribute = new CObject::OBJATTRIBUTE[m_nNumObject];	// オブジェクトの種類
	m_pMoveType = new CObject::MOVETYPE[m_nNumObject];			// 移動パターン
	m_pMovePower = new float[m_nNumObject];						// 移動力の割合

	for(int nCnt = 0; nCnt < m_nNumObject; nCnt++)
	{// 未入力分の対応
		m_pObjAttribute[nCnt] = (CObject::OBJATTRIBUTE)0;
		m_pMoveType[nCnt] = (CObject::MOVETYPE)0;
		m_pMovePower[nCnt] = 1.0f;
	}

	int nCntObject = 0;	// カウント

	// データの読み込み
	pFile = fopen(m_pFileNameObj[stage], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		aLoadText[0] = {};

	  // スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 配置情報の読み込み
			if (strcmp("OBJECTSET", pLoadText) == 0)
			{
				char aObjectData[TEXT_NUM] = {};
				char *pObjectData = &aObjectData[0];

				while (strcmp("END_OBJECTSET", pObjectData) != 0)
				{
					// 読み込み
					fscanf(pFile, "%s", &aObjectData[0]);

					// 影のサイズ
					if (strcmp("SHADOW", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f", &aObjectData[0], &m_pShadowSize[nCntObject].x, &m_pShadowSize[nCntObject].y);
					}

					// 種類
					if (strcmp("TYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_pType[nCntObject]);
					}

					// 位置
					if (strcmp("POS", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aObjectData[0],
							&m_pPos[nCntObject].x,
							&m_pPos[nCntObject].y,
							&m_pPos[nCntObject].z);
					}

					// 向き
					if (strcmp("ROT", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aObjectData[0],
							&m_pRot[nCntObject].x,
							&m_pRot[nCntObject].y,
							&m_pRot[nCntObject].z);
					}

					// オブジェクトの属性
					if (strcmp("OBJATTRIBUTE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_pObjAttribute[nCntObject]);
					}

					// 移動パターン
					if (strcmp("MOVETYPE", pObjectData) == 0)
					{
						fscanf(pFile, "%s %d", &aObjectData[0], &m_pMoveType[nCntObject]);
					}

					// 移動力の値
					if (strcmp("MOVEPOWER", pObjectData) == 0)
					{
						fscanf(pFile, "%s %f", &aObjectData[0], &m_pMovePower[nCntObject]);

						m_pMovePower[nCntObject] /= 100.0f;
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
void CTextDataObject::Unload(void)
{
	// ファイル名ポインタの開放
	if (m_pFileObjectName != NULL)
	{
		delete[] m_pFileObjectName;
		m_pFileObjectName = NULL;
	}

	// ファイル名読み込みポインタの開放
	if (m_pFileObjectNameLoad != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumFile; nCnt++)
		{
			delete[] m_pFileObjectNameLoad[nCnt];
			m_pFileObjectNameLoad[nCnt] = NULL;
		}

		delete[] m_pFileObjectNameLoad;
		m_pFileObjectNameLoad = NULL;
	}

	// 種類の開放
	if (m_pType != NULL)
	{
		delete[] m_pType;
		m_pType = NULL;
	}

	// 位置の開放
	if (m_pPos != NULL)
	{
		delete[] m_pPos;
		m_pPos = NULL;
	}

	// 角度の開放
	if (m_pRot != NULL)
	{
		delete[] m_pRot;
		m_pRot = NULL;
	}

	// 影の大きさの開放
	if (m_pShadowSize != NULL)
	{
		delete[] m_pShadowSize;
		m_pShadowSize = NULL;
	}

	// オブジェクトの種類の開放
	if (m_pObjAttribute != NULL)
	{
		delete[] m_pObjAttribute;
		m_pObjAttribute = NULL;
	}

	// 移動パターンの開放
	if (m_pMoveType != NULL)
	{
		delete[] m_pMoveType;
		m_pMoveType = NULL;
	}

	// 移動力の割合の開放
	if (m_pMovePower != NULL)
	{
		delete[] m_pMovePower;
		m_pMovePower = NULL;
	}
}

//==============================================================================
// オブジェクトデータの生成
//==============================================================================
CTextDataObject *CTextDataObject::Create(STAGEINFO stage)
{
	// プレイヤーデータの読み込み
	CTextDataObject *pDataObject;
	pDataObject = new CTextDataObject;

	// データのロード
	if (pDataObject != NULL)
	{
		pDataObject->LoadData(stage);
	}

	return pDataObject;
}