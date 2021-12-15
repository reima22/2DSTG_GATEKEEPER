//=============================================================================
//
// メッセージウィンドウ描画処理 [message_window.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MASSAGE_WINDOW_H_
#define _MASSAGE_WINDOW_H_

#include "main.h"
#include "scene.h"
#include "message.h"

// 前方宣言
class CPolygon;

//==============================================================================
// メッセージウィンドウクラス
//==============================================================================
class CMessageWindow : public CScene2D
{
public:
	CMessageWindow(int nPriority = CScene::PRIORITY_MESSAGE);
	~CMessageWindow();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMessageWindow *Create(D3DXVECTOR3 pos);
	void SetState(void);
	static CMessage::MESSAGE_START GetState(void) { return m_StartState; }

private:
	static CMessage::MESSAGE_START m_StartState;	// 開始時メッセージの状態
	static D3DXCOLOR m_col;
};
#endif
