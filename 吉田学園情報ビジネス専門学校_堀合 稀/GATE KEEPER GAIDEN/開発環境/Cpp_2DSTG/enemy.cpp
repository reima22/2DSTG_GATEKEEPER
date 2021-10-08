//==============================================================================
//
// �G�`�揈���kenemy.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "enemy.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"
#include "item.h"
#include "score.h"
#include "defeat.h"
#include "effect.h"
#include "effect_on.h"
#include "game.h"
#include "textdata_enemy.h"
#include "caution.h"
#include "sound.h"

// �ÓI�����o�ϐ��錾
int CEnemy::m_nNumEnemy = 0;						// ����
int CEnemy::m_nSetEnemyCnt = 0;						// ���������ݒ�J�E���g
CEnemy::BULLET_PATTERN CEnemy::m_patternSet = 
BULLET_PATTERN_NORMAL;								// �e�̔��˃p�^�[��

int CEnemyFormation::m_nFormNumInterval = 0;		// ���񐶐��̌ʊԂ̊Ԋu
int CEnemyFormation::m_nFormSetInterval = 0;		// ���񂻂̂��̂̐����Ԋu
bool CEnemyFormation::m_bFormSetInt = false;		// ����𐶐����邩
D3DXVECTOR3 CEnemyFormation::m_pos;					// ����z�u�J�n�ʒu
D3DXVECTOR3 CEnemyFormation::m_moveRot;				// ����z�u�ړ�����
float CEnemyFormation::m_fMove;						// ����ړ���
CTexture::TEXTYPE CEnemyFormation::m_type;			// ����̎��
int CEnemyFormation::m_nInterval = 0;				// �������銴�o
bool CEnemyFormation::m_bSetParam = false;			// �p�����[�^��ݒ肷�邩
bool CEnemyFormation::m_bLock = false;

CEnemyMother::MOTHER_STATE CEnemyMother::m_motherState = MOTHER_STATE_NONE;	// ��͂̏��
CEnemyMother::MOTHER_PHASE CEnemyMother::m_phase = MOTHER_PHASE_0;			// ��͐�̃t�F�[�Y
int CEnemyMother::m_nMotherStartLife;										// ��͂̍ő�̗�

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemy::CEnemy()
{
	m_nNumEnemy++;		// �����̉��Z
	m_nBulletCnt = 0;
	m_nBulletSetDelay = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEnemy::~CEnemy()
{

}

//==============================================================================
// �G�̏���������
//==============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay)
{
	// �������ݒ�
	CScene2D::Init(pos);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);

	// �e���˃p�^�[���̐ݒ�
	SetBulletPattern();

	// ���ˊԊu�̐ݒ�
	m_nBulletSetDelay = nBulletSetDelay;

	// ��Ԑݒ�
	m_state = ENEMYSTATE_NORMAL;

	return S_OK;
}

//==============================================================================
// �G�̏I������
//==============================================================================
void CEnemy::Uninit(void)
{
	// �����̌���
	m_nNumEnemy--;

	CScene2D::Uninit();
}

