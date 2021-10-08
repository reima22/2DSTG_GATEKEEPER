//==============================================================================
//
// 弾UI描画処理〔bullet_UI.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "UI.h"
#include "bullet_UI.h"
#include "polygon.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "item.h"
#include "textdata_player.h"
#include "effect.h"

// 静的メンバ変数宣言
CScene2D *CBulletUI::m_apScene2D[BUITYPE_MAX];
int CBulletUI::m_nUse = 0;
bool CBulletUI::m_bTake[BUITYPE_MAX];
D3DXVECTOR2 CBulletUI::m_size[BUITYPE_MAX];

//==============================================================================
// コンストラクタ
//==============================================================================
CBulletUI::CBulletUI(int nPriority) : CScene2D(nPriority)
{
	// ポリゴンの初期化
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
		m_aPattern[nCnt] = 0;
	}

	// アイテムによる取得出来る弾
	m_bTake[1] = (bool)CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_UNLOCK_BULLET_WAVE);
	m_bTake[2] = (bool)CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_UNLOCK_BULLET_MISSILE);
	m_bTake[3] = (bool)CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_UNLOCK_BULLET_REVERSE);

	// 通常弾
	m_nUse = 0;
	m_aPattern[0] = 0;
	m_bTake[0] = true;
}

//==============================================================================
// デストラクタ
//==============================================================================
CBulletUI::~CBulletUI()
{
}

//==============================================================================
// 弾UIの初期化処理
//==============================================================================
HRESULT CBulletUI::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		m_size[nCnt] = D3DXVECTOR2(BUI_SIZE_X, BUI_SIZE_Y);

		// インスタンス生成
		m_apScene2D[nCnt] = new CScene2D;
	
		m_apScene2D[nCnt]->Init(D3DXVECTOR3(pos.x - 592.0f + nCnt * 80.0f, pos.y, 0.0f));
		m_apScene2D[nCnt]->SetSize(m_size[nCnt].x, m_size[nCnt].y);
		m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apScene2D[nCnt]->BindTexture((CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_UI_0 + nCnt));
	}

	return S_OK;
}

//==============================================================================
// 弾UIの終了処理
//==============================================================================
void CBulletUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{// NULLチェック後に破棄
			delete m_apScene2D[nCnt];
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 弾UIの更新処理
//==============================================================================
void CBulletUI::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// アイテムの切り替え
	if (keyboard->GetTrigger(CInput::KEYINFO_CHANGE_RIGHT) == true || gamepad->GetTrigger(CInput::KEYINFO_CHANGE_RIGHT) == true)
	{// 弾の変更
		if (m_bTake[1] == true || m_bTake[2] == true || m_bTake[3] == true)
		{
			// サウンド再生
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
			
			// 弾の変化
			BulletChangeRight();
		}
	}

	if (keyboard->GetTrigger(CInput::KEYINFO_CHANGE_LEFT) == true || gamepad->GetTrigger(CInput::KEYINFO_CHANGE_LEFT) == true)
	{
		if (m_bTake[1] == true || m_bTake[2] == true || m_bTake[3] == true)
		{
			// サウンド再生
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);

			// 弾の変化
			BulletChangeLeft();
		}
	}

	BulletStatus();

	// 通常弾UI
	m_apScene2D[0]->SetTex(2,1, m_aPattern[0],0, 0.0f, 0.0f);

	// 追加弾UI
	for (int nCnt = 1; nCnt < BUITYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt]->SetTex(3,1, m_aPattern[nCnt],0, 0.0f, 0.0f);
	}
}

//==============================================================================
// 弾UIの描画処理
//==============================================================================
void CBulletUI::Draw(void)
{
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//==============================================================================
// 弾UIの生成処理
//==============================================================================
CBulletUI *CBulletUI::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CBulletUI *pBulletUI;
	pBulletUI = new CBulletUI;

	// 初期化
	pBulletUI->Init(pos);

	return pBulletUI;
}

//==============================================================================
// 弾の変更
//==============================================================================
void CBulletUI::BulletChangeRight(void)
{
	// プレイヤーの位置
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	D3DXVECTOR3 posPlayer = pScene->GetPosition();

	// m_nUseの管理
	switch (m_nUse)
	{// 使用している弾と保有している弾によって次の弾を決定
	case 0:	// 通常弾
		if (m_bTake[1] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f,1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}
			
			m_nUse = 1;
			break;
		}
		else if (m_bTake[2] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ミサイル弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[3] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 逆ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}

	case 1:	// 追加弾1
		if (m_bTake[2] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ミサイル弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[3] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 逆ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 通常弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}

	case 2:	// 追加弾2
		if (m_bTake[3] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 逆ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 通常弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}

	case 3:	// 追加弾3(通常弾へ移行のみ)
		for (int nCnt = 0; nCnt < 100; nCnt++)
		{// 通常弾変化エフェクト
			CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
		}

		m_nUse = 0;
		break;

	default:
		break;
	}
}

//==============================================================================
// 弾の変更
//==============================================================================
void CBulletUI::BulletChangeLeft(void)
{
	// プレイヤーの位置
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	D3DXVECTOR3 posPlayer = pScene->GetPosition();

	// m_nUseの管理
	switch (m_nUse)
	{// 使用している弾と保有している弾によって次の弾を決定
	case 0:	// 通常弾
		if (m_bTake[3] == true)
		{// 逆ウェーブ
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 逆ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}
		else if (m_bTake[2] == true)
		{// ミサイル
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ミサイル弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[1] == true)
		{// ウェーブ
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 1;
			break;
		}

	case 1:	// 追加弾1
		for (int nCnt = 0; nCnt < 100; nCnt++)
		{// 通常弾変化エフェクト
			CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
		}

		m_nUse = 0;
		break;

	case 2:	// 追加弾2
		if (m_bTake[1] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 1;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 通常弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}

	case 3:	// 追加弾3(通常弾へ移行のみ)
		if (m_bTake[2] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ミサイル弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[1] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// ウェーブ弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 1;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// 通常弾変化エフェクト
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}
		

	default:
		break;
	}
}

//==============================================================================
// 弾の状態に対応するテクスチャの設定
//==============================================================================
void CBulletUI::BulletStatus(void)
{
	switch(m_nUse)
	{
		case 0:	// 通常弾
			m_aPattern[0] = 0;

			if (m_bTake[1] == true)
			{
				m_aPattern[1] = 1;
			}
			if (m_bTake[2] == true)
			{
				m_aPattern[2] = 1;
			}
			if (m_bTake[3] == true)
			{
				m_aPattern[3] = 1;
			}

			break;

		case 1:	// 追加弾1
			m_aPattern[0] = 1;
			m_aPattern[1] = 2;

			if (m_bTake[2] == true)
			{
				m_aPattern[2] = 1;
			}
			if (m_bTake[3] == true)
			{
				m_aPattern[3] = 1;
			}

			break;

		case 2:	// 追加弾2
			m_aPattern[0] = 1;
			m_aPattern[2] = 2;

			if (m_bTake[1] == true)
			{
				m_aPattern[1] = 1;
			}
			if (m_bTake[3] == true)
			{
				m_aPattern[3] = 1;
			}
			break;

		case 3:	// 追加弾3
			m_aPattern[0] = 1;
			m_aPattern[3] = 2;

			if (m_bTake[1] == true)
			{
				m_aPattern[1] = 1;
			}
			if (m_bTake[2] == true)
			{
				m_aPattern[2] = 1;
			}
			break;

		default:
			break;
	}

	// 使用状態に応じてUIのサイズを変化
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		if (nCnt == m_nUse)
		{// 選択中
			if (m_size[nCnt].x <= BUI_SIZE_X)
			{
				m_size[nCnt].x += 0.32f;
			}
			else if (m_size[nCnt].x >= BUI_SIZE_X)
			{
				m_size[nCnt].x = BUI_SIZE_X;
			}

			if (m_size[nCnt].y <= BUI_SIZE_Y)
			{
				m_size[nCnt].y += 0.34f;
			}
			else if (m_size[nCnt].y >= BUI_SIZE_Y)
			{
				m_size[nCnt].y = BUI_SIZE_Y;
			}
		}
		else
		{// 選択外
			if (m_size[nCnt].x >= BUI_SIZE_X / 2)
			{
				m_size[nCnt].x -= 0.32f;
			}
			else if (m_size[nCnt].x <= BUI_SIZE_X / 2)
			{
				m_size[nCnt].x = BUI_SIZE_X / 2;
			}

			if (m_size[nCnt].y >= BUI_SIZE_Y / 2)
			{
				m_size[nCnt].y -= 0.34f;
			}
			else if (m_size[nCnt].y <= BUI_SIZE_Y / 2)
			{
				m_size[nCnt].y = BUI_SIZE_Y / 2;
			}
		}	

			m_apScene2D[nCnt]->SetSize(m_size[nCnt].x, m_size[nCnt].y);
		
	}
}

//==============================================================================
// 武装の追加
//==============================================================================
void CBulletUI::AddBullet(CItem::ITEM_TYPE type)
{
	switch (type)
	{
	case CItem::ITEM_TYPE_0:
		if (m_bTake[1] == false)
		{
			m_bTake[1] = true;
		}
		break;

	case CItem::ITEM_TYPE_1:
		if (m_bTake[2] == false)
		{
			m_bTake[2] = true;
		}
		break;

	case CItem::ITEM_TYPE_2:
		if (m_bTake[3] == false)
		{
			m_bTake[3] = true;
		}
		break;
	}
}