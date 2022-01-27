//=============================================================================
//
// エフェクトの設定 [effect.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define EFFECT_SPEED_DOWN		(0.1f)	// 減衰係数
#define EFFECT_NUM_SANDSMOKE	(1)		// 砂煙エフェクト数
#define EFFECT_NUM_LANDING		(20)	// 着地エフェクト数
#define EFFECT_NUM_WATER		(20)	// 水没エフェクト数
#define EFFECT_NUM_LAVA			(1)		// 溶岩落下エフェクト数

// 前方宣言
class CScene2D;

//==============================================================================
// エフェクトクラス
//==============================================================================
class CEffect : public CScene
{
public:
	CEffect(int nPriority = CScene::PRIORITY_EFFECT);
	~CEffect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffect *Create(void);

	//void SetEffect(void);
	void SetVertex(void);
	//void EffectMove(void);

	// 位置の取得・設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }

	// 角度の取得・設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	// 色の取得・設定
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetColor(D3DXCOLOR col) { m_col = col; }

	// サイズの取得・設定
	D3DXVECTOR2 GetSize(void) { return m_size; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

	// 透明度低下値の設定
	float GetAlphaDown(void) { return m_fAlphaDown; }
	void SetAlphaDown(float fAlphaDown) { m_fAlphaDown = fAlphaDown; }

	// 移動力取得・設定
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	float GetMovePower(void) { return m_fMove; }
	void SetMovePower(float fMove) { m_fMove = fMove; }

	// テクスチャの設定
	void SetTexture(char *pText);

private:
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_rot;								// 向き
	D3DXVECTOR2 m_size;								// 大きさ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// バッファへのポインタ
	D3DXMATRIX m_mtxWorld;							// ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;			// テクスチャへのポインタ

	D3DXVECTOR3 m_move;								// 移動力
	D3DXCOLOR m_col;								// 色
	float m_fAlphaDown;								// エフェクトの透明度低下の値

	D3DXVECTOR3 m_rotMove;							// 移動方向
	float m_fMove;									// 移動力基準
};

//==============================================================================
// エフェクト詳細
//==============================================================================
class CEffectDetail : public CEffect
{
public:
	typedef enum
	{
		EFFECTTYPE_NORMAL = 0,		// 通常
		EFFECTTYPE_SANDSMOKE,		// 砂煙
		EFFECTTYPE_LANDING,			// 着地
		EFFECTTYPE_DIVE_WATER,		// 水中へ落ちる
		EFFECTTYPE_DIVE_LAVA,		// 溶岩へ落ちる
		EFFECTTYPE_PARTICLE,		// パーティクルエフェクト
		EFFECTTYPE_GAMECLEAR,		// ゲームクリア演出
		EFFECTTYPE_MAXJUMP,			// 最大ジャンプエフェクト
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffectDetail(int nPriority = CScene::PRIORITY_EFFECT);
	~CEffectDetail();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffectDetail *Create(EFFECTTYPE effectType, D3DXVECTOR3 pos);

	// 位置設定
	void SetPositionMain(D3DXVECTOR3 pos) { m_posMain = pos; }

	// 種類の設定
	void SetType(EFFECTTYPE effectType) { m_effectType = effectType; }

private:
	CEffect **m_ppEffect;		// エフェクトクラス
	CScene2D *m_pClearEffect;	// 2Dポリゴンクラス

	D3DXVECTOR3 m_move;			// 移動力
	D3DXVECTOR3 m_posMain;		// 位置
	int m_nNumEffect;			// エフェクトの個数
	EFFECTTYPE m_effectType;	// エフェクトの種類
};

#endif