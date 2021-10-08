//==============================================================================
//
// メッセージテキスト描画処理〔message_text.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
#include "message.h"
#include "message_text.h"
#include "message_window.h" 
#include "fade.h"

// 静的メンバ変数宣言
CMessage::MESSAGE_START CMessageText::m_StartState;

//==============================================================================
// コンストラクタ
//==============================================================================
CMessageText::CMessageText(int nPriority) : CScene2D(nPriority)
{
	m_StartState = CMessage::MESSAGE_START_START;

	m_nStartCnt = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CMessageText::~CMessageText()
{
}

//==============================================================================
// メッセージテキストの初期化処理
//==============================================================================
HRESULT CMessageText::Init(D3DXVECTOR3 pos)
{
	// 初期化	
	CScene2D::Init(pos);
	CScene2D::SetSize(MESSAGE_SIZEX, MESSAGE_SIZEY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);

	// 色の初期設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//==============================================================================
// メッセージの終了処理
//==============================================================================
void CMessageText::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// メッセージの更新処理
//==============================================================================
void CMessageText::Update(void)
{
	SetState();

	CScene2D::Update();
}

//==============================================================================
// メッセージの描画処理
//==============================================================================
void CMessageText::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// メッセージの生成処理
//==============================================================================
CMessageText *CMessageText::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CMessageText *pMessageText;
	pMessageText = new CMessageText;

	// 初期化
	pMessageText->Init(pos);

	pMessageText->BindTexture(CTexture::TEXTYPE_MESSAGE_TEXT);

	return pMessageText;
}

//==============================================================================
// メッセージの状態
//==============================================================================
void CMessageText::SetState(void)
{
	// フェード状態の取得
	CFade::FADE fade = CFade::GetFade();

	// ウィンドウの状態取得
	CMessage::MESSAGE_START startWindow = CMessageWindow::GetState();

	if (fade == CFade::FADE_NONE)
	{
		switch (m_StartState)
		{
		case CMessage::MESSAGE_START_START:	// メッセージ表示開始
			// ウィンドウが表示した
			if (startWindow == CMessage::MESSAGE_START_ON)
			{
				// フェードインへ
				m_StartState = CMessage::MESSAGE_START_IN;
				CLine::SetState(CLine::LINE_STATE_TUTORIAL);
			}

			break;

		case CMessage::MESSAGE_START_IN:		// メッセージのフェードイン
			m_col.a += 0.01f;

			if (m_col.a >= 1.0f)
			{// 表示完了
				m_StartState = CMessage::MESSAGE_START_ON;
			}
			break;

		case CMessage::MESSAGE_START_ON:		// メッセージの表示
			// 表示間のカウント
			m_nStartCnt++;

			if (m_nStartCnt > 180)
			{// 約3秒経過
				m_StartState = CMessage::MESSAGE_START_OUT;
			}

			break;

		case CMessage::MESSAGE_START_OUT:		// メッセージのフェードアウト
			m_col.a -= 0.01f;
			if (m_col.a <= 0.0f)
			{
				m_StartState = CMessage::MESSAGE_START_END;
				CLine::SetState(CLine::LINE_STATE_NONE);
			}

			break;

		case CMessage::MESSAGE_START_END:		// メッセージ終了

			break;
		}
	}

	// 色の設定
	CScene2D::SetCol(m_col);
}