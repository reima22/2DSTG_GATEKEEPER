//==============================================================================
//
// テキストデータ（地形）管理〔textdata_terrain.cpp〕
// Author : Jitaku Yo
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "textdata.h"
#include "textdata_terrain.h"
#include "stdio.h"

//==============================================================================
// 地形データの生成
//==============================================================================
CTextDataTerrain *CTextDataTerrain::Create()
{
	// プレイヤーデータの読み込み
	CTextDataTerrain *pDataTerrain = new CTextDataTerrain;

	// データのロード
	if (pDataTerrain != nullptr)
		pDataTerrain->LoadData();

	return pDataTerrain;
}

HRESULT CTextDataTerrain::LoadData()
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	FILE *pFile = fopen(m_pFileName[TEXTDATA_TERRAIN], "r");
	if (pFile == nullptr)
		return E_FAIL;

	bool bReadStart = false;
	bool bReadTerrainSet = false;
	char aLine[128];
	char aText1[128];
	char aText2[128];
	char aAddress[128];

	while (fgets(aLine, 128, pFile) != NULL)
	{
		if (!bReadStart && strcmp(aLine, "#START\n") == 0)//STARTマークを探す
		{
			bReadStart = true;
			continue;
		}
		else if (strcmp(aLine, "#END") == 0 || strcmp(aLine, "#END\n") == 0)//ENDマークを探す
		{
			fclose(pFile);
			break;
		}
		else if (bReadStart)
		{
			sscanf(aLine, "%s", &aText1);
			if (strcmp(aText1, "TERRAIN_NUM") == 0)//モードの数
			{
				sscanf(aLine, "%s = %d", aText1, &m_nTerrainMax);
			}
			if (strcmp(aText1, "#TERRAIN_SET") == 0)//地形の設置を読み込む
			{
				bReadTerrainSet = true;
				m_pTerrainInfo = new CTerrainInfo();
				continue;
			}
			if (bReadTerrainSet && strcmp(aText1, "#END_TERRAIN_SET") == 0)//地形の設置を読み込む終了
			{
				bReadTerrainSet = false;
				vTerrain.push_back(m_pTerrainInfo);
				continue;
			}

			if (bReadTerrainSet)//地形の設定
			{
				if (strcmp(aText1, "TEXTURE") == 0)
				{
					sscanf(aLine, "%s = %s", aText1, aAddress);
					if (FAILED(D3DXCreateTextureFromFile(m_pDevice, aAddress, &m_pTerrainInfo->m_pTexture)))//デバイスを使ってリソースを作る
						return E_FAIL;
				}
				if (strcmp(aText1, "TYPE") == 0)
				{
					sscanf(aLine, "%s = %s", aText1, aText2);
					if (strcmp(aText2, "T_Valley") == 0)
						m_pTerrainInfo->m_Type = CTerrainInfo::TERRAIN_VALLEY;
					if (strcmp(aText2, "T_Ocean") == 0)
						m_pTerrainInfo->m_Type = CTerrainInfo::TERRAIN_OCEAN;
					if (strcmp(aText2, "T_Lava") == 0)
						m_pTerrainInfo->m_Type = CTerrainInfo::TERRAIN_LAVA;
				}
				if (strcmp(aText1, "POS") == 0)
				{
					sscanf(aLine, "%s = %f %f %f",
						aText1,
						&m_pTerrainInfo->m_Pos.x,
						&m_pTerrainInfo->m_Pos.y,
						&m_pTerrainInfo->m_Pos.z);
				}
				if (strcmp(aText1, "ROT") == 0)
				{
					sscanf(aLine, "%s = %f %f %f",
						aText1,
						&m_pTerrainInfo->m_Rot.x,
						&m_pTerrainInfo->m_Rot.y,
						&m_pTerrainInfo->m_Rot.z);
				}
				if (strcmp(aText1, "COLUMN") == 0)
				{
					sscanf(aLine, "%s = %d", aText1, &m_pTerrainInfo->m_nCellsPerCol);
				}
				if (strcmp(aText1, "RAW") == 0)
				{
					sscanf(aLine, "%s = %d", aText1, &m_pTerrainInfo->m_nCellsPerRow);
				}
				if (strcmp(aText1, "SPACE") == 0)
				{
					sscanf(aLine, "%s = %f", aText1, &m_pTerrainInfo->m_fCellSpacing);
				}
				if (strcmp(aText1, "WAVE_SPEED") == 0)
				{
					sscanf(aLine, "%s = %f", aText1, &m_pTerrainInfo->m_W_fSpeed);
				}
				if (strcmp(aText1, "WAVE_FREQUNENCY") == 0)
				{
					sscanf(aLine, "%s = %f", aText1, &m_pTerrainInfo->m_W_fFrequency);
				}
				if (strcmp(aText1, "WAVE_POWER") == 0)
				{
					sscanf(aLine, "%s = %f", aText1, &m_pTerrainInfo->m_W_fPower);
				}
				if (strcmp(aText1, "WAVE_DIRECTION") == 0)
				{
					sscanf(aLine, "%s = %s", aText1, aText2);
					if (strcmp(aText2, "X") == 0)
						m_pTerrainInfo->m_W_dirX = true;
					if (strcmp(aText2, "Z") == 0)
						m_pTerrainInfo->m_W_dirX = false;
				}
			}
		}
	}
	return S_OK;
}

void CTextDataTerrain::Unload()
{
	for (auto pTerrainInfo : vTerrain)
	{
		delete pTerrainInfo;
		pTerrainInfo = nullptr;
	}
	m_pDevice->Release();
	m_pDevice = nullptr;
}


CTerrainInfo* CTextDataTerrain::GetTerrain(int nID)
{
	return vTerrain.at(nID - 1);
}