//==============================================================================
// �G�̍X�V����
//==============================================================================
void CEnemy::Update(void)
{
	// �L�������̎擾
	D3DXVECTOR3 posEnemy = CScene2D::GetPosition();
	float fSizeCharaX = CScene2D::GetSize().x;
	float fSizeCharaY = CScene2D::GetSize().y;

	// �ړ�����
	posEnemy += m_Move;

	if (m_type == ENEMY_TYPE_MOTHER)
	{
		CEnemy::MoveMother(posEnemy);
	}

	CEnemyMother::MoveParts();

	// �e���˃J�E���g
	m_nBulletCnt++;

	// �e�̎�������
	if ( m_nBulletSetDelay != 0 && m_nBulletCnt % m_nBulletSetDelay == 0)
	{	
		AutoSetBullet(posEnemy, m_pattern);
	}

	// �e�Ƃ̓����蔻��
	CollisionBullet(posEnemy, fSizeCharaX, fSizeCharaY);

	// ��ʉ��[�ڐG�ɂ��ړ��̐܂�Ԃ�	
	if (m_bUnderSet == false)
	{// X���̔���L��
		if (posEnemy.x < fSizeCharaX || posEnemy.x > SCREEN_WIDTH - fSizeCharaX)
		{
			m_Move.x *= -1;
		}

		if (CGame::GetMotherEnemy() == true)
		{
			if (m_type == ENEMY_TYPE_1)
			{
				if (posEnemy.x < fSizeCharaX + 10.0f || posEnemy.x > SCREEN_WIDTH - fSizeCharaX - 10.0f)
				{
					m_Move.x = 0.0f;
					m_Move.y = 1.0f;
				}
			}
		}
	}
	else
	{// �܂�Ԃ������̏ꍇ�A����ʒu�ō~��
		if (m_Move.x > 0 && posEnemy.x >= 1240.0f)
		{// �E�ړ��̏ꍇ
			m_Move = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		}
		else if (m_Move.x < 0 && posEnemy.x <= 40.0f)
		{// ���ړ��̏ꍇ
			m_Move = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		}		
	}

	// �X�e�[�^�X���
	EnemyState();

	// �ʒu�̐ݒ�
	CScene2D::SetPosition(posEnemy);

	// �X�V
	CScene2D::Update();
}

//==============================================================================
// �G�̕`�揈��
//==============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// �G�̐�������
//==============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, CTexture::TEXTYPE type, D3DXVECTOR3 move,int nLife,int nBulletSetDelay,BULLET_PATTERN pattern,bool bUnder,ENEMY_TYPE enemyType)
{
	// ���[�J���ϐ��錾
	CEnemy *pEnemy;
	pEnemy = new CEnemy;

	// �e���˃p�^�[���̈ꎞ�ۑ�
	m_patternSet = pattern;

	// ������
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, objType, fSizeX, fSizeY,nBulletSetDelay);
	}

	// �e�N�X�`�����蓖��
	pEnemy->BindTexture(type);

	// �L�����N�^�[�p�����[�^
	pEnemy->SetMove(move);
	pEnemy->SetLife(CHARA_TYPE_ENEMY,nLife);
	pEnemy->CharaSetType(CHARA_TYPE_ENEMY);

	pEnemy->m_bUnderSet = bUnder;
	pEnemy->m_type = enemyType;

	return pEnemy;
}

//==============================================================================
// �ʎ�����������
//==============================================================================
void CEnemy::AutoSetCreate(void)
{
	// �����_���ݒu�J�E���g��l
	int nSetMin = 0;
	int nSetDif = 0;

	float fSpeed = 0.0f;

	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_EASY);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_EASY);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_NORMAL);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_NORMAL);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_HARD);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_HARD);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_HARD);
		break;
	}

	// �ݒu�J�E���g�̑���
	m_nSetEnemyCnt++;

	// �G�̎�������
	if (m_nSetEnemyCnt % nSetMin == 0)
	{
		// �z�u����X���̐ݒ�
		float fRandX = (float)(rand() % 1200 + 60);

		// ��������G�̃T�C�Y
		float fSizeX;
		float fSizeY;

		ENEMY_TYPE enemyType;

		// �G�̎�ސݒ�
		int nType = rand() % 4;

		fSpeed -= (float)nType / 10.0f;

		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

		// �e�����_�������J�E���g��l
		int nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MIN);
		int nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_DIFFERENCE);

		// �e�̔��ˊԊu
		int nBulletCntSet = rand() % nSetBulletDif + nSetBulletMin;

		// �G�̃N���G�C�g
		switch (nType)
		{// �T�C�Y�̕���
		case 0:
			fSizeX = ENEMYX_0;
			fSizeY = ENEMYY_0;
			enemyType = ENEMY_TYPE_0;
			break;

		case 1:
			fSizeX = ENEMYX_1;
			fSizeY = ENEMYY_1;
			enemyType = ENEMY_TYPE_1;
			break;

		case 2:
			fSizeX = ENEMYX_2;
			fSizeY = ENEMYY_2;
			enemyType = ENEMY_TYPE_2;
			break;

		case 3:
			fSizeX = ENEMYX_3;
			fSizeY = ENEMYY_3;
			enemyType = ENEMY_TYPE_3;
			break;
		}

		// �G�̐���
		CEnemy::Create(D3DXVECTOR3(fRandX, ENEMY_SET_POS_Y, 0.0f),
			CScene::OBJTYPE_ENEMY,
			fSizeX, fSizeY, (CTexture::TEXTYPE)(CTexture::TEXTYPE_ENEMY_0 + nType),
			move, nType + 1, nBulletCntSet, BULLET_PATTERN_NORMAL, false,enemyType);

	}
}

