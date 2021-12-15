//=============================================================================
//
// ライフ描画処理 [life.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define LIFE_MAX	(5)	// 最大ライフ数

// 前方宣言
class CPolygon;
class CCharacter;

//==============================================================================
// スコアクラス
//==============================================================================
class CLife : public CScene2D
{
public:
	CLife(int nPriority = PRIORITY_UI);
	~CLife();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLife *Create(D3DXVECTOR3 pos);

	static int GetLifeMax(void) { return m_nLifeMax; }

private:
	CCharacter *m_pChara;
	static CPolygon **m_apPolygon;	// ナンバークラスダブルポインタ
	static int m_nLife;				// ライフ数
	static int m_nLifeMax;			// ライフの最大値
};
#endif
