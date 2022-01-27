//=============================================================================
//
// ランキングロゴ描画処理 [ranking_logo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RANKING_LOGO_H_
#define _RANKING_LOGO_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define RANK_LOGO_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 70.0f, 0.0f))	// 通常遷移時の配置
#define RANK_LOGO_POS_T	(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f))	// 自動遷移時の配置
#define RANK_LOGO_SIZE	(D3DXVECTOR2(240.0f, 40.0f))

//==============================================================================
// ランキングロゴクラス
//==============================================================================
class CRankingLogo : public CRanking
{
public:
	CRankingLogo(int nPriority = PRIORITY_UI);
	~CRankingLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRankingLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, char *pTypeCheck);

private:
	CScene2D *m_pScene2D;
};
#endif
