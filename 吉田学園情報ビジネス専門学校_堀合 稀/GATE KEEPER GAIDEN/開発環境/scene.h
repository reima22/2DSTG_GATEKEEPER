//=============================================================================
//
// オブジェクト描画処理 [scene.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D_1	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2)

//==============================================================================
// 頂点フォーマットに合わせた構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 1.0で固定
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標0
	D3DXVECTOR2 tex1;	// テクスチャ座標1
} VERTEX_3D_1;

//// 前方宣言
//class CScene2D;

// オブジェクトクラス
class CScene
{
public:
	// オブジェクトの種類
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BG,			// 背景
		OBJTYPE_PLAYER,		// プレイヤー
		OBJTYPE_ENEMY,		// 敵
		OBJTYPE_BULLET,		// 弾
		OBJTYPE_ITEM,		// アイテム
		OBJTYPE_EXPLOSION,	// 爆発
		OBJTYPE_EFFECT,		// エフェクト
		OBJTYPE_UI,			// UI系
		OBJTYPE_MAX
	}OBJTYPE;

	// 描画優先順位
	typedef enum
	{
		PRIORITY_BG = 0,	// 背景
		PRIORITY_DEFAULT,	// デフォルト値
		PRIORITY_EFFECT,	// エフェクト
		PRIORITY_BULLET,	// 弾
		PRIORITY_CHARA,		// キャラクター
		PRIORITY_ITEM,		// アイテム
		PRIORITY_EXPLOSION,	// 爆発
		PRIORITY_EFFECT_ON,
		PRIORITY_UI,		// UI
		PRIORITY_PAUSE,		// ポーズ
		PRIORITY_MESSAGE,	// メッセージ
		PRIORITY_MAX
	}Priority;

	CScene(int nPriority = PRIORITY_DEFAULT);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void);

	// シーンの取得
	static CScene *GetScene(int nPriority);
	static CScene *GetSceneNext(CScene *pScene);
	
	virtual D3DXVECTOR3 GetPosition(void) = 0;	// 位置取得の仮想関数
	virtual D3DXVECTOR3 GetSize(void) = 0;		// サイズ取得の仮想関数

	static int GetNumAll(void) { return m_nNumAll; }

protected:
	void Release(void);

private:
	static CScene *m_pTop[PRIORITY_MAX];	// 先頭のオブジェクトへのポインタ
	static CScene *m_pCur[PRIORITY_MAX];	// 現在(最後尾)のオブジェクトへのポインタ
	CScene *m_pPrev;						// 前のオブジェクトへのポインタ
	CScene *m_pNext;						// 次のオブジェクトへのポインタ
	bool m_bDeath;							// 死亡フラグ

	D3DXVECTOR3 m_pos;
	static int m_nNumAll;					// 生成数
	int m_nID;								// オブジェクト順番
	OBJTYPE m_objType;						// オブジェクトの種類
};
#endif