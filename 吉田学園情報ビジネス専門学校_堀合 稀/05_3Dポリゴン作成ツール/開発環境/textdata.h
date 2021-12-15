//=============================================================================
//
// テキストデータ管理 [textdata.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_H_
#define _TEXTDATA_H_

#include "main.h"
#include "meshfield.h"

// マクロ定義
#define TEXT_NUM	(128)	// テキストデータバイト数

//==============================================================================
// テキスト管理クラス
//==============================================================================
class CTextData
{
public:
	// テキストデータの種類
	typedef enum
	{
		TEXTDATA_TEXTURE = 0,		// テクスチャデータ
		TEXTDATA_MESHFIELD,		// メッシュフィールドデータ
		TEXTDATA_MAX
	}TEXTDATA;

	CTextData();
	~CTextData();

	static CTextData *Create(void);	// 生成

	void LoadText(void);
	void UnloadText(void);

	void SaveText(void);

	// データの取得
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			// 位置
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			// 角度
	D3DXVECTOR2 GetSize(void) { return m_size; }			// サイズ
	int GetWidthNum(void) { return m_nWidth; }				// 面の横幅枚数
	int GetDepthNum(void) { return m_nDepth; }				// 面の奥行枚数
	int GetType(void) { return m_nType; }					// テクスチャ種類
	int GetTypeSub(void) { return m_nTypeSub; }				// テクスチャ2枚目
	float GetHeightWave(void) { return m_fHeightWave; }		// 波の高さ
	float GetDistanceWave(void) { return m_fDistanceWave; }	// 波の間隔
	float GetSpeedWave(void) { return m_fSpeedWave; }			// 波の早さ

	int GetSyntheticType(void) { return m_nSyntheticType; }	// 合成方法
	int GetWaveType(void) { return m_nWaveType; }			// 波の方向
	bool GetCutTex(void) { return m_bCutTex; }				// テクスチャの分割
	float GetTexMoveRot(int nIdx) { return m_fTexMoveRot[nIdx]; }	// テクスチャの流れる方向
	float GetTexMove(int nIdx) { return m_fTexMove[nIdx]; }			// テクスチャの移動力
	int GetNumTexture(void) { return m_nNumTexture; }
	char *GetFileName(int nIdx) { return m_pFileTextureName[nIdx]; }

private:
	int m_nNumTexture;			// テクスチャの数
	char **m_pFileTextureName;	// ファイル名を格納するポインタ
	int *m_pIdx;				// テクスチャのインデックス

	int m_nType;				// テクスチャ種類
	int m_nTypeSub;				// テクスチャ2枚目

	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 角度
	D3DXVECTOR2 m_size;			// 1面当たりのサイズ
	int m_nWidth;				// 横幅のポリゴン枚数
	int m_nDepth;				// 奥行のポリゴン枚数

	int m_nIdxPoint;			// 総頂点数
	float m_fHeightWave;		// 波の高さ
	float m_fDistanceWave;		// 波の間隔
	float m_fSpeedWave;			// 波の早さ

	int m_nSyntheticType;		// 描画合成方法
	int m_nWaveType;			// 波の向き
	bool m_bCutTex;				// テクスチャの分割

	float m_fTexMoveRot[CMeshfield::TEXTUREINFO_MAX];		// テクスチャの流れる方向
	float m_fTexMove[CMeshfield::TEXTUREINFO_MAX];		// テクスチャの移動力


};
#endif