//=============================================================================
//
// アイテム描画処理 [item.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "scene2D.h"

// マクロ定義
#define ITEM_X			(20.0f)	// アイテムの大きさX軸
#define ITEM_Y			(20.0f)	// アイテムの大きさY軸
#define ITEM_BLINK		(0.02f)	// α値の変化する値
#define ITEM_BLINK_MIN	(0.5f)	// α値の最小
#define ITEM_BLINK_MAX	(1.0f)	// α値の最大
#define ITEM_CHASE_AREA (50.0f)	// 追従する範囲

// アイテムクラス
class CItem : public CScene2D
{
public:
	// アイテムの種類
	typedef enum
	{
		ITEM_TYPE_0 = CTexture::TEXTYPE_ITEM_0,	// ウェーブ取得
		ITEM_TYPE_1,							// ミサイル取得
		ITEM_TYPE_2,							// リバースウェーブ取得
		ITEM_TYPE_3,							// 回復アイテム
		ITEM_TYPE_MAX
	}ITEM_TYPE;

	CItem(int nPriority = PRIORITY_ITEM);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, 
		OBJTYPE objType, 
		float fSizeX, 
		float fSizeY, 
		D3DXVECTOR3 move, 
		CTexture::TEXTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CItem *Create(
		D3DXVECTOR3 pos,
		OBJTYPE objType,
		float fSizeX, float fSizeY,
		D3DXVECTOR3 move,
		CTexture::TEXTYPE type);

	static void RandCreate(D3DXVECTOR3 pos);

	void SetType(CTexture::TEXTYPE texType);

	void SetBlink(void);

	void SetChaseMove(D3DXVECTOR3 posItem);

	CItem::ITEM_TYPE GetType(void) { return m_Type; }

	// 追従開始
	void SetChase(void) { m_bChase = true; }

private:
	D3DXVECTOR3 m_move;					// 移動力
	ITEM_TYPE m_Type;					// アイテムの種類
	D3DXCOLOR m_col;					// 色
	int m_nBlinkCnt;					// 点滅カウント
	static int m_nSetHealCnt;			// 回復アイテム発生カウント
	bool m_bBlinkUp;					// 点滅フェイズ
	bool m_bChase;						// アイテムの追尾判定
};

#endif
