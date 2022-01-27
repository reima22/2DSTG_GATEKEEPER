//=============================================================================
//
// テキストデータ（地形）管理 [textdata_Terrain.h]
// Author : Jitaku Yo
//
//============================================================================= 
#ifndef _TEXTDATA_TERRAIN_H_
#define _TEXTDATA_TERRAIN_H_

#include "main.h"
#include "textdata.h"
#include "manager.h"
#include "Renderer.h"
//==============================================================================
//地形情報についてのクラス
//==============================================================================
class CTerrainInfo
{
public:
	typedef enum
	{
		TERRAIN_VALLEY = 1,
		TERRAIN_OCEAN,
		TERRAIN_LAVA,
		TERRAIN_MAX
	}TERRAIN_TYPE;
	CTerrainInfo() :
		m_nCellsPerRow(0),
		m_nCellsPerCol(0),
		m_fCellSpacing(0),
		m_W_fSpeed(0),
		m_W_fFrequency(0),
		m_W_fPower(0),
		m_W_dirX(false),
		m_Pos(D3DXVECTOR3(0, 0, 0)),
		m_Rot(D3DXVECTOR3(0, 0, 0)),
		m_pTexture(nullptr),
		m_Type(TERRAIN_MAX){};
	~CTerrainInfo() {
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	int m_nCellsPerRow;
	int m_nCellsPerCol;
	float m_fCellSpacing;
	float m_W_fSpeed;
	float m_W_fFrequency;
	float m_W_fPower;
	bool m_W_dirX;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	LPDIRECT3DTEXTURE9 m_pTexture;
	TERRAIN_TYPE m_Type;
};

//==============================================================================
// テキスト管理（system）クラス
//==============================================================================
class CTextDataTerrain : public CTextData
{
public:
	CTextDataTerrain() :
		m_pDevice(CManager::GetRenderer()->GetDevice()),
		m_pTerrainInfo(nullptr),
		m_nTerrainMax(0){};

	HRESULT LoadData();
	void Unload();
	static CTextDataTerrain *Create();	// 生成
	CTerrainInfo* GetTerrain(int nID);
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	CTerrainInfo* m_pTerrainInfo;
	vector<CTerrainInfo*> vTerrain;
	int m_nTerrainMax;
};
#endif
