//=============================================================================
//
// メッセージ描画処理 [message.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MASSAGE_H_
#define _MASSAGE_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define MESSAGE_SIZEX	(400.0f)	// 幅
#define MESSAGE_SIZEY	(120.0f)	// 高さ

// 前方宣言
class CPolygon;

//==============================================================================
// メッセージクラス
//==============================================================================
class CMessage
{
public:
	// 開始時メッセージの段階
	typedef enum
	{
		MESSAGE_START_START = 0,	// 開始
		MESSAGE_START_IN,			// フェードインで表示
		MESSAGE_START_ON,			// 表示している
		MESSAGE_START_OUT,			// フェードアウトで消去
		MESSAGE_START_END,			// 終了
		MESSAGE_START_MAX
	}MESSAGE_START;

	CMessage();
	~CMessage();
	HRESULT Init(D3DXVECTOR3 pos);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	static CMessage *Create(D3DXVECTOR3 pos);

	static void ChangeOnEnd(void) { m_bStartMessageEnd = true; }
	static bool GetEnd(void) { return m_bStartMessageEnd; }

protected:
	static int m_nStartCnt;			// ゲーム開始時演出カウント
	static bool m_bStartMessageEnd;	// メッセージの終了
};
#endif
