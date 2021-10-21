//==============================================================================
//
// メッセージウィンドウ描画処理〔message_window.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "message.h"
#include "message_text.h"
#include "message_window.h" 
#include "fade.h"

// 静的メンバ変数宣言
CMessage::MESSAGE_START CMessageWindow::m_StartState;
D3DXCOLOR CMessageWindow::m_col;

//==============================================================================
// コンストラクタ
//==============================================================================
CMessageWindow::CMessageWindow(int nPriority) : CScene2D(nPriority)
{
	m_StartState = CMessage::MESSAGE_START_START;
}

//==============================================================================
// デストラクタ
//==============================================================================
CMessageWindow::~CMessageWindow()
{
}

//==============================================================================
// メッセージの初期化処理
//==============================================================================
HRESULT CMessageWindow::Init(D3DXVECTOR3 pos)
{
	// 初期化
	CScene2D::Init(pos);
	CScene2D::SetSize(MESSAGE_SIZEX + 10.0f, MESSAGE_SIZEY + 20.0f);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);

	// 色の初期設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//==============================================================================
// メッセージの終了処理
//==============================================================================
void CMessageWindow::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// メッセージの更新処理
//==============================================================================
void CMessageWindow::Update(void)
{
	SetState();

	// 更新
	CScene2D::Update();
}

//==============================================================================
// メッセージの描画処理
//==============================================================================
void CMessageWindow::Draw(void)
{
	// 描画
	CScene2D::Draw();
}

//==============================================================================
// メッセージの生成処理
//==============================================================================
CMessageWindow *CMessageWindow::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CMessageWindow *pMessageWindow;
	pMessageWindow = new CMessageWindow;

	// 初期化
	pMessageWindow->Init(pos);

	pMessageWindow->BindTexture(CTexture::TEXTYPE_MESSAGE_WINDOW);

	return pMessageWindow;
}

//==============================================================================
// メッセージの状態
//==============================================================================
void CMessageWindow::SetState(void)
{
	// フェード状態の取得
	CFade::FADE fade = CFade::GetFade();

	// メッセージテキスト状態の取得
	CMessage::MESSAGE_START startText = CMessageText::GetState();

	if (fade == CFade::FADE_NONE)
	{
		switch (m_StartState)
		{
		case CMessage::MESSAGE_START_START:	// メッセージ表示開始
			// フェードインへ
			m_StartState = CMessage::MESSAGE_START_IN;

			break;

		case CMessage::MESSAGE_START_IN:		// メッセージのフェードイン
			m_col.a += 0.01f;

			if (m_col.a >= 1.0f)
			{// 表示完了
				m_StartState = CMessage::MESSAGE_START_ON;
			}

			break;

		case CMessage::CMessage::MESSAGE_START_ON:		// メッセージの表示
			if (startText == CMessage::MESSAGE_START_END)
			{
				m_StartState = CMessage::MESSAGE_START_OUT;
			}
			break;

		case CMessage::MESSAGE_START_OUT:		// メッセージのフェードアウト
			m_col.a -= 0.01f;

			if (m_col.a <= 0.0f)
			{// 消去完了
				m_StartState = CMessage::MESSAGE_START_END;
			}

			break;

		case CMessage::MESSAGE_START_END:		// メッセージ終了
			CMessage::ChangeOnEnd();
			break;
		}
	}

	// 色の設定
	CScene2D::SetCol(m_col);
}