//==============================================================================
//
// �L�����N�^�[�`�揈���kcharacter.cpp�l
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
// �R���X�g���N�^
//==============================================================================
CCharacter::CCharacter(int nPriority) : CScene2D(nPriority)
{
	
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CCharacter::~CCharacter()
{
}

//==============================================================================
// �e�Ƃ̓����蔻��
//==============================================================================
void CCharacter::CollisionBullet(D3DXVECTOR3 posChara, float fSizeCharaX, float fSizeCharaY)
{
	// �G���
	CScene *pScene = GetScene(PRIORITY_BULLET);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{
				// �e���
				D3DXVECTOR3 posBullet = pScene->GetPosition();
				float fSizeBulletX = pScene->GetSize().x;
				float fSizeBulletY = pScene->GetSize().y;

				// �I�u�W�F�N�g�̎��
				CScene::OBJTYPE objType;
				objType = CScene2D::GetObjType();

				// �e�̃|�C���^
				CBullet *pBullet = (CBullet*)pScene;

				// �e�̍U����
				int nAttack = pBullet->GetAttack();

				// �e�̓����蔻��
				if (posBullet.x + fSizeBulletX >= posChara.x - fSizeCharaX &&
					posBullet.x - fSizeBulletX <= posChara.x + fSizeCharaX &&
					posBullet.y + fSizeBulletY >= posChara.y - fSizeCharaY &&
					posBullet.y - fSizeBulletY <= posChara.y + fSizeCharaY)
				{
					if (objType == OBJTYPE_ENEMY && pBullet->GetSide() == CBullet::BULLET_SIDE_PLAYER)
					{// �G����Ǝ��@�̒e����
						if (CEnemyMother::GetMotherState() == CEnemyMother::MOTHER_STATE_START)
						{// ��͏o�����o������
							return;
						}

						// �����A�j���[�V�����Đ�
						// ���e�_�Ŕ���
						CExplosion::Create(posBullet, CScene::OBJTYPE_EXPLOSION, 10.0f, 10.0f);

						// ���e�p�[�e�B�N��
						for (int nCnt = 0; nCnt < 10; nCnt++)
						{
							CEffect::Create(posBullet, 0.005f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.02f, 0);
						}

						// ��_���[�W
						DamageLife(nAttack, posChara);

						// �e�̏���
						pScene->Uninit();
					}
					else if (objType == OBJTYPE_PLAYER &&  pBullet->GetSide() == CBullet::BULLET_SIDE_ENEMY)
					{// ���@����e��������

						CPlayer::PLAYERSTATE state = CPlayer::GetState();

						if (state != CPlayer::PLAYERSTATE_NORMAL)
						{// �ʏ��Ԏ��ȊO�̔��薳��
							return;
						}

						// ���C�t�̌���
						DamageLife(nAttack, posChara);

						// �e�̏���
						pScene->Uninit();
					}
				}
			}
		}

		// ���̃|�C���^
		pScene = pSceneNext;
	}
}

//==============================================================================
// �L�������m�̓����蔻��
//==============================================================================
void CCharacter::CollisionChara(D3DXVECTOR3 posChara, float fSizeCharaX, float fSizeCharaY)
{
	// �G���
	CScene *pScene = GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		// ���̃|�C���^�擾
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			D3DXVECTOR3 posEnemy = pScene->GetPosition();
			float fSizeEnemyX = pScene->GetSize().x;
			float fSizeEnemyY = pScene->GetSize().y;

			// �G�̓����蔻��
			if (posEnemy.x + fSizeEnemyX >= posChara.x - fSizeCharaX &&
				posEnemy.x - fSizeEnemyX <= posChara.x + fSizeCharaX &&
				posEnemy.y + fSizeEnemyY >= posChara.y - fSizeCharaY &&
				posEnemy.y - fSizeEnemyY <= posChara.y + fSizeCharaY)
			{
				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// ���Ĕ���

					CPlayer::PLAYERSTATE state = CPlayer::GetState();

					if (state != CPlayer::PLAYERSTATE_NORMAL)
					{// �ʏ��Ԏ��ȊO�̔��薳��
						return;
					}

					// �����A�j���[�V�����Đ�
					// ���e�_�Ŕ���
					CExplosion::Create(posChara, CScene::OBJTYPE_EXPLOSION, 10.0f, 10.0f);

					// �T�E���h�Đ�
					CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

					// ��_���[�W
					DamageLife(1, posChara);

					// �G�_���[�W
					CEnemy *pEnemy = (CEnemy*)pScene;
					pEnemy->DamageLife(1, posEnemy);
				}
			}
		}

		pScene = pSceneNext;
	}
}

//==============================================================================
// �A�C�e���Ƃ̓����蔻��
//==============================================================================
void CCharacter::CollisionItem(D3DXVECTOR3 posChara, float fSizeCharaX, float fSizeCharaY)
{
	// �A�C�e�����
	CScene *pScene = GetScene(PRIORITY_ITEM);
	CScene *pSceneNext = NULL;

	// �e���
	D3DXVECTOR3 posItem;
	float fSizeItemX;
	float fSizeItemY;


	while (pScene)
	{
		// ���̃|�C���^
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			if (pScene->GetObjType() == OBJTYPE_ITEM)
			{
				// �z�u���
				posItem = pScene->GetPosition();
				fSizeItemX = pScene->GetSize().x;
				fSizeItemY = pScene->GetSize().y;

				// �A�C�e���|�C���^
				CItem *pItem = (CItem*)pScene;

				// �e�̓����蔻��
				if (posItem.x + fSizeItemX >= posChara.x - fSizeCharaX &&
					posItem.x - fSizeItemX <= posChara.x + fSizeCharaX &&
					posItem.y + fSizeItemY >= posChara.y - fSizeCharaY &&
					posItem.y - fSizeItemY <= posChara.y + fSizeCharaY)
				{
					if (m_charaType == CHARA_TYPE_PLAYER)
					{// ���Ĕ���

					 // �G�t�F�N�g�̔���
						CEffect::Create(posChara, 0.0f, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), 10.0f, 0.01f, 2);

						// �T�E���h�Đ�
						CSound::Play(CSound::SOUND_LABEL_SE_ITEM000);

						// �A�C�e���̎�ނ̉���������
						if (pItem->GetType() == CItem::ITEM_TYPE_3)
						{// �񕜃A�C�e��
							if (m_nLife[CHARA_TYPE_PLAYER] < CLife::GetLifeMax())
							{// ���C�t�̉��Z
								AddLife(1, CHARA_TYPE_PLAYER);
							}
							else
							{// �̗͏����
								CScore::AddScore(10000);
							}
						}
						else
						{// ����A�C�e��
							CBulletUI::AddBullet(pItem->GetType());
							CScore::AddScore(1000);
						}

						// �e�̏���
						pItem->Uninit();
					}
				}
				else if (posItem.x + fSizeItemX + ITEM_CHASE_AREA >= posChara.x - fSizeCharaX &&
					posItem.x - fSizeItemX - ITEM_CHASE_AREA <= posChara.x + fSizeCharaX &&
					posItem.y + fSizeItemY + ITEM_CHASE_AREA >= posChara.y - fSizeCharaY &&
					posItem.y - fSizeItemY - ITEM_CHASE_AREA <= posChara.y + fSizeCharaY)
				{
					// �Ǐ]�J�n
					pItem->SetChase();
				}
			}
		}
		// ���̃|�C���^
		pScene = pSceneNext;
	}
}