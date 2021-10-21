
//==============================================================================
//
// �Q�[�����[�h�����kgame.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "life.h"
#include "UI.h"
#include "defeat.h"
#include "line.h"
#include "invasion.h"
#include "fade.h"
#include "stock.h"
#include "result.h"
#include "pause.h"
#include "bullet_UI.h"
#include "textdata_system.h"
#include "message.h"
#include "message_text.h"
#include "message_window.h"
#include "caution.h"
#include "window_effect.h"
#include "result_logo.h"
#include "title_difficulty.h"
#include "sceneX.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
int CGame::m_nClearDefeat;	// �N���A���錂�j��
int CGame::m_nEndInvasion;	// �N�����E��
CCharacter *CGame::m_pCharaPlayer = NULL;	// �v���C���[�L�����̃|�C���^
bool CGame::m_bGameStart;
int CGame::m_nPhase = 0;
int CGame::m_nSetType = 0;
bool CGame::m_bSetEnemy = false;
bool CGame::m_bMotherEnemy = false;
int CGame::m_nCntDownEnd = 0;

//CSceneX *CGame::m_pSceneX = NULL;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CGame::CGame()
{
	m_nPhase = 0;
	m_nCntDownEnd = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CGame::~CGame()
{
}

//==============================================================================
// �Q�[�����[�h�̏���������
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos)
{
	// �V�X�e��
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nClearDefeat = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_DEFEAT_EASY);		// �N���A���錂�j��
		m_nEndInvasion = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_INVASION_EASY);	// �N�����E��
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nClearDefeat = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_DEFEAT_NORMAL);	// �N���A���錂�j��
		m_nEndInvasion = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_INVASION_NORMAL);	// �N�����E��
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nClearDefeat = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_DEFEAT_HARD);		// �N���A���錂�j��
		m_nEndInvasion = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_INVASION_HARD);	// �N�����E��
		break;
	}

	m_bGameStart = false;	// �Q�[���J�n�̃X�C�b�`
	m_bSetEnemy = false;	// �G�̐ݒ�Ԋu
	m_bMotherEnemy = false;	// ��͐�t�F�[�Y
	m_bSetSE = false;		// �x�����SE
	m_bSetMother = false;	// ��͐�BGM�Đ�
	m_bSetLogo = false;

	// ��͂̏�ԃ��Z�b�g
	CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_NONE);
	CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_0);

	// �w�i����
	CBg::Create();

	// �x����ʂ̐���
	CCaution::Create(SCREEN_CENTER);

	// ���C��UI����
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 675.0f, 0.0f));

	// �{�[�_�[���C���̐���
	CLine::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 620.0f, 0.0f));

	// �v���C���[����
	m_pCharaPlayer = CPlayer::Create(PLAYER_POS,
		CScene::OBJTYPE_PLAYER,
		PLAYERX, PLAYERY);

	// �c�@���̐���
	CStock::Create(STOCK_POS);

	// ���j���̐���
	CDefeat::Create(DEFEAT_POS);

	//// �N�����̐���
	CInvasion::Create(INVASION_POS);

	// �X�R�A�̐���
	CScore::Create(SCORE_POS, 10.0f, 20.0f);

	// ��ʃG�t�F�N�g�̐���
	CWindowEffect::Create(W_EFFECT_POS);

	// ���C�t�̐���
	CLife::Create(LIFE_POS);

	// �|�[�Y�̐���
	CPause::Create();

	// ���b�Z�[�W����
	CMessage::Create(SCREEN_CENTER);

	// �f�u���̔z�u
	CSceneX::Create(D3DXVECTOR3(100.0f, 100.0f, 300.0f), D3DXVECTOR3(0.0f, -0.6f, 0.0f), D3DXVECTOR3(0.01f, 0.009f, 0.0f), 1);
	CSceneX::Create(D3DXVECTOR3(-100.0f, 100.0f, 280.0f), D3DXVECTOR3(0.0f, -0.8f, 0.0f), D3DXVECTOR3(0.01f, 0.01f, 0.0f), 2);
	CSceneX::Create(D3DXVECTOR3(-200.0f, 200.0f, 280.0f), D3DXVECTOR3(0.0f, -0.5f, 0.0f), D3DXVECTOR3(0.01f, 0.008f, 0.0f), 1);
	CSceneX::Create(D3DXVECTOR3(200.0f, 200.0f, 300.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR3(0.01f, 0.01f, 0.0f), 2);

	// ���f���̔z�u
	CSceneX::Create(D3DXVECTOR3(150.0f, 50.0f, 240.0f), D3DXVECTOR3(-0.4f, -0.6f, 0.0f), D3DXVECTOR3(0.03f, 0.05f, 0.0f),0);
	CSceneX::Create(D3DXVECTOR3(150.0f, 100.0f, 120.0f), D3DXVECTOR3(-0.7f, -0.6f, 0.0f), D3DXVECTOR3(0.06f, 0.04f, 0.0f),0);
	CSceneX::Create(D3DXVECTOR3(150.0f, 100.0f, 0.0f), D3DXVECTOR3(-0.5f, -0.6f, 0.0f), D3DXVECTOR3(0.04f, 0.03f, 0.0f),0);

	// BGM�̐���
	if (CManager::GetBGMPlay(CSound::BGMTYPE_GAME) == true)
	{ 
		CSound::Play(CSound::SOUND_LABEL_BGM001);
	}
	
	return S_OK;
}

//==============================================================================
// �Q�[�����[�h�̏I������
//==============================================================================
void CGame::Uninit(void)
{
	// BGM�̒�~
	CSound::Stop();

	// ���b�Z�[�W�̔j��
	CMessage::Uninit();

	// �|�[�Y�̏I��
	CPause::Uninit();

	// �G�̏���
	CEnemy::UninitAll();

	// �v���C���[�|�C���^�̔j��
	if (m_pCharaPlayer != NULL)
	{
		m_pCharaPlayer->Uninit();
		m_pCharaPlayer = NULL;
	}

	Release();	// �C���X�^���X�̃f���[�g
}

//==============================================================================
// �Q�[�����[�h�̍X�V����
//==============================================================================
void CGame::Update(void)
{
	// ���b�Z�[�W�̍X�V
	CMessage::Update();

	// �J�n����
	m_bGameStart = CMessage::GetEnd();

	// �t�F�[�h���̎擾 
	CFade::FADE fade = CFade::GetFade();

	// ���j���̎擾
	int nDefeat = CDefeat::GetDefeat();

	// �N�����̎擾
	int nInvasion = CInvasion::GetInvasion();

	// �v���C���[�̏�Ԏ擾
	CPlayer::PLAYERSTATE state = CPlayer::GetState();

	// ��̓t�F�[�Y��
	if (nDefeat >= m_nClearDefeat && m_bMotherEnemy == false)
	{
		m_bMotherEnemy = true;
	}

	// �G�̎�������
	if (m_bGameStart == true && m_bMotherEnemy == false)
	{// �ʏ�G�̃t�F�[�Y
		// ���[�J���ϐ��錾
		bool bTrue = false;	// �o�����@�������ɉ����ēK�؂�

		if (m_bSetEnemy == false)
		{
			// ���[�J���ϐ��錾
			bool aType[CBulletUI::BUITYPE_MAX];

			// �e�ۗL���̎擾
			for (int nCnt = 1; nCnt < CBulletUI::BUITYPE_MAX; nCnt++)
			{
				aType[nCnt] = CBulletUI::GetTakeBullet(nCnt);
			}

			// ���L�A�C�e���̉����ēG�̏o�����@���w��
			while (bTrue != true)
			{
				// �o��������@�̎w��
				m_nSetType = rand() % CBulletUI::BUITYPE_MAX;
				
				if (aType[m_nSetType] == true || m_nSetType <= 1)
				{// �L���̏ꍇ�A���[�v�I��
					bTrue = true;
				}
			}
			
			// �z�u���ݒ芮��
			m_bSetEnemy = true;
		}

		// �ґ�����
		if (CEnemyFormation::GetIntervalBool() == false)
		{
			if (m_bSetEnemy == true)
			{					
				if (CDefeat::GetDefeat() >= m_nPhase * 10 && CDefeat::GetDefeat() % 10 == 0 && CDefeat::GetDefeat() != 0)
				{// 10�̂���
					m_nPhase++;
					m_bSetEnemy = false;
				}
			}

			// �G�̔z�u
			SetEnemyPattern(m_nSetType);
		}
		else
		{
			CEnemyFormation::FormInterval(120);
		}
	}
	else if (m_bMotherEnemy == true)
	{// ��̓t�F�[�Y
		if (m_bSetSE == false)
		{// �o�ꉉ�o
			CSound::Stop();	// BGM�̈ꎞ��~
			CSound::Play(CSound::SOUND_LABEL_SE_WARNING);
			CEnemy::UninitAll();
			CBullet::UninitAll();
			m_bSetSE = true;
		}

		if (CCaution::GetCautionState() == CCaution::CAUTION_STATE_ALERT_END)
		{
			if (m_bSetMother == false)
			{// BGM�̍Đ��ƕ�͂̐���
				CEnemyMother::MotherSetCreate();

				if (CManager::GetBGMPlay(CSound::BGMTYPE_BOSS) == true)
				{
					CSound::Play(CSound::SOUND_LABEL_BGM005);
				}

				m_bSetMother = true;
			}		
		}	
	}

	if (CEnemyMother::GetMotherState() == CEnemyMother::MOTHER_STATE_END || nInvasion >= m_nEndInvasion || state == CPlayer::PLAYERSTATE_GAMEOVER)
	{
		// �Q�[���I�����o
		Ending();
	}
}

//==============================================================================
// �Q�[�����[�h�̕`�揈��
//==============================================================================
void CGame::Draw(void)
{
	CMessage::Draw();

	CPause::Draw();
}

//==============================================================================
// �Q�[�����[�h�̐�������
//==============================================================================
CGame *CGame::Create()
{
	// �C���X�^���X����
	CGame *pGame;
	pGame = new CGame;
	D3DXVECTOR3 pos;

	// ������
	pGame->Init(pos);

	return pGame;
}

//==============================================================================
// �ʒu�̎擾
//==============================================================================
D3DXVECTOR3 CGame::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// �T�C�Y�̎擾
//==============================================================================
D3DXVECTOR3 CGame::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}

//==============================================================================
// �G�z�u�̕��@�w��
//==============================================================================
void CGame::SetEnemyPattern(int nType)
{
	switch (nType)
	{
	case 0:	// �P�̃����_��
		CEnemy::AutoSetCreate();
		break;

	case 1:	// �����_���o��
		CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_RANDOM);
		break;

	case 2:	// ���T�C�h
		CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_SIDE);
		break;

	case 3:	// ��ʉ���
		CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_UNDER);
		break;
	}
}

//==============================================================================
// �Q�[���I���܂ł̉��o
//==============================================================================
void CGame::Ending(void)
{
	// �I����Ԃ̐ݒ�
	if (CInvasion::GetInvasion() >= m_nEndInvasion || CPlayer::GetState() == CPlayer::PLAYERSTATE_GAMEOVER)
	{
		// �Q�[���I�[�o�[��Ԃ̐ݒ�
		CResult::SetEndState(CResult::ENDSTATE_GAMEOVER);
	}
	else if (CEnemyMother::GetMotherState() == CEnemyMother::MOTHER_STATE_END)
	{
		// �Q�[���N���A��Ԃ̐ݒ�
		CResult::SetEndState(CResult::ENDSTATE_CLEAR);
	}

	if (m_bSetLogo == false)
	{
		// ���S�̕\��
		CResultLogo::Create(RESULT_GAMEEND);

		m_bSetLogo = true;
	}

	// �Q�[���I���܂ł̊Ԋu
	CntDownEnd();
}

//==============================================================================
// �Q�[���I���܂ł̃J�E���g�_�E��
//==============================================================================
void CGame::CntDownEnd(void)
{
	// �t�F�[�h���̎擾 
	CFade::FADE fade = CFade::GetFade();

	if (m_nCntDownEnd == 0)
	{
		CBullet::UninitAll();
		CEnemy::UninitAll();
	}

	// �J�E���g���Z
	m_nCntDownEnd++;

	if (m_nCntDownEnd >= 180)
	{// ��3�b
		// ���ʃX�R�A�̍X�V
		CScore::SetResultScore(CScore::GetScore());

		// �t�F�[�h�J�n
		if (fade == CFade::FADE_NONE)
		{
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
		}
	}
}