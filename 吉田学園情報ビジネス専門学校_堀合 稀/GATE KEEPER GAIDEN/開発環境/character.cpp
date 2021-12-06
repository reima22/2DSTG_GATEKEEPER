//==============================================================================
//
// キャラクター描画処理〔character.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "character.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "bullet_UI.h"
#include "renderer.h"
#include "texture.h"
#include "defeat.h"
#include "sound.h"
#include "explosion.h"
#include "effect.h"
#include "item.h"
#include "score.h"
#include "enemy.h"
#include "enemy_mother.h"
#include "player.h"
#include "life.h"
#include "game.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CCharacter::CCharacter(int nPriority) : CScene2D(nPriority)
{
	
}

//==============================================================================
// デストラクタ
//==============================================================================
CCharacter::~CCharacter()
{
}

//==============================================================================
// 弾との当たり判定
//==============================================================================
void CCharacter::CollisionBullet(D3DXVECTOR3 posChara, float fSizeCharaX, float fSizeCharaY)
{
	// 敵情報
	CScene *pScene = GetScene(PRIORITY_BULLET);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{
				// 弾情報
				D3DXVECTOR3 posBullet = pScene->GetPosition();
				float fSizeBulletX = pScene->GetSize().x;
				float fSizeBulletY = pScene->GetSize().y;

				// オブジェクトの種類
				CScene::OBJTYPE objType;
				objType = CScene2D::GetObjType();

				// 弾のポインタ
				CBullet *pBullet = (CBullet*)pScene;

				// 弾の攻撃力
				int nAttack = pBullet->GetAttack();

				// 弾の当たり判定
				if (posBullet.x + fSizeBulletX >= posChara.x - fSizeCharaX &&
					posBullet.x - fSizeBulletX <= posChara.x + fSizeCharaX &&
					posBullet.y + fSizeBulletY >= posChara.y - fSizeCharaY &&
					posBullet.y - fSizeBulletY <= posChara.y + fSizeCharaY)
				{
					if (objType == OBJTYPE_ENEMY && pBullet->GetSide() == CBullet::BULLET_SIDE_PLAYER)
					{// 敵相手と自機の弾判定
						if (CEnemyMother::GetMotherState() == CEnemyMother::MOTHER_STATE_START)
						{// 母艦出現演出時無効
							return;
						}

						// 爆発アニメーション再生
						// 着弾点で爆発
						CExplosion::Create(posBullet, CScene::OBJTYPE_EXPLOSION, 10.0f, 10.0f);

						// 着弾パーティクル
						for (int nCnt = 0; nCnt < 10; nCnt++)
						{
							CEffect::Create(posBullet, 0.005f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.02f, 0);
						}

						// 被ダメージ
						DamageLife(nAttack, posChara);

						// 弾の消滅
						pScene->Uninit();
					}
					else if (objType == OBJTYPE_PLAYER &&  pBullet->GetSide() == CBullet::BULLET_SIDE_ENEMY)
					{// 自機が被弾した判定

						CPlayer::PLAYERSTATE state = CPlayer::GetState();

						if (state != CPlayer::PLAYERSTATE_NORMAL)
						{// 通常状態時以外の判定無効
							return;
						}

						// ライフの減少
						DamageLife(nAttack, posChara);

						// 弾の消滅
						pScene->Uninit();
					}
				}
			}
		}

		// 次のポインタ
		pScene = pSceneNext;
	}
}

