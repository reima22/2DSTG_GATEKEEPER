//=============================================================================
//
// メッセージテキスト描画処理 [message_text.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MASSAGE_TEXT_H_
#define _MASSAGE_TEXT_H_

#include "main.h"
#include "scene.h"
#include "message.h"

// 前方宣言
class CPolygon;

//==============================================================================
// メッセージテキストクラス
//==============================================================================
class CMessageText : public CScene2D
{
public:
	CMessageText(int nPriority = CScene::PRIORITY_MESSAGE);
	~CMessageText();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMessageText *Create(D3DXVECTOR3 pos);
	void SetState(void);
	static CMessage::MESSAGE_START GetState(void) { return m_StartState; }

private:
	int m_nStartCnt;
	static CMessage::MESSAGE_START m_StartState;	// 開始時メッセージの状態
	D3DXCOLOR m_col;
};
#endif