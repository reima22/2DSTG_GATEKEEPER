//=============================================================================
//
// 描画優先低エフェクト描画処理 [effect_on.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _EFFECT_ON_H_
#define _EFFECT_ON_H_

#include "main.h"
#include "scene2D.h"

// 描画優先低エフェクトクラス
class CEffectOn : public CScene2D
{
public:
	// エフェクトの種類
	typedef enum
	{
		EFFECT_TYPE_PARTICLE = 0,			// 拡散パーティクル
		EFFECT_TYPE_PARTICLE_DIRECTION,		// 指向性パーティクル
		EFFECT_TYPE_WAVE,					// 波紋型
		EFFECT_TYPE_PARTICLE_UNDER,			// 下向きパーティクル
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffectOn(int nPriority = PRIORITY_EFFECT_ON);
	~CEffectOn();
	HRESULT Init(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffectOn *Create(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern);

private:
	D3DXCOLOR m_col;						// 色
	float m_fRadius;						// 半径
	float m_fAlphaDown;						// 明度低下値
	D3DXVECTOR3 m_Move;						// 移動量
	EFFECT_TYPE m_effectType;				// エフェクトの種類
};

#endif
