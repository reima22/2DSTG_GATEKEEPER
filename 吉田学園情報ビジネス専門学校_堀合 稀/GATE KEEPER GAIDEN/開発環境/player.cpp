//==============================================================================
//
// プレイヤー描画処理〔player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"
#include "bullet_UI.h"
#include "effect.h"
#include "stock.h"
#include "sound.h"
#include "enemy.h"

// 静的メンバ変数宣言
int CPlayer::m_nStock;
CPlayer::PLAYERSTATE CPlayer::m_state;
int CPlayer::m_nMissileCnt = 0;
bool CPlayer::m_bMissileOff = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CPlayer::CPlayer()
{
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStock = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_STOCK);
	m_nStateCnt = 0;
	m_state = PLAYERSTATE_NORMAL;
	m_fSpeed = CTextDataPlayer::SetSpeed();
}

//==============================================================================
// デストラクタ
//==============================================================================
CPlayer::~CPlayer()
{

}

//==============================================================================
// プレイヤーの初期化処理
//==============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay)
{
	// 初期化
	CScene2D::Init(pos);
	
	// サイズ設定
	CScene2D::SetSize(fSizeX, fSizeY);
	
	// テクスチャ設定
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	
	// オブジェクトタイプ設定
	CScene2D::SetObjType(objType);

	return S_OK;
}

//==============================================================================
// プレイヤーの終了処理
//==============================================================================
void CPlayer::Uninit(void)
{
	// 終了
	CScene2D::Uninit();
}

//==============================================================================
// プレイヤーの更新処理
//==============================================================================
void CPlayer::Update(void)
{
	// 位置情報の取得
	D3DXVECTOR3 posPlayer = CScene2D::GetPosition();

	// 移動
	posPlayer += m_Move;

	// プレイヤーの操作
	PlayerMove(posPlayer);

	// プレイヤーの状態
	PlayerState();

	// 弾との当たり判定
	CollisionBullet(posPlayer, PLAYERX, PLAYERY);

	// 敵キャラクターとの当たり判定
	CollisionChara(posPlayer, PLAYERX, PLAYERY);

	// アイテムとの当たり判定
	CollisionItem(posPlayer, PLAYERX, PLAYERY);

	// 左右移動限界
	if (posPlayer.x < PLAYERX)
	{
		posPlayer.x = PLAYERX;
	}
	if (posPlayer.x > SCREEN_WIDTH - PLAYERX)
	{
		posPlayer.x = SCREEN_WIDTH - PLAYERX;
	}

	// 上下移動限界
	if (posPlayer.y < PLAYERY)
	{
		posPlayer.y = PLAYERY;
	}

	if (posPlayer.y > SCREEN_HEIGHT - PLAYERY - 110.0f)
	{
		posPlayer.y = SCREEN_HEIGHT - PLAYERY - 110.0f;
	}

	// 再出現する位置
	if (m_state == PLAYERSTATE_WAIT)
	{
		posPlayer = PLAYER_POS;
	}

	// 位置情報の設定
	CScene2D::SetPosition(posPlayer);

	// 更新
	CScene2D::Update();
}

//==============================================================================
// プレイヤーの描画処理
//==============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// 被ダメージ
//==============================================================================
void CPlayer::DamageLife(int nDamage,D3DXVECTOR3 pos)
{
	// 体力の減算
	m_nLife[CHARA_TYPE_PLAYER] -= nDamage;

	if (m_nLife[CHARA_TYPE_PLAYER] <= 0)
	{// プレイヤーの消滅
	 // 自機破壊音
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION003);

		// パーティクル
		for (int nCnt = 0; nCnt < 100; nCnt++)
		{
			CEffect::Create(pos, 0.05f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 10.0f, 0.02f,0);
		}

		// 残機減少
		TransStock(-1);

		// 状態変化
		if (m_nStock < 0)
		{
			m_state = PLAYERSTATE_GAMEOVER;
			m_nStock = 0;
		}
		else
		{
			m_state = PLAYERSTATE_DEATH;
		}
	}
	else
	{// 被ダメージ
	 // 被ダメージ音
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION001);

		// パーティクル
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			CEffect::Create(pos, 0.005f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 10.0f, 0.02f,0);
		}

		// 状態変化
		m_state = PLAYERSTATE_DAMAGE;
	}
}

