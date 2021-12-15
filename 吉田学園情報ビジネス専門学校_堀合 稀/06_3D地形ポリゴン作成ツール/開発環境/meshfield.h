//==============================================================================
//
// メッシュフィールドの描画〔meshfield.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include "main.h"
#include "scene.h"

// マクロ定義
#define MESH_CHANGE	(1.0f)
//#define MESH_CHANGE_MINI	(0.5f)
#define MESH_AREA_CONTROL	(1.0f)
#define EDIT_RADIUS			(50.0f)

// メッシュフィールドクラス
class CMeshfield : public CScene
{
public:
	CMeshfield();
	~CMeshfield();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshfield *Create(void);

	// 法線の計算
	void CalcuNormal(void);

	// テクスチャの分割処理
	void TexCut(bool bTexCut);

	// 編集処理
	void EditControl(void);
	void ResetVtxPos(void);	// 頂点の高さをすべて0へ
	void LoadTextSet(void);	// 保存した情報を読み込む

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	
	void SetPostion(D3DXVECTOR3 pos) { m_pos = pos; }

	// 角度の取得・設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// 面の数の取得・設定
	int GetWidthNum(void) { return m_nWidth; }
	void SetWidthNum(int nWidth) { m_nWidth = nWidth; }
	int GetDepthNum(void) { return m_nDepth; }
	void SetDepthNum(int nDepth) { m_nDepth = nDepth; }		

	// 長さの取得・設定
	float GetWidthSize(void) { return m_fWidth; }
	void SetWidthSize(float fWidth) { m_fWidth = fWidth; }
	float GetDepthSize(void) { return m_fDepth; }
	void SetDepthSize(float fDepth) { m_fDepth = fDepth; }

	// 頂点数の取得・設定
	int GetWidthPoint(void) { return m_nWidthPoint; }
	void SetWidthPoint(int nWidthPoint) { m_nWidthPoint = nWidthPoint; }
	int GetDepthPoint(void) { return m_nDepthPoint; }
	void SetDepthPoint(int nDepthPoint) { m_nDepthPoint = nDepthPoint; }

	// 総頂点数
	int GetIdxPoint(void) { return m_nIdxPoint; }

	float GetEditArea(void) { return m_fEditRadius; }

	D3DXVECTOR3 GetPosVtx(int nIdx) { return m_posVtx[nIdx]; }	// メッシュの頂点情報取得

	//void SetIdx(int nIdx) { m_nIdx = nIdx; }					// メッシュインデックス設定
	int GetTexIdx(void) { return m_nTexIdx; }
	bool GetTexCut(void) { return m_bTexCut; }
	bool GetWireFrame(void) { return m_bWireFrame; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャへのポインタ
	//int m_nIdx;

	D3DXVECTOR3 *m_posVtx;						// メッシュの各頂点情報

	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 角度
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス

	int m_nAllPoint;							// 総頂点数
	int m_nPolygon;								// ポリゴン数
	int m_nIdxPoint;							// インデックスバッファの必要な確保数
	
	float m_fWidth;								// ポリゴン一つあたりの幅
	float m_fDepth;								// ポリゴン一つあたりの奥行

	int m_nWidth;								// 横幅の面の数
	int m_nDepth;								// 奥行の面の数

	int m_nWidthPoint;							// 横幅の頂点数
	int m_nDepthPoint;							// 奥行の頂点数

	float m_fWidthMax;							// 横幅の最大サイズ
	float m_fDepthMax;							// 奥行の最大サイズ

	int m_nNor;									// 法線を求めるポリゴン数
	D3DXVECTOR3 *m_pNor;						// 法線の値

	int m_nTexIdx;								// テクスチャのインデックス
	bool m_bTexCut;								// テクスチャの分割判定
	bool m_bWireFrame;							// ワイヤーフレーム表示

	float m_fEditRadius;						// 編集する頂点の範囲半径
};

#endif
