//==============================================================================
//
// 弾描画処理〔bullet.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "explosion.h"
#include "sound.h"
#include "effect.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = BULLET_LIFE;
}

//==============================================================================
// デストラクタ
//==============================================================================
CBullet::~CBullet()
{

}

//==============================================================================
// 弾の初期化処理
//==============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,D3DXVECTOR3 move, CTexture::TEXTYPE type, D3DXVECTOR3 rot,int nAttack)
{
	CScene2D::Init(pos);
	CScene2D::SetRot(rot);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);
	m_move = move;
	m_Type = type;
	m_BulletType = (BULLET_TYPE)type;
	m_aAttack = nAttack;

	return S_OK;
}

//==============================================================================
// 弾の終了処理
//==============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 弾の更新処理
//==============================================================================
void CBullet::Update(void)
{
	// キーボードの取得
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// 位置情報の取得
	D3DXVECTOR3 posBullet = CScene2D::GetPosition();
	int nNumAll = CScene::GetNumAll();

	// 弾の移動
	posBullet += m_move;

	// 位置情報判定
	if (posBullet.y < 0 || posBullet.y > SCREEN_HEIGHT || posBullet.x < 0 || posBullet.x > SCREEN_WIDTH)
	{
		Uninit();
		return;
	}

	// 寿命の減少
	//m_nLife--;

	// 寿命による消滅
	if (m_nLife <= 0)
	{
		CExplosion::Create(posBullet, CScene::OBJTYPE_EXPLOSION, 50.0f, 50.0f);
		Uninit();
	}

	// 弾の軌跡
	SetEffectBullet(posBullet, m_Type);

	// 位置情報の設定
	CScene2D::SetPosition(posBullet);

	CScene2D::Update();
}

//==============================================================================
// 弾の描画処理
//==============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// 弾の生成処理
//==============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,D3DXVECTOR3 move, CTexture::TEXTYPE type, CBullet::BULLET_SIDE side,D3DXVECTOR3 rot, int nAttack)
{
	// ローカル変数宣言
	CBullet *pBullet;
	pBullet = new CBullet;

	// 初期化
	if (pBullet != NULL)
	{
		pBullet->Init(pos, objType, fSizeX, fSizeY, move, type,rot,nAttack);
	}

	// テクスチャ割り当て
	pBullet->BindTexture(type);
	
	// 敵味方情報
	pBullet->m_Side = side;

	// 発射音
	CSound::Play(CSound::SOUND_LABEL_SE_SHOT000);

	return pBullet;
}

//==============================================================================
// 使用弾に応じて生成する弾のパターンが変化
//==============================================================================
void CBullet::CreateType(D3DXVECTOR3 pos, int nUse)
{
	switch (nUse)
	{
	case 0:	// 通常弾
		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_A_X, BULLET_A_Y,
			D3DXVECTOR3(0.0f, -10.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case 1:	// ウェーブ弾
		Create(
			D3DXVECTOR3(pos.x + 30.0f, pos.y, 0.0f),
			CScene::OBJTYPE_BULLET,
			BULLET_B_X, BULLET_B_Y,
			D3DXVECTOR3(0.0f, -10.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

		Create(
			D3DXVECTOR3(pos.x - 30.0f, pos.y, 0.0f),
			CScene::OBJTYPE_BULLET,
			BULLET_B_X, BULLET_B_Y,
			D3DXVECTOR3(0.0f, -10.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case 2:	// ミサイル弾
		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_C_X, BULLET_C_Y,
			D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2.0f), 3);

		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_C_X, BULLET_C_Y,
			D3DXVECTOR3(10.0f, 0.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI / 2.0f), 3);
		break;

	case 3:	// 後方ウェーブ弾
		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_D_X, BULLET_D_Y,
			D3DXVECTOR3(sinf(D3DX_PI / 6.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 6.0f), 1);

		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_D_X, BULLET_D_Y,
			D3DXVECTOR3(sinf(-D3DX_PI / 6.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI / 6.0f), 1);
		break;

	default:
		break;
	}
}

//==============================================================================
// 軌跡エフェクトの設定
//==============================================================================
void CBullet::SetEffectBullet(D3DXVECTOR3 pos, CTexture::TEXTYPE type)
{
	switch (type)
	{
	case CTexture::TEXTYPE_BULLET_0:	// 通常弾
		CEffect::Create(pos, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.1f,0);
		break;

	case CTexture::TEXTYPE_BULLET_1:	// ウェーブ弾
		CEffect::Create(pos, 0.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.1f), 20.0f, 0.005f,0);
		break;

	case CTexture::TEXTYPE_BULLET_2:	// ミサイル弾
		CEffect::Create(pos, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.1f,0);
		break;

	case CTexture::TEXTYPE_BULLET_3:	// 後方ウェーブ弾
		CEffect::Create(pos, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 20.0f, 0.1f,0);
		break;

	case CTexture::TEXTYPE_BULLET_ENEMY:
		CEffect::Create(pos, 0.0f, D3DXCOLOR(0.9f, 0.9f, 0.5f, 1.0f), 10.0f, 0.1f, 0);
		break;

	default:
		break;
	}
}

//==============================================================================
// 弾の全削除
//==============================================================================
void CBullet::UninitAll(void)
{
	// ローカル変数宣言
	CScene *pScene = CScene::GetScene(PRIORITY_BULLET);
	CScene *pSceneNext = NULL;

	// 全ての敵の死亡フラグ建て
	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		CBullet *pBullet = (CBullet*)pScene;

		if (pBullet->GetSide() == CBullet::BULLET_SIDE_ENEMY)
		{
			pScene->Uninit();
		}

		pScene = pSceneNext;
	}
}