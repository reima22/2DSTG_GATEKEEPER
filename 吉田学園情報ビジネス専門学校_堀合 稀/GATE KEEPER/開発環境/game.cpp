//==============================================================================
//
// �Q�[����ʂ̕`��kgame.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "ui.h"
#include "fade.h"
#include "item.h"
#include "pause.h"
#include "counter.h"
#include "life.h"
#include "effect.h"
#include "particle.h"
#include "bullet_ui.h"
#include "line.h"
#include "stock.h"
#include "warning.h"
#include "gamepad.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define ENEMY_POSX			(50.0f)		// �G�̏���X���W
#define ENEMY_POSY			(-20.0f)	// �G�̏���Y���W

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
bool g_bPause;

//==============================================================================
// �Q�[����ʂ̏���������
//==============================================================================
HRESULT InitGame(void)
{
	// �����������猻���������O
	srand((unsigned int)time(0));

	// �ϐ��̏�����
	g_bPause = false;

	// �w�i�|���S���̏������ݒ�
	InitBg();

	// ���[�U�[�C���^�[�t�F�[�X�̏������ݒ�
	InitUI();
	InitBulletUI();
	InitLine();
	InitStock();

	// �v���C���[�̏���������
	InitPlayer();

	// �G�̏������ݒ�
	InitEnemy();

	// �G�̏����z�u
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY, 0.0f), 0, 5);
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY - 70.0f, 0.0f), 1, 8);
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY - 140.0f, 0.0f), 2, 12);
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY - 210.0f, 0.0f), 3, 15);
	}

	// �A�C�e���̏���������
	InitItem();

	// �e�̏������ݒ�
	InitBullet();

	// �����̏���������
	InitExplosion();

	// �G�t�F�N�g�̏���������
	InitParticle();

	// �X�R�A�̏���������
	InitScore();

	// �J�E���^�[�̏���������
	InitCounter();

	// �|�[�Y���j���[�̏���������
	InitPause();

	// �̗͕\���̏���������
	InitLife();

	// �x����ʂ̏������ݒ�
	InitWarning();

	// BGM�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);

	return S_OK;
}

//==============================================================================
// �Q�[����ʂ̏I������
//==============================================================================
void UninitGame(void)
{
	// �T�E���h�̒�~����
	StopSound();

	// �w�i�|���S���̏I������
	UninitBg();

	// ���[�U�[�C���^�[�t�F�[�X�̏I������
	UninitUI();
	UninitBulletUI();
	UninitLine();
	UninitStock();

	// �e�̏I������
	UninitBullet();

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�̏I������
	UninitEnemy();

	// �A�C�e���̏I������
	UninitItem();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�N�g�̏I������
	UninitParticle();

	// �X�R�A�̏I������
	UninitScore();

	// �J�E���^�[�̏I������
	UninitCounter();

	// �|�[�Y���j���[�̏I������
	UninitPause();

	// �̗͕\���̏I������
	UninitLife();

	// �x����ʂ̏I������
	UninitWarning();
}

//==============================================================================
// �Q�[����ʂ̍X�V����
//==============================================================================
void UpdateGame(void)
{
	// ���[�J���ϐ��錾
	PLAYER *Player;
	PAUSE_MENU pause;
	FADE fade;
	ENEMYINFO info = GetInfo();		// �G�̑S�̏��̎擾
	PHASESTATE phase = GetPhase();	// �x����Ԃ̎擾

	// �v���C���[�̎擾
	Player = GetPlayer();

	// �|�[�Y�̎擾
	pause = GetPause();

	// �t�F�[�h�̎擾
	fade = GetFade();

	// �|�[�Y���j���[�ֈڍs
	if (info.nStartCnt >= OPEN_CNT)
	{
		if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || IsButtonDown(KEYINFO::KEYINFO_PAUSE) == true)
		{
			// ���̍Đ�
			PlaySound(SOUND_LABEL_SE_START000);
			g_bPause = g_bPause ? false : true;	// �|�[�Y�̐؂�ւ�
		}
	}

	if (g_bPause == true)
	{ // �|�[�Y���̐ݒ�
		UpdatePause();	// �|�[�Y�̍X�V����

		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{ // �I�����̏���
			if (fade != FADE_OUT)
			{ // �I�������̏d���h�~
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_START000);
			}
			
			if (pause == PAUSE_MENU_CONTINUE)
			{ // CONTINUE�̎�
				g_bPause = false;				// �|�[�Y�̉���(�Q�[���̑��s)
			}
			else if(pause == PAUSE_MENU_RETRY)
			{ // RETRY�̎�
				SetFade(FADE_OUT, MODE_GAME);	// �Q�[���̂�蒼��
			}
			else if (pause == PAUSE_MENU_QUIT)
			{ // QUIT�̎�
				SetFade(FADE_OUT, MODE_TITLE);	// �^�C�g���ֈڍs
			}
		}
	}
	else
	{ // ��|�[�Y���̏���
		if (info.boss != BIGENEMY_DEATH && Player->state != PLAYERSTATE_GAMEOVER)
		{ // �Q�[�����̏���
		  // �w�i�|���S���̍X�V����
			UpdateBg();

			// ���[�U�[�C���^�[�t�F�[�X�̍X�V����
			UpdateUI();
			UpdateBulletUI();
			UpdateLine();
			UpdateStock();

			// �e�̍X�V����
			UpdateBullet();

			// �G�̍X�V����
			UpdateEnemy();

			// �A�C�e���̍X�V����
			UpdateItem();

			// �v���C���[�X�V���̏���
			if (info.nStartCnt >= OPEN_CNT && phase == PHASESTATE_NORMAL)
			{
				// �v���C���[�̍X�V����
				UpdatePlayer();
			}

			// �����̍X�V����
			UpdateExplosion();

			// �G�t�F�N�g�̍X�V����
			UpdateParticle();

			// �X�R�A�̍X�V����
			UpdateScore();

			// �J�E���^�[�̍X�V����
			UpdateCounter();

			// �̗͕\���̍X�V����
			UpdateLife();

			// �x����ʂ̍X�V����
			UpdateWarning();
		}

		// ���U���g��ʂւ̑J��
		if (info.boss == BIGENEMY_DEATH || Player->state == PLAYERSTATE_GAMEOVER)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//==============================================================================
// �Q�[����ʂ̕`�揈��
//==============================================================================
void DrawGame(void)
{
	// �w�i�|���S���̕`�揈��
	DrawBg();

	// �G�̕`�揈��
	DrawEnemy();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �e�̕`�揈��
	DrawBullet();

	// �����̕`�揈��
	DrawExplosion();

	// �G�t�F�N�g�̍X�V����
	DrawParticle();
	
	// �|���S���̕`�揈��
	DrawPlayer();

	// �x����ʂ̕`�揈��
	DrawWarning();

	// ���[�U�[�C���^�[�t�F�[�X�̕`�揈��
	DrawUI();
	DrawBulletUI();
	DrawLine();
	DrawStock();

	// �X�R�A�̕`�揈��
	DrawScore();

	// �J�E���^�[�̕`�揈��
	DrawCounter();

	// �̗͕\���̕`�揈��
	DrawLife();

	// �|�[�Y�̕`�揈��
	if (g_bPause == true)
	{
		DrawPause();
	}
}