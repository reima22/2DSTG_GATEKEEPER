//=============================================================================
//
// 落下数UI描画処理 [fall.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _FALL_H_
#define _FALL_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define FALL_UIBASE_POS		(D3DXVECTOR3(1130.0f, 50.0f, 0.0f))	// UIベースの位置
#define FALL_UIBASE_SIZE	(D3DXVECTOR2(150.0f, 50.0f))		// UIベースのサイズ

#define FALL_LOGO_POS		(D3DXVECTOR3(1070.0f, 50.0f, 0.0f))	// ロゴの位置
#define FALL_LOGO_SIZE		(D3DXVECTOR2(60.0f, 40.0f))			// ロゴのサイズ

// 前方宣言
class CFallCount;		// 落下数カウントクラス
class CFallLogo;		// 落下数ロゴクラス

//==============================================================================
// 落下数UIクラス
//==============================================================================
class CFall : public CScene
{
public:
	CFall(int nPriority = PRIORITY_UI);
	~CFall();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFall *Create(void);

	CFallCount *GetFallCount(void) { return m_pFallCount; }

private:
	CFallCount *m_pFallCount;	// 落下数カウントクラスポインタ
	CFallLogo *m_pFallLogo;		// 落下数ロゴクラスポインタ
};

#endif