//=============================================================================
//
// ”wŒi•`‰æˆ— [bg.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

// ‘O•ûéŒ¾
//class CScene2D;

//==============================================================================
// ”wŒiƒNƒ‰ƒX
//==============================================================================
class CBg : public CScene
{
public:
	CBg(int nPriority = PRIORITY_BG);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg *Create(void);

	CScene2D *GetScene2D(void) { return m_pScene2D; }

private:
	CScene2D *m_pScene2D;			// CScene2D‚Ìƒ|ƒCƒ“ƒ^

	static D3DXVECTOR3 m_pos;		// ”wŒi‚Ì’†SˆÊ’u
};

#endif