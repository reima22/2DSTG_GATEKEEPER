//=============================================================================
//
// 残機数描画処理 [stock.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _STOCK_H_
#define _STOCK_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CNumber;

//==============================================================================
// 残機クラス
//==============================================================================
class CStock : public CScene
{
public:
	CStock(int nPriority = PRIORITY_UI);
	~CStock();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStock *Create(D3DXVECTOR3 pos);
	void SetStock(int nStock);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

private:
	CNumber *m_apNumber;	// ナンバークラスポインタ
	static int m_nStock;	// 残機数値
};

#endif
