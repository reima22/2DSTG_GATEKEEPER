//=============================================================================
//
// ゲーム結果ロゴ描画処理 [gamesetlogo.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _GAMESETLOGO_H_
#define _GAMESETLOGO_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define LOGO_POS	(D3DXVECTOR3(640.0f, 360.0f,0.0f))
#define LOGO_SIZE	(D3DXVECTOR2(480.0f,80.0f))

//==============================================================================
// ゲーム結果ロゴクラス
//==============================================================================
class CGamesetLogo : public CScene
{
public:
	CGamesetLogo(int nPriority = PRIORITY_UI);
	~CGamesetLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGamesetLogo *Create(void);

private:
	CScene2D *m_pScene2D;	// 2Dクラスポインタ
};
#endif