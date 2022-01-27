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

	static CTextDataMeshfield *Create(void);	// 生成

	//char *GetFileName(int nType) { return m_pFileObjectName[nType]; }				// ファイル名の取得
	//int GetNumObject(void) { return m_nNumObject; }									// オブジェクト数の取得
	//int GetTypeObject(int nIdx) { return m_pType[nIdx]; }								// 種類の取得
	//float GetInfo(int nIdx, OBJECTINFO info) { return m_pObjectInfo[nIdx][info]; }	// 位置・角度の取得
	//float GetShadowWidth(int nIdx) { return m_pShadowWidth[nIdx]; }					// 影の幅
	//float GetShadowDepth(int nIdx) { return m_pShadowDepth[nIdx]; }					// 影の奥行
	//bool GetSavePoint(int nIdx) { return m_pSavePoint[nIdx]; }						// 中間地点の可否

	// 情報取得
	int GetNumMesh(void) { return m_nNumMesh; }											// メッシュ数
	int GetTexType(int nIdx) { return m_pType[nIdx]; }									// テクスチャの種類
	D3DXVECTOR3 GetPosition(int nIdx) { return m_pPos[nIdx]; }
	D3DXVECTOR3 GetRotation(int nIdx) { return m_pRot[nIdx]; }
	D3DXVECTOR2 GetSize(int nIdx) { return m_pSize[nIdx]; }
	int GetWidthPoly(int nIdx) { return m_pWidth[nIdx]; }								// 面の数
	int GetDepthPoly(int nIdx) { return m_pDepth[nIdx]; }								// 面の数

	D3DXVECTOR3 GetPosVtx(int nIdx,int nIdxPoint) { return m_pPosVtx[nIdx][nIdxPoint]; }	// メッシュ頂点情報

private:
	int *m_pType;				// 種類
	D3DXVECTOR3 *m_pPos;		// 位置
	D3DXVECTOR3 *m_pRot;		// 角度
	D3DXVECTOR2 *m_pSize;		// 1面当たりのサイズ
	int *m_pWidth;				// 横幅のポリゴン枚数
	int *m_pDepth;				// 奥行のポリゴン枚数

	int *m_pIdxPoint;			// 総頂点数
	D3DXVECTOR3 **m_pPosVtx;	// 頂点座標

	int m_nNumMesh;				// メッシュの総数
};
#endif