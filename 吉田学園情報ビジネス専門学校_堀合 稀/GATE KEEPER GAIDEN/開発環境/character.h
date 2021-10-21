//=============================================================================
//
// �L�����N�^�[�`�揈�� [character.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#include "scene2D.h"
#include "item.h"

// �O���錾
class CBullet;
class CEnemy;
class CItem;

// �L�����N�^�[�N���X
class CCharacter : public CScene2D
{
public:
	// �L�����N�^�[�̎��
	typedef enum
	{
		CHARA_TYPE_PLAYER = 0,
		CHARA_TYPE_ENEMY,
		CHARA_TYPE_MAX
	}CHARA_TYPE;

	CCharacter(int nPriority = PRIORITY_CHARA);
	virtual ~CCharacter() = 0;
	virtual HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void DamageLife(int nDamage,D3DXVECTOR3 pos) = 0;

	void AddLife(int nLife,CHARA_TYPE type) { m_nLife[type] += nLife; }

	// �e�Ƃ̓����蔻��
	void CollisionBullet(			// �e�Ƃ̓����蔻��
		D3DXVECTOR3 posChara,		// �L�����̈ʒu
		float fSizeCharaX,			// �L�����̕�
		float fSizeCharaY);			// �L�����̍���

	// �v���C���[�ƓG�̓����蔻��
	void CollisionChara(
		D3DXVECTOR3 posChara, 
		float fSizeCharaX, 
		float fSizeCharaY);

	// �v���C���[�ƃA�C�e���̓����蔻��
	void CollisionItem(
		D3DXVECTOR3 posChara,
		float fSizeCharaX,
		float fSizeCharaY);
					
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }						// �ړ��͂̐ݒ�
	void SetLife(CHARA_TYPE type,int nLife) { m_nLife[type] = nLife; }		// �̗͂̐ݒ�
	int GetLife(CHARA_TYPE type) { return m_nLife[type]; }					// �̗͂̎擾
	void CharaSetType(CHARA_TYPE charaType) { m_charaType = charaType; }	// �L�����̎�ނ̎擾

protected:
	int m_nLife[CHARA_TYPE_MAX];	// �̗�
	D3DXVECTOR3 m_Move;				// �ړ���
	CHARA_TYPE m_charaType;			// �L�����̎��

private:
	
};

#endif