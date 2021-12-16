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

// 前方宣言
class CScene2D;	// 2Dポリゴンクラス

// マクロ定義

#define WAVE_HEIGHT_ADJUST		(0.001f)	// 波の高さの調整数値
#define WAVE_DISTANCE_ADJUST	(0.001f)	// 波形間隔の調整数値
#define WAVE_SPEED_ADJUST		(0.001f)	// サイン波の大きさ調整値

#define TEXTURE_UI_POS_0		(D3DXVECTOR3(1060.0f, 50.0f,0.0f))	// UI0の位置
#define TEXTURE_UI_POS_1		(D3DXVECTOR3(1200.0f, 50.0f,0.0f))	// UI1の位置
#define TEXTURE_UI_SIZE			(D3DXVECTOR2(50.0f, 50.0f))			// テクスチャUIのサイズ

//==============================================================================
// メッシュフィールドクラス
//==============================================================================
class CMeshfield : public CScene
{
public:
	typedef enum
	{
		WAVETYPE_NONE = 0,	// 波形無し
		WAVETYPE_WAVE_X,	// 横向き
		WAVETYPE_WAVE_Z,	// 奥向き
		WAVETYPE_WAVE_XZ,	// 双方
		WAVETYPE_CIRCLE,	// 円形
		WAVETYPE_MAX
	}WAVETYPE;

	// テクスチャの合成
	typedef enum
	{
		SYNTHETICTYPE_NONE = 0,		// 合成なし
		SYNTHETICTYPE_ADD,			// 加算合成
		SYNTHETICTYPE_SUBTRA,		// 減算合成
		SYNTHETICTYPE_MULTI,		// 乗算合成
		SYNTHETICTYPE_MAX
	}SYNTHETICTYPE;

	// テクスチャの情報
	typedef enum
	{
		TEXTUREINFO_0 = 0,		// 一つ目
		TEXTUREINFO_1,			// 二つ目
		TEXTUREINFO_MAX
	}TEXTUREINFO;

	// テクスチャ移動の方法
	typedef enum
	{
		TEXMOVE_STRAIGHT = 0,	// 直線
		TEXMOVE_WAVE,			// 蛇行
		TEXMOVE_MAX
	}TEXMOVE;

	CMeshfield();
	~CMeshfield();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshfield *Create(void);

	void CalcuNormal(void);	// 法線の計算
	void MeshWave(void);	// 波形
	void ControlMesh(void);	// メッシュの操作

	void CreateUI(void);	// 2DポリゴンUIの生成
	void UninitUI(void);	// 2DポリゴンUIの破棄
	void UpdateUI(void);	// 2DポリゴンUIの更新

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

	// 波形情報の取得
	// 文字列
	char *GetWaveTypeChar(void) { return m_pWaveType[m_waveType]; }
	char *GetSyntheticTypeChar(void) { return m_pSyntheticType[m_syntheticType]; }
	char *GetTexChar(void) { return m_pTex[m_texInfo]; }
	char *GetWireONOFF(void) { return m_apOnOff[m_bWireFrame]; }		// オンオフの文字列

	// 詳細の取得
	float GetHeightWave(void) { return m_fHeightWave; }					// 波の高さ
	float GetDistanceWave(void) { return m_fDistanceWave; }				// 波の間隔
	float GetSpeedWave(void) { return m_fSpeedWave; }					// 波の速さ
	SYNTHETICTYPE GetSyntheticType(void) { return m_syntheticType; }	// 合成方法
	WAVETYPE GetWaveType(void) { return m_waveType; }					// 波の方向
	TEXTUREINFO GetTex(void) { return m_texInfo; }						// 操作中のテクスチャ
	bool GetCutTex(void) { return m_bCutTex; }							// テクスチャの分割有無
	bool GetWireFrame(void) { return m_bWireFrame; }					// ワイヤーフレームの有効

	float GetTexMoveRot(int nIdx) { return m_aTexMoveRot[nIdx]; }		// テクスチャの流れる方向
	float GetTexMove(int nIdx) { return m_aTexMoveSpeed[nIdx]; }		// テクスチャの流れる速さ倍率
	int GetTexIdx(int nIdx) { return m_aTextureIdx[nIdx]; }				// テクスチャのインデックス

	// 総頂点数
	int GetIdxPoint(void) { return m_nIdxPoint; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture[TEXTUREINFO_MAX] = {};	// テクスチャへのポインタ

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
	int m_nIdxNor;								// 操作するインデックス

	/**/

	int m_nCntWave;								// 波形アニメーションのカウント

	float m_fHeightWave;						// 波の高さ
	float m_fDistanceWave;						// 波形間隔
	float m_fSpeedWave;							// 波の速さ

	SYNTHETICTYPE m_syntheticType;				// 合成の種類
	WAVETYPE m_waveType;						// 波の種類
	bool m_bCutTex;								// テクスチャ分割の有無
	bool m_bWireFrame;							// ワイヤーフレームの有効化		

	TEXMOVE	m_texMove;							// テクスチャ座標移動方法
	TEXTUREINFO m_texInfo;						// 操作中のテクスチャ情報

	// テクスチャの情報
	D3DXVECTOR2 m_aTexMove[TEXTUREINFO_MAX];	// 流れる移動力
	float m_aTexMoveRot[TEXTUREINFO_MAX];		// テクスチャ座標の流れる向き(Y軸)
	float m_aTexMoveSpeed[TEXTUREINFO_MAX];		// 流れる速さの倍率
	int m_nTextureNum;							// テクスチャの総数
	int m_aTextureIdx[TEXTUREINFO_MAX];			// テクスチャインデックス

	CScene2D *m_apScene2D[TEXTUREINFO_MAX];		// テクスチャを描画するUI

	// 表記文字列
	char *m_pWaveType[WAVETYPE_MAX] =
	{
		{ "波形無し" },
		{ "X軸方向" },
		{ "Z軸方向" },
		{ "XZ軸双方" },
		{ "円形波紋" }
	};

	char *m_pSyntheticType[SYNTHETICTYPE_MAX] =
	{
		{ "合成無し" },
		{ "加算合成" },
		{ "減算合成" },
		{ "乗算合成" }
	};

	char *m_pTex[TEXTUREINFO_MAX] = 
	{
		{ "テクスチャ(下)" },
		{ "テクスチャ(上)" }
	};

	char *m_apOnOff[2] =
	{
		{"OFF"},
		{"ON"}
	};
};

#endif
