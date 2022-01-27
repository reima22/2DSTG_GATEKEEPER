//===================================================================
//HeaderFile: Terrain.h
//Author : Yo jitaku
//===================================================================
#ifndef _Terrain_H_
#define _Terrain_H_
#include "main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "textdata_Terrain.h"

class CWave
{
public:
	typedef enum
	{
		WAVE_LINE = 0,
		WAVE_WATERSURFACE,
		WAVE_MAX
	}WAVE_TYPE;
	CWave() :
		m_bWaveLine(false),
		m_bWaterSurface(false),
		m_DirX(false),
		m_fPower(0),
		m_fFrequency(0),
		m_fSpeed(0),
		m_fStartPosY(0) {};
	void SetWaveLine(float power, float fre, float speed, bool bDir);
	void SetWaterSurface(float power, float fre, float speed);
	bool m_bWaveLine;
	bool m_bWaterSurface;
	bool m_DirX;
	float m_fPower;
	float m_fFrequency;
	float m_fSpeed;
	float m_fStartPosY;
	static const int EffectRange = 5;
};
//*******************************************************************
//地形クラス
//*******************************************************************
class CTerrain
{
public:
	CTerrain() :
		m_pDevice(CManager::GetRenderer()->GetDevice()),
		m_pVtxBuff(nullptr),
		m_pIdxBuff(nullptr),
		m_pDataTerrain(nullptr),
		m_pManager(new CManager),
		m_pWave(new CWave),
		m_nVertsPerRow(0),
		m_nVertsPerCol(0),
		m_nNumVertices(0),
		m_nCells(0),
		m_nNumTriangles(0),
		m_nNumIndex(0),
		m_nCntTime(0),
		m_fTerrainWidth(0),
		m_fTerrainDepth(0),
		m_ValidZoneMin(D3DXVECTOR3(0, 0, 0)),
		m_ValidZoneMax(D3DXVECTOR3(0, 0, 0)) {};
	~CTerrain()
	{
		if (m_pDevice != nullptr)
		{
			m_pDevice->Release();
			m_pDevice = nullptr;
		}
		if (m_pTexture != nullptr)
		{
			m_pTexture->Release();
			m_pTexture = nullptr;
		}
		if (m_pVtxBuff != nullptr)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = nullptr;
		}
		if (m_pIdxBuff != nullptr)
		{
			m_pIdxBuff->Release();
			m_pIdxBuff = nullptr;
		}
		delete m_pManager;
		m_pManager = nullptr;
		delete m_pWave;
		m_pWave = nullptr;
	}
	static CTerrain *Create(CTerrainInfo::TERRAIN_TYPE nType);				//地形の初期化
	HRESULT Init(CTerrainInfo::TERRAIN_TYPE nType);
	void Update();						//更新
	void Draw();						//地形の描画
	bool FallCollider(CScene* pGameObject);//当たり判定関数
	CTerrainInfo::TERRAIN_TYPE GetType(void) { return m_type; }
	vector<D3DXVECTOR3> vVertexBuffer;				//頂点バッファのコピー
	vector<int> vIndexBuffer;						//Indexバッファのコピー
private:
	void WaveLineUpdate();
	void WaterSurfaceUpdate();
	void TerrainMove();
	const int MAX_PRIMITIVE_COUNT = 60000;			//描画出来る最大値
	const int TERRAIN_HEIGHT = 0;					//地形の高さ
	const float TERRAIN_MOVE_SPEED = 0.0005f;		//画像移動速度
	LPDIRECT3DDEVICE9 m_pDevice;					//デバイスを取る
	LPDIRECT3DTEXTURE9 m_pTexture;					//Textureのポインター
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;				//頂点バッファ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;				//Indexバッファ
	CTerrainInfo* m_pDataTerrain;
	CManager* m_pManager;
	CWave* m_pWave;
	int m_nVertsPerRow;					//1行あたりの頂点の数
	int m_nVertsPerCol;					//1列あたりの頂点の数
	unsigned int m_nNumVertices;		//頂点の総数
	unsigned int m_nCells;				//セル数
	unsigned int m_nNumTriangles;		//描画三角形数
	unsigned int m_nNumIndex;			//Indexの番号
	unsigned int m_nCntTime;			//時間count
	float m_fTerrainWidth;				//地形の幅
	float m_fTerrainDepth;				//地形の深さ
	D3DXVECTOR3 m_ValidZoneMin;			//地形の範囲の最小値
	D3DXVECTOR3 m_ValidZoneMax;			//地形の範囲の最大値
	CTerrainInfo::TERRAIN_TYPE m_type;
};

class CWaterSurface : public CTerrain
{

};
#endif