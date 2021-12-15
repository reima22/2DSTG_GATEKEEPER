//=============================================================================
//
// テキストデータ（メッシュフィールド）管理 [textdata_meshfield.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_MESHFIELD_H_
#define _TEXTDATA_MESHFIELD_H_

#include "main.h"
#include "textdata.h"
#include "meshfield.h"

//==============================================================================
// テキスト管理（メッシュフィールド）クラス
//==============================================================================
class CTextDataMeshfield : public CTextData
{
public:
	CTextDataMeshfield();
	~CTextDataMeshfield();

	void LoadData(void);
	void Unload(void);

	void SaveData(CMeshfield *pMesh);

	static CTextDataMeshfield *Create(void);	// 生成


	// 情報取得
	int GetTexType(void) { return m_nType; }								// テクスチャの種類
	D3DXVECTOR3 GetPosition(void) { return m_pos; }							// 位置
	D3DXVECTOR3 GetRotation(void) { return m_rot; }							// 角度
	D3DXVECTOR2 GetSize(void) { return m_size; }							// サイズ
	int GetWidthPoly(void) { return m_nWidth; }								// 面の数
	int GetDepthPoly(void) { return m_nDepth; }								// 面の数
	D3DXVECTOR3 GetPosVtx(int nIdxPoint) { return m_pPosVtx[nIdxPoint]; }	// メッシュ頂点情報

private:
	int m_nNumFile;			// ファイル名の総数
	int m_nType;			// 種類
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR2 m_size;		// 1面当たりのサイズ
	int m_nWidth;			// 横幅のポリゴン枚数
	int m_nDepth;			// 奥行のポリゴン枚数
	int m_nIdxPoint;		// 総頂点数
	D3DXVECTOR3 *m_pPosVtx;	// 頂点座標
};
#endif