//==============================================================================
// プレイヤーの生成処理
//==============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY)
{
	// ローカル変数宣言
	CPlayer *pPlayer;
	pPlayer = new CPlayer;

	// 初期化
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, objType, fSizeX, fSizeY,0);
	}

	// テクスチャ割り当て
	pPlayer->BindTexture(CTexture::TEXTYPE_PLAYER);

	// 体力
	pPlayer->SetLife(CHARA_TYPE_PLAYER, CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_START));
	
	// キャラタイプ
	pPlayer->CharaSetType(CHARA_TYPE_PLAYER);

	return pPlayer;
}

//==============================================================================
// STATE毎の処理
//==============================================================================
void CPlayer::PlayerState(void)
{
	m_nStateCnt++;

	switch (m_state)
	{
	case PLAYERSTATE_NORMAL:	// 通常時
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nStateCnt = 0;
		break;

	case PLAYERSTATE_DAMAGE:	// 被ダメージ
		if (m_nStateCnt <= 9)
		{
			CScene2D::SetCol(D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_state = PLAYERSTATE_APPEAR;
			m_nStateCnt = 0;
		}
		break;

	case PLAYERSTATE_APPEAR:	// 出現時
		if (m_nStateCnt > 120)
		{// 通常状態へ
			m_state = PLAYERSTATE_NORMAL;
			m_nStateCnt = 0;
		}
		else if (m_nStateCnt % 2 == 0)
		{
			CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		break;

	case PLAYERSTATE_WAIT:		// 出現待機

		if (m_nStateCnt > 60)
		{
			 //= PLAYER_POS;
			m_state = PLAYERSTATE_APPEAR;
			m_nLife[CHARA_TYPE_PLAYER] = 3;
			m_nStateCnt = 0;
		}
		break;

	case PLAYERSTATE_DEATH:		// ライフ喪失		
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_state = PLAYERSTATE_WAIT;

		m_nStateCnt = 0;
		break;

	case PLAYERSTATE_GAMEOVER:	// ゲームオーバー	
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;

	default:
		break;
	}
}

//==============================================================================
// プレイヤーの操作
//==============================================================================
void CPlayer::PlayerMove(D3DXVECTOR3 pos)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	// 使用している弾
	int nUseNum = CBulletUI::GetUseNum();

	// ミサイルのインターバル
	if (m_bMissileOff == true)
	{
		MissileInterval();
	}

	// ブースターエフェクトの設定
	if (m_state != PLAYERSTATE_DEATH && m_state != PLAYERSTATE_WAIT && m_state != PLAYERSTATE_GAMEOVER)
	{
		for (int nCnt = 0; nCnt < 40; nCnt++)
		{
			switch (nUseNum)
			{
			case 0:	// 通常弾
				CEffect::Create(D3DXVECTOR3(pos.x, pos.y + 20.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 3.0f, 0.1f, 0);
				break;

			case 1:	// ウェーブ
				CEffect::Create(D3DXVECTOR3(pos.x, pos.y + 20.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), 3.0f, 0.1f, 0);
				break;

			case 2:	// ミサイル
				CEffect::Create(D3DXVECTOR3(pos.x, pos.y + 20.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 3.0f, 0.1f, 0);
				break;
			
			case 3:	// 逆ウェーブ
				CEffect::Create(D3DXVECTOR3(pos.x, pos.y + 20.0f, 0.0f), 0.009f, D3DXCOLOR(0.2f, 0.4f, 0.9f, 1.0f), 3.0f, 0.1f, 0);
				break;
			}
		}
	}

	if (m_state == PLAYERSTATE_APPEAR || m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_DAMAGE)
	{// ステータスによって操作不可
		// 操作処理
		if (keyboard->GetPress(CInput::KEYINFO_UP) == true || gamepad->GetPress(CInput::KEYINFO_UP) == true)
		{// 上へ移動
			m_Move.y += cosf(D3DX_PI) * m_fSpeed;

			if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true || gamepad->GetPress(CInput::KEYINFO_LEFT) == true)
			{// 左上
				m_Move.x += sinf(D3DX_PI / -2) * m_fSpeed;
			}
			else if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true || gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
			{// 右上
				m_Move.x += sinf(D3DX_PI / 2) * m_fSpeed;
			}
			else if (keyboard->GetPress(CInput::KEYINFO_DOWN) == true || gamepad->GetPress(CInput::KEYINFO_DOWN) == true)
			{// 上下
				m_Move.y += cosf(0.0f) * m_fSpeed;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_DOWN) == true || gamepad->GetPress(CInput::KEYINFO_DOWN) == true)
		{// 下へ移動
			m_Move.y += cosf(0.0f) * m_fSpeed;

			if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true || gamepad->GetPress(CInput::KEYINFO_LEFT) == true)
			{
				m_Move.x += sinf(D3DX_PI / -2) * m_fSpeed;
			}
			else if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true || gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
			{
				m_Move.x += sinf(D3DX_PI / 2) * m_fSpeed;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_LEFT) == true || gamepad->GetPress(CInput::KEYINFO_LEFT) == true)
		{// 左へ移動
			m_Move.x += sinf(D3DX_PI / -2) * m_fSpeed;

			if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true || gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
			{
				m_Move.x += sinf(D3DX_PI / 2) * m_fSpeed;
			}
		}
		else if (keyboard->GetPress(CInput::KEYINFO_RIGHT) == true || gamepad->GetPress(CInput::KEYINFO_RIGHT) == true)
		{// 右へ移動
			m_Move.x += sinf(D3DX_PI / 2) * m_fSpeed;
		}

		// 弾の発射
		if (keyboard->GetTrigger(CInput::KEYINFO_SHOT) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
		{// 種類を分けて発射
			if (nUseNum != 2)
			{
				CBullet::CreateType(pos, nUseNum);
			}
			else if (nUseNum == 2 && m_bMissileOff == false)
			{
				CBullet::CreateType(pos, nUseNum);
				m_bMissileOff = true;
			}
		}

		if (keyboard->GetPress(CInput::KEYINFO_SHOT) == true || gamepad->GetPress(CInput::KEYINFO_OK) == true)
		{// カウント開始
			m_nBulletPress++;
		}
		else
		{// カウントリセット
			m_nBulletPress = 0;
		}


		if (keyboard->GetPress(CInput::KEYINFO_SHOT) == true || gamepad->GetPress(CInput::KEYINFO_OK) == true)
		{
			if (nUseNum != 2 && m_nBulletPress % 10 == 0)
			{
				CBullet::CreateType(pos, nUseNum);
			}
			else if (m_nBulletPress % MISSILE_INTERVAL == 0)
			{
				CBullet::CreateType(pos, nUseNum);
			}			
		}

		// 移動力の減少
		m_Move.x += (0.0f - m_Move.x) * SPEED_DOWN;
		m_Move.y += (0.0f - m_Move.y) * SPEED_DOWN;
	}
}

//==============================================================================
// ミサイルの発射間隔設定
//==============================================================================
void CPlayer::MissileInterval(void)
{
	// インターバル中のカウント
	m_nMissileCnt++;

	// カウント終了
	if (m_nMissileCnt >= MISSILE_INTERVAL)
	{
		m_nMissileCnt = 0; 

		m_bMissileOff = false;
	}
}