//==============================================================================
// キャラ同士の当たり判定
//==============================================================================
void CCharacter::CollisionChara(D3DXVECTOR3 posChara, float fSizeCharaX, float fSizeCharaY)
{
	// 敵情報
	CScene *pScene = GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		// 次のポインタ取得
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			D3DXVECTOR3 posEnemy = pScene->GetPosition();
			float fSizeEnemyX = pScene->GetSize().x;
			float fSizeEnemyY = pScene->GetSize().y;

			// 敵の当たり判定
			if (posEnemy.x + fSizeEnemyX >= posChara.x - fSizeCharaX &&
				posEnemy.x - fSizeEnemyX <= posChara.x + fSizeCharaX &&
				posEnemy.y + fSizeEnemyY >= posChara.y - fSizeCharaY &&
				posEnemy.y - fSizeEnemyY <= posChara.y + fSizeCharaY)
			{
				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// 撃墜判定

					CPlayer::PLAYERSTATE state = CPlayer::GetState();

					if (state != CPlayer::PLAYERSTATE_NORMAL)
					{// 通常状態時以外の判定無効
						return;
					}

					// 爆発アニメーション再生
					// 着弾点で爆発
					CExplosion::Create(posChara, CScene::OBJTYPE_EXPLOSION, 10.0f, 10.0f);

					// サウンド再生
					CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

					// 被ダメージ
					DamageLife(1, posChara);

					// 敵ダメージ
					CEnemy *pEnemy = (CEnemy*)pScene;
					pEnemy->DamageLife(1, posEnemy);
				}
			}
		}

		pScene = pSceneNext;
	}
}

//==============================================================================
// アイテムとの当たり判定
//==============================================================================
void CCharacter::CollisionItem(D3DXVECTOR3 posChara, float fSizeCharaX, float fSizeCharaY)
{
	// アイテム情報
	CScene *pScene = GetScene(PRIORITY_ITEM);
	CScene *pSceneNext = NULL;

	// 弾情報
	D3DXVECTOR3 posItem;
	float fSizeItemX;
	float fSizeItemY;


	while (pScene)
	{
		// 次のポインタ
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			if (pScene->GetObjType() == OBJTYPE_ITEM)
			{
				// 配置情報
				posItem = pScene->GetPosition();
				fSizeItemX = pScene->GetSize().x;
				fSizeItemY = pScene->GetSize().y;

				// アイテムポインタ
				CItem *pItem = (CItem*)pScene;

				// 弾の当たり判定
				if (posItem.x + fSizeItemX >= posChara.x - fSizeCharaX &&
					posItem.x - fSizeItemX <= posChara.x + fSizeCharaX &&
					posItem.y + fSizeItemY >= posChara.y - fSizeCharaY &&
					posItem.y - fSizeItemY <= posChara.y + fSizeCharaY)
				{
					if (m_charaType == CHARA_TYPE_PLAYER)
					{// 撃墜判定

					 // エフェクトの発生
						CEffect::Create(posChara, 0.0f, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), 10.0f, 0.01f, 2);

						// サウンド再生
						CSound::Play(CSound::SOUND_LABEL_SE_ITEM000);

						// アイテムの種類の応じた効果
						if (pItem->GetType() == CItem::ITEM_TYPE_3)
						{// 回復アイテム
							if (m_nLife[CHARA_TYPE_PLAYER] < CLife::GetLifeMax())
							{// ライフの加算
								AddLife(1, CHARA_TYPE_PLAYER);
							}
							else
							{// 体力上限時
								CScore::AddScore(10000);
							}
						}
						else
						{// 武器アイテム
							CBulletUI::AddBullet(pItem->GetType());
							CScore::AddScore(1000);
						}

						// 弾の消滅
						pItem->Uninit();
					}
				}
				else if (posItem.x + fSizeItemX + ITEM_CHASE_AREA >= posChara.x - fSizeCharaX &&
					posItem.x - fSizeItemX - ITEM_CHASE_AREA <= posChara.x + fSizeCharaX &&
					posItem.y + fSizeItemY + ITEM_CHASE_AREA >= posChara.y - fSizeCharaY &&
					posItem.y - fSizeItemY - ITEM_CHASE_AREA <= posChara.y + fSizeCharaY)
				{
					// 追従開始
					pItem->SetChase();
				}
			}
		}
		// 次のポインタ
		pScene = pSceneNext;
	}
}