//==============================================================================
// ��_���[�W�̏���
//==============================================================================
void CEnemy::DamageLife(int nDamage,D3DXVECTOR3 pos)
{
	// �_���[�W�v�Z
	m_nLife[CHARA_TYPE_ENEMY] -= nDamage;

	// ����
	if (m_nLife[CHARA_TYPE_ENEMY] <= 0)
	{// ���j
		// ���j�X�e�[�^�X��
		SetState(ENEMYSTATE_DEATH);
	}
	else
	{// ��_���[�W
		// �T�E���h�Đ�
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

		// ��_���[�W�X�e�[�^�X��
		SetState(ENEMYSTATE_DAMAGE);
	}
}

//==============================================================================
// STATE���̏���
//==============================================================================
void CEnemy::EnemyState(void)
{
	// �G�̈ʒu
	D3DXVECTOR3 posEnemy = GetPosition();

	m_nStateCnt++;

	switch (m_state)
	{
	case ENEMYSTATE_NORMAL:	// �ʏ펞
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nStateCnt = 0;
		break;

	case ENEMYSTATE_DAMAGE:	// ��_���[�W
		if (m_nStateCnt <= 9)
		{
			CScene2D::SetCol(D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_state = ENEMYSTATE_NORMAL;
			m_nStateCnt = 0;
		}
		break;

	case ENEMYSTATE_DEATH:	// ���C�t�r��		
		m_nStateCnt = 0;

		// ������
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

		// �X�R�A�̉��Z
		if (m_type == ENEMY_TYPE_MOTHER)
		{// ��͂̌��j
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{
				CEffect::Create(posEnemy, 0.08f, D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f), 20.0f, 0.01f, 0);
			}

			CScore::AddScore(30000);
			
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_END);
		}
		else
		{// ���G�̌��j
			CScore::AddScore(10000);

			// ���j���̉��Z
			CDefeat::AddDefeat();

			// �A�C�e������
			CItem::RandCreate(posEnemy);

			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				CEffect::Create(posEnemy, 0.05f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.02f, 0);
			}
		}
		
		Uninit();
		break;

	default:
		break;
	}
}

//==============================================================================
// �G�̒e��������
//==============================================================================
void CEnemy::AutoSetBullet(D3DXVECTOR3 pos,BULLET_PATTERN pattern)
{
	// ���˂������
	switch (pattern)
	{
	case BULLET_PATTERN_NONE:	//	�����Ȃ�
		break;

	case BULLET_PATTERN_NORMAL:	// �^���֌���
		CBullet::Create(pos, CScene::OBJTYPE_BULLET, 
			BULLET_A_X, BULLET_A_Y, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 
			CTexture::TEXTYPE_BULLET_ENEMY, CBullet::BULLET_SIDE_ENEMY,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case BULLET_PATTERN_AIM:	// �v���C���[��_��
		// �v���C���[�̈ʒu�擾
		D3DXVECTOR3 posPlayer = CScene::GetScene(PRIORITY_CHARA)->GetPosition();

		// �v���C���[�ւ̕���
		D3DXVECTOR3 vecPlayer = posPlayer - pos;

		// ���������߂�
		float fLength = (float)sqrt((vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y));

		// �p�x
		float fAngle = atan2f(vecPlayer.x, vecPlayer.y);

		// �����𒲐�
		vecPlayer.x = vecPlayer.x / fLength;
		vecPlayer.y = vecPlayer.y / fLength;

		// �������w��
		vecPlayer.x *= BULLET_AIM_SPEED;
		vecPlayer.y *= BULLET_AIM_SPEED;

		CBullet::Create(pos, CScene::OBJTYPE_BULLET,
			BULLET_A_X, BULLET_A_Y, vecPlayer,
			CTexture::TEXTYPE_BULLET_ENEMY, CBullet::BULLET_SIDE_ENEMY,
			D3DXVECTOR3(0.0f, 0.0f, fAngle), 1);
		break;
	}
}

//==============================================================================
// �c������G�̑S�j��
//==============================================================================
void CEnemy::UninitAll(void)
{
	// ���[�J���ϐ��錾
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	// �S�Ă̓G�̎��S�t���O����
	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{
			pScene->Uninit();
		}

		pScene = pSceneNext;
	}
}

//==============================================================================
// ��͂̋���
//==============================================================================
void CEnemy::MoveMother(D3DXVECTOR3 pos)
{
	// ��͂̏o�����
	CEnemyMother::MOTHER_STATE state = CEnemyMother::GetMotherState();

	// ��Ԃɂ��ω�
	switch (state)
	{
	case CEnemyMother::MOTHER_STATE_NONE:
		// �x���̏I��
		if (CCaution::GetCautionState() == CCaution::CAUTION_STATE_ALERT_END)
		{
			// ��Ԃ̕ω�
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_START);
		}

		break;

	case CEnemyMother::MOTHER_STATE_START:
		// �ړ��͂̐ݒ�
		m_Move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		if (pos.y >= MOTHER_POS_Y)
		{
			// ��Ԃ̕ω�
			m_Move = D3DXVECTOR3(MOTHER_MOVE_0, 0.0f, 0.0f);			
			CEnemyFormation::SetParamReset();	// ������̃��Z�b�g
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_ACTIVE);
		}
		break;

	case CEnemyMother::MOTHER_STATE_ACTIVE:
		if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() * 3.0f / 4.0f)
		{
			// �����C���^�[�o��
			CEnemyFormation::SetInterval(100);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_0);
		}
		else if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() / 2.0f)
		{// �̗�50%�܂�
			// �t�F�[�Y�V�t�g
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_0)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_1);

				if (m_Move.x > 0)
				{
					m_Move.x = MOTHER_MOVE_1;
				}
				else
				{
					m_Move.x = -MOTHER_MOVE_1;
				}
			}

			CEnemyFormation::SetInterval(110);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_1);
		}
		else if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() / 4.0f)
		{// �̗�25%�܂�
		 // �t�F�[�Y�V�t�g
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_1)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_2);

				// �ړ��͂̉���
				if (m_Move.x > 0)
				{
					m_Move.x = MOTHER_MOVE_2;
				}
				else
				{
					m_Move.x = -MOTHER_MOVE_2;
				}
			}

			// �C���^�[�o���̐ݒ�
			CEnemyFormation::SetInterval(120);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_2);
		}
		else
		{// �̗�25%�ȍ~
		 // �t�F�[�Y�V�t�g
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_2)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_3);
			}

			CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
			CScene *pSceneNext = NULL;

			while (pScene)
			{
				pSceneNext = CScene::GetSceneNext(pScene);

				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{
					CEnemy *pEnemy = (CEnemy*)pScene;

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER)
					{
						if (pEnemy->GetPosition().x < 650.0f && pEnemy->GetPosition().x > 630.0f)
						{
							pEnemy->SetPosition(D3DXVECTOR3(640.0f, pEnemy->GetPosition().y, 0.0f));
							pEnemy->m_Move.x = 0.0f;
						}					
					}
				}

				pScene = pSceneNext;
			}

			// �C���^�[�o���̐ݒ�
			CEnemyFormation::SetInterval(130);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_PARTS);
		}

		break;
	}
}

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemyFormation::CEnemyFormation()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEnemyFormation::~CEnemyFormation()
{

}

//==============================================================================
// �G�̕ґ���Ԑ���
//==============================================================================
void CEnemyFormation::SetFormationEnemy(D3DXVECTOR3 pos, OBJTYPE objType, CTexture::TEXTYPE type, D3DXVECTOR3 moveRot, float fMove, int nSetNum, int nInterval,TEAM_PATTERN team)
{
	// �ړ��{��
	moveRot.x *= fMove;
	moveRot.y *= fMove;

	// �e�����_�������J�E���g��l
	int nSetBulletMin;
	int nSetBulletDif;

	// ��͐펞
	if (team == TEAM_PATTERN_MOTHER_0 || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_2)
	{
		nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MOTHER_MIN);
		nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE);
	}
	else
	{// �ʏ�
		nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MIN);
		nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_DIFFERENCE);
	}

	// ���i�ړ��p�^�[��
	bool bUnder = false;
	if (team == TEAM_PATTERN_UNDER)
	{
		bUnder = true;
	}

	// ���[�J���ϐ��錾
	int nNumMax = 1;	// ��x�̍ő�z�u��

	// �z�u�p�^�[���ɂ��ݒ�
	if (team == TEAM_PATTERN_SIDE || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_PARTS)
	{
		nNumMax = 2;
	}
	else if (team == TEAM_PATTERN_MOTHER_2)
	{
		nNumMax = 3;
	}

	m_nFormNumInterval++;

	if (m_nFormNumInterval % nInterval == 0)
	{
		// ���[�J���ϐ��錾
		float fSizeX;
		float fSizeY;
		int nLife;
		ENEMY_TYPE enemyType;
		int nBulletDelay = rand() % nSetBulletDif + nSetBulletMin;

		// �e�p�[�c
		CEnemy *pLeft = NULL;
		CEnemy *pRight = NULL;
		CEnemy *pCenter = NULL;

		// ��ނ��Ƃ̐ݒ�
		switch (type)
		{
		case CTexture::TEXTYPE_ENEMY_0:
			fSizeX = ENEMYX_0;
			fSizeY = ENEMYY_0;
			nLife = 1;
			enemyType = ENEMY_TYPE_0;
			break;

		case CTexture::TEXTYPE_ENEMY_1:
			fSizeX = ENEMYX_1;
			fSizeY = ENEMYY_1;
			nLife = 2;
			enemyType = ENEMY_TYPE_1;
			break;

		case CTexture::TEXTYPE_ENEMY_2:
			fSizeX = ENEMYX_2;
			fSizeY = ENEMYY_2;
			nLife = 3;
			enemyType = ENEMY_TYPE_2;
			break;

		case CTexture::TEXTYPE_ENEMY_3:
			fSizeX = ENEMYX_3;
			fSizeY = ENEMYY_3;
			nLife = 4;
			enemyType = ENEMY_TYPE_3;
			break;
		}

		// ���[�J���ϐ��錾
		D3DXVECTOR3 posLeft;
		D3DXVECTOR3 posRight;
		D3DXVECTOR3 posCenter;

		int nLifeLeft = 0;
		int nLifeRight = 0;
		int nLifeCenter = 0;

		// ��͐�A�G�����G�t�F�N�g
		if (team == TEAM_PATTERN_MOTHER_0 || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_2 || team == TEAM_PATTERN_MOTHER_PARTS)
		{
			CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
			CScene *pSceneNext = NULL;

			while (pScene)
			{
				pSceneNext = CScene::GetSceneNext(pScene);

				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// �G�ł���
					CEnemy *pEnemy = (CEnemy*)pScene;

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_LEFT)
					{// ���p�[�c
					 // �ʒu�̎擾
						pLeft = pEnemy;
					}

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_RIGHT)
					{// �E�p�[�c
						pRight = pEnemy;
					}

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_CENTER)
					{// �����p�[�c
						pCenter = pEnemy;
					}
				}

				// ���̃|�C���^
				pScene = pSceneNext;
			}

			if (pLeft != NULL)
			{
				nLifeLeft = pLeft->GetLife(CHARA_TYPE_ENEMY);
				posLeft = pLeft->GetPosition();
			}

			if (pRight != NULL)
			{
				nLifeRight = pRight->GetLife(CHARA_TYPE_ENEMY);
				posRight = pRight->GetPosition();
			}

			if (pCenter != NULL)
			{
				nLifeCenter = pCenter->GetLife(CHARA_TYPE_ENEMY);
				posCenter = pCenter->GetPosition();
			}
		}

		// �����ݒ�
		if (team == TEAM_PATTERN_SIDE)
		{
			for (int nNum = 0; nNum < nNumMax; nNum++)
			{
				if (nNum < 1)
				{// 1�̖�
					CEnemy::Create(pos, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
				}
				else
				{// 2�̖�
					CEnemy::Create(D3DXVECTOR3(pos.x + 1200.0f, pos.y, 0.0f), objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_0)
		{// �ʏ�ݒ�
			if (pCenter != NULL)
			{
				// ������
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					// �g��^�G�t�F�N�g
					CEffect::Create(posCenter, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(posCenter.x + 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(posCenter.x - 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
				}

				CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_1)
		{
			if (pCenter != NULL)
			{
				// ������
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nNum = 0; nNum < nNumMax; nNum++)
				{
					if (nNum < 1)
					{// 1�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							// �g��^�G�t�F�N�g
							CEffect::Create(posCenter, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
							CEffect::Create(D3DXVECTOR3(posCenter.x + 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
							CEffect::Create(D3DXVECTOR3(posCenter.x - 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
						}
					}
					else
					{// 2�̖�

						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
				}				
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_2)
		{
			if (pCenter != NULL)
			{
				// ������
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					// �g��^�G�t�F�N�g
					CEffect::Create(pos, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(pos.x + 10.0f, pos.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(pos.x - 10.0f, pos.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
				}

				for (int nNum = 0; nNum < nNumMax; nNum++)
				{
					if (nNum < 1)
					{// 1�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
					else if (nNum < 2)
					{// 2�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(0.0f, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
					else
					{// 3�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
				}				
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_PARTS)
		{
			for (int nNum = 0; nNum < nNumMax; nNum++)
			{
				if (nNum < 1 && pLeft != NULL)
				{
					// ������
					CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

					CEnemy::Create(posLeft, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, 0.0f), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						// �g��^�G�t�F�N�g
						CEffect::Create(posLeft, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y + 10.0f ,0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y - 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
					}
				}
				else if (nNum < 2 && pRight != NULL)
				{
					// ������
					CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

					CEnemy::Create(posRight, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						// �g��^�G�t�F�N�g
						CEffect::Create(posRight, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);
						CEffect::Create(D3DXVECTOR3(posRight.x, posRight.y + 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);
						CEffect::Create(D3DXVECTOR3(posRight.x, posRight.y - 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);

					}
				}
			}	
		}
		else
		{// �P��
			CEnemy::Create(pos, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
		}
	}

	if (m_nFormNumInterval >= nInterval * nSetNum || CGame::GetSetEnemy() == false)
	{
		// �Ґ������̃C���^�[�o����
		m_bFormSetInt = true;

		// ���������߂�
		m_bSetParam = false;

		// �J�E���g���Z�b�g
		m_nFormNumInterval = 0;
	}
}

//==============================================================================
// �G�̕ґ�������񃉃��_����
//==============================================================================
void CEnemyFormation::SetFormRandom(TEAM_PATTERN team)
{
	// ���[�J���ϐ��錾
	CEnemy *pEnemy = NULL;
	// ��͂̈ʒu�擾
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	switch (team)
	{
	case TEAM_PATTERN_RANDOM: // �����_���z�u

		if (m_bSetParam == false)
		{
			// �ʒu�̐ݒ�
			m_pos = D3DXVECTOR3((float)(rand() % 1200 + 40), ENEMY_SET_POS_Y, 0.0f);

			// ��ނ̐ݒ�
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// �ړ������E�͂̐ݒ�
			float fSinAngle = ((float)(rand() % 314 - 157) / 100.0f);
			m_moveRot = D3DXVECTOR3(sinf(fSinAngle), cosf(0.0f), 0.0f);
			m_fMove = (float)(rand() % 10 + 10) / 10.0f;

			// ��������Ԋu
			m_nInterval = 60;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval,team);

		break;

	case TEAM_PATTERN_SIDE:	// ���T�C�h�z�u

		if (m_bSetParam == false)
		{
			// �ʒu�̐ݒ�
			m_pos = D3DXVECTOR3(40.0f, ENEMY_SET_POS_Y, 0.0f);
			
			// ��ނ̐ݒ�
			m_type = CTexture::TEXTYPE_ENEMY_1;

			// �ړ������E�͂̐ݒ�
			m_moveRot = D3DXVECTOR3(0.0f, cosf(0.0f), 0.0f);
			m_fMove = 1.0f;

			// ��������Ԋu
			m_nInterval = 60;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u(���E)
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval,team);

		break;

	case TEAM_PATTERN_UNDER:

		if (m_bSetParam == false)
		{
			// ���E�̂ǂ���ɏo�邩
			bool bLeft = false;

			int nRand = rand() % 2;

			if (nRand == 0)
			{
				bLeft = true;
			}

			if (bLeft == true)
			{
				// �ʒu�̐ݒ�
				m_pos = D3DXVECTOR3(ENEMY_SET_POS_X_UNDER_L, ENEMY_SET_POS_Y_UNDER, 0.0f);

				// �ړ������E�͂̐ݒ�
				m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), 0.0f, 0.0f);
				m_fMove = (float)(rand() % 10 + 10) / 10.0f;
			}
			else
			{
				// �ʒu�̐ݒ�
				m_pos = D3DXVECTOR3(ENEMY_SET_POS_X_UNDER_R, ENEMY_SET_POS_Y_UNDER, 0.0f);

				// �ړ������E�͂̐ݒ�
				m_moveRot = D3DXVECTOR3(sinf(-D3DX_PI / 2), 0.0f, 0.0f);
				m_fMove = (float)(rand() % 10 + 10) / 10.0f;
			}

			// ��ނ̐ݒ�
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// ��������Ԋu
			m_nInterval = 60;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);

		break;
		
	case TEAM_PATTERN_MOTHER_0:

		while (pScene)
		{
			// ���̃|�C���^
			pSceneNext = CScene::GetSceneNext(pScene);

			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G�ł���
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER)
				{// ��͂ł���
					// �ʒu�̎擾
					pos = pEnemy->GetPosition();
					break;
				}
			}

			// ���̃|�C���^
			pScene = pSceneNext;
		}
		
		// �ʒu�̐ݒ�
		m_pos = pos;

		if (m_bSetParam == false)
		{
			// ��ނ̐ݒ�
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// �ړ������E�͂̐ݒ�
			float fSinAngle = ((float)(rand() % 314 - 157) / 100.0f);
			m_moveRot = D3DXVECTOR3(sinf(fSinAngle), cosf(0.0f), 0.0f);
			m_fMove = (float)(rand() % 10 + 10) / 10.0f;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);
		break;

	case TEAM_PATTERN_MOTHER_1:
		while (pScene)
		{
			// ���̃|�C���^
			pSceneNext = CScene::GetSceneNext(pScene);

			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G�ł���
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER)
				{// ��͂ł���
				 // �ʒu�̎擾
					pos = pEnemy->GetPosition();
					break;
				}
			}

			// ���̃|�C���^
			pScene = pSceneNext;
		}

		// �ʒu�̐ݒ�
		m_pos = pos;

		// ��ނ̐ݒ�
		m_type = CTexture::TEXTYPE_ENEMY_3;

		// ���ꂼ��̈ړ���
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2),cosf(0.0f),0.0f);

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 1, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_2:

		while (pScene)
		{
			// ���̃|�C���^
			pSceneNext = CScene::GetSceneNext(pScene);

			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G�ł���
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER)
				{// ��͂ł���
				 // �ʒu�̎擾
					pos = pEnemy->GetPosition();
					break;
				}
			}

			// ���̃|�C���^
			pScene = pSceneNext;
		}

		// �ʒu�̐ݒ�
		m_pos = pos;

		// ��ނ̐ݒ�
		m_type = CTexture::TEXTYPE_ENEMY_2;

		// ���ꂼ��̈ړ���
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), cosf(0.0f), 0.0f);

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 1, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_PARTS:

		// ��ނ̐ݒ�
		m_type = CTexture::TEXTYPE_ENEMY_1;

		// ���ꂼ��̈ړ���
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2) * 4.0f, 0.0f, 0.0f);

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval, team);

		break;
	}

}

//==============================================================================
// �C���^�[�o���J�E���g
//==============================================================================
void CEnemyFormation::FormInterval(int nIntervalCnt)
{
	// �J�E���g���Z
	m_nFormSetInterval++;

	if (m_nFormSetInterval >= nIntervalCnt)
	{
		m_nFormSetInterval = 0;
		m_bFormSetInt = false;
	}
}

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemyMother::CEnemyMother()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEnemyMother::~CEnemyMother()
{

}

//==============================================================================
// ��͂̐���
//==============================================================================
void CEnemyMother::MotherSetCreate(void)
{
	// ��Փx���Ƃ̕�̗͑�
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_HARD);
		break;
	}

	D3DXVECTOR3 pos = MOTHER_SET_POS;

	// �p�[�c�̗̑�
	int nLifeParts = 50;

	// �{��
	CEnemy::Create(pos, CScene::OBJTYPE_ENEMY, MOTHER_ENEMY_X, MOTHER_ENEMY_Y,
		CTexture::TEXTYPE_ENEMY_4_MOTHER,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		m_nMotherStartLife,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER);

	// ��
	CEnemy::Create(D3DXVECTOR3(pos.x - 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false,ENEMY_TYPE_MOTHER_PARTS_LEFT);
	
	// �E
	CEnemy::Create(D3DXVECTOR3(pos.x + 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false,ENEMY_TYPE_MOTHER_PARTS_RIGHT);

	// ����
	CEnemy::Create(D3DXVECTOR3(pos.x, pos.y + 80.0f, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false,ENEMY_TYPE_MOTHER_PARTS_CENTER);
}

//==============================================================================
// ��̓p�[�c�̋���
//==============================================================================
void CEnemyMother::MoveParts(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posLeft = MOTHER_PARTS_POS_0;
	D3DXVECTOR3 posRight = MOTHER_PARTS_POS_1;
	D3DXVECTOR3 posCenter = MOTHER_PARTS_POS_2;

	// ��͂̏��
	// ��͂̈ʒu�擾
	D3DXVECTOR3 pos;
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		// ���̃|�C���^
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{// �G�ł���
			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER)
			{// ��͂ł���
			 // �ʒu�̎擾
				pos = pEnemy->GetPosition();
				break;
			}
		}

		// ���̃|�C���^
		pScene = pSceneNext;
	}

	// �|�C���^�C��
	pScene = CScene::GetScene(PRIORITY_CHARA);
	pSceneNext = NULL;

	while (pScene)
	{
		// ���̃|�C���^
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{// �G�ł���
			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_LEFT)
			{
				posLeft = pos + posLeft;
				pEnemy->SetPosition(posLeft);
			}
			else if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_RIGHT)
			{
				posRight = pos + posRight;
				pEnemy->SetPosition(posRight);
			}
			else if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_CENTER)
			{
				posCenter = pos + posCenter;
				pEnemy->SetPosition(posCenter);
			}
		}

		// ���̃|�C���^
		pScene = pSceneNext;
	}
}