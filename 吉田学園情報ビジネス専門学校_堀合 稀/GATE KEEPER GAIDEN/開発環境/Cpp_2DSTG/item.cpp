//==============================================================================
//
// アイテム描画処理〔item.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "score.h"
#include "texture.h"
#include "item.h"
#include "bullet_UI.h"
#include "effect.h"
#include "defeat.h"
#include "textdata_system.h"
#include "character.h"
#include "title_difficulty.h"
#include "player.h"

// 静的メンバ変数宣言
int CItem::m_nSetHealCnt = 0;

//==============================================================================
// コンストラクタ
//==============================================================================
CItem::CItem(int nPriority) : CScene2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nBlinkCnt = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CItem::~CItem()
{

}

//==============================================================================
// アイテムの初期化処理
//==============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, D3DXVECTOR3 move, CTexture::TEXTYPE type)
{
	// 初期化設定
	CScene2D::Init(pos);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);
	m_move = move;
	m_bChase = false;
	SetType(type);

	return S_OK;
}

//==============================================================================
// アイテムの終了処理
//==============================================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// アイテムの更新処理
//==============================================================================
void CItem::Update(void)
{
	// 位置情報の取得
	D3DXVECTOR3 posItem = CScene2D::GetPosition();

	// プレイヤーが死亡した
	if (CPlayer::GetState() == CPlayer::PLAYERSTATE_DEATH && m_bChase == true)
	{
		m_bChase = false;
	}

	// 移動方法の指定
	SetChaseMove(posItem);

	// 弾の移動
	posItem += m_move;

	// 位置情報の設定
	CScene2D::SetPosition(posItem);

	// 点滅処理
	SetBlink();

	// Scene2Dの更新
	CScene2D::Update();

	// 画面外の位置情報判定
	if (posItem.y < 0 || posItem.y > SCREEN_HEIGHT)
	{
		Uninit();
	}
}

//==============================================================================
// アイテムの描画処理
//==============================================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// アイテムの生成処理
//==============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, D3DXVECTOR3 move, CTexture::TEXTYPE type)
{
	// ローカル変数宣言
	CItem *pItem;
	pItem = new CItem;

	// 初期化
	if (pItem != NULL)
	{
		pItem->Init(pos, objType, fSizeX, fSizeY, move, type);
	}

	// テクスチャ割り当て
	pItem->BindTexture(type);

	return pItem;
}

//==============================================================================
// アイテムのランダム生成
//==============================================================================
void CItem::RandCreate(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	int nDefeat = CDefeat::GetDefeat();		// 撃破数の取得

	// 回復アイテム設定カウント
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nSetHealCnt = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_HEALITEM_CNT_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nSetHealCnt = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_HEALITEM_CNT_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nSetHealCnt = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_HEALITEM_CNT_HARD);
		break;
	}

	if (nDefeat % m_nSetHealCnt == 0)
	{
		Create(pos, CScene::OBJTYPE_ITEM, ITEM_X, ITEM_Y,
			D3DXVECTOR3(0.0f, 0.5f, 0.0f),
			CTexture::TEXTYPE_ITEM_3);

		return;
	}
	else
	{
		int nRandCreate = rand() % 3;	// アイテム生成判定

										// 生成する場合
		if (nRandCreate == 0)
		{
			int nRandType = rand() % 3;	// アイテムの種類

			Create(pos, CScene::OBJTYPE_ITEM, ITEM_X, ITEM_Y,
				D3DXVECTOR3(0.0f, 0.5f, 0.0f),
				(CTexture::TEXTYPE)(CTexture::TEXTYPE_ITEM_0 + nRandType));

			return;
		}
		else
		{
			return;
		}
	}
}

//==============================================================================
// 種類の設定
//==============================================================================
void CItem::SetType(CTexture::TEXTYPE texType)
{
	switch (texType)
	{
	case CTexture::TEXTYPE_ITEM_0:
		m_Type = ITEM_TYPE_0;
		break;

	case CTexture::TEXTYPE_ITEM_1:
		m_Type = ITEM_TYPE_1;
		break;
	
	case CTexture::TEXTYPE_ITEM_2:
		m_Type = ITEM_TYPE_2;
		break;

	case CTexture::TEXTYPE_ITEM_3:
		m_Type = ITEM_TYPE_3;
		break;
	}
}

//==============================================================================
// 点滅処理
//==============================================================================
void CItem::SetBlink(void)
{
	switch (m_bBlinkUp)
	{
	case false:	// α値の減算

		m_col.a -= 0.02f;

		if (m_col.a <= 0.5f)
		{
			m_bBlinkUp = true;
		}

		break;

	case true:	// α値の加算

		m_col.a += 0.02f;

		if (m_col.a >= 1.0f)
		{
			m_bBlinkUp = false;
		}

		break;
	}

	// 色情報の設定
	CScene2D::SetCol(m_col);
}

// 追従移動力反映
void CItem::SetChaseMove(D3DXVECTOR3 posItem)
{
	if (m_bChase == true)
	{
		// ローカル変数宣言
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_CHARA);

		// プレイヤーの位置
		D3DXVECTOR3 posPlayer = pScene->GetPosition();

		// プレイヤーへの方向
		D3DXVECTOR3 vecPlayer = posPlayer - posItem;

		// 長さを求める
		float fLength = (float)sqrt((vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y));

		// 角度
		float fAngle = atan2f(vecPlayer.x, vecPlayer.y);

		// 長さを調整
		vecPlayer.x = vecPlayer.x / fLength;
		vecPlayer.y = vecPlayer.y / fLength;

		// 速さを指定
		vecPlayer.x *= 5.0f;
		vecPlayer.y *= 5.0f;

		m_move = vecPlayer;
	}
	else
	{
		m_move = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	}
}