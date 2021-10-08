//=============================================================================
//
// ライトの設定 [light.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

// マクロ定義
#define LIGHT_NUM	(3)

//==============================================================================
// ライトクラス
//==============================================================================
class CLight
{
public:
	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[LIGHT_NUM];	// ライト情報
};
#endif