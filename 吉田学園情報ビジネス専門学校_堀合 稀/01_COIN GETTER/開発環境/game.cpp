//==============================================================================
//
// �Q�[����ʂ̕`��kgame.cpp�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "player.h"
#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "ui.h"
#include "fade.h"
#include "item.h"
#include "pause.h"
#include "life.h"
#include "particle.h"
#include "block.h"
#include "timer.h"
#include "gamepad.h"
#include "nodamage.h"
#include "time.h"
#include "blockpoint.h"
#include "stdlib.h"
#include "stdio.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define VERTICAL_STEP	(90)	// �c�ړ�����u���b�N�̃J�E���g�Ԋu
#define SIDE_STEP		(240)	// ���ړ�����u���b�N�̃J�E���g�Ԋu
#define BLOCKX			(46)	// �u���b�N�z�u��csv�t�@�C���̗�
#define BLOCKY			(24)	// �u���b�N�z�u��csv�t�@�C���̍s��
#define PATTERN_MAX		(8)		// �z�u�p�^�[���̍ő吔
#define POINT_SET		(15)	// �ړ��_�̔z�u�Ԋu

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
bool g_bPause;				// �|�[�Y�̏��
int g_nGameCnt = START_NUM;	// �X�e�[�W�J�E���g
int g_nStartCnt;
int g_aMapData[BLOCKY][BLOCKX];

//==============================================================================
// �Q�[����ʂ̏���������
//==============================================================================
HRESULT InitGame(void)
{
	// �����������猻���������O
	srand((unsigned int)time(0));

	// �ϐ��̏�����
	g_bPause = false;
	g_nStartCnt = g_nGameCnt;

	// �w�i�|���S���̏������ݒ�
	InitBg();

	// �^�C�}�[�̏���������
	InitTimer();

	// �A�C�e���̏���������
	InitItem();

	// �A�C�e���̔z�u
	PutItem(g_nGameCnt);

	// �v���C���[�̏���������
	InitPlayer();

	// �G�̏������ݒ�
	InitEnemy();

	// �G�̔z�u
	PutEnemy(g_nGameCnt);

	// �ړ��_�̏���������
	InitBlockPoint();

	// �u���b�N�̏���������
	InitBlock();

	// �u���b�N�̔z�u
	PutBlock(g_nGameCnt);

	// �G�t�F�N�g�̏���������
	InitParticle();

	// �X�R�A�̏���������
	InitScore();

	// �|�[�Y���j���[�̏���������
	InitPause();

	// �̗͕\���̏���������
	InitLife();

	// UI�̏������ݒ�
	InitUI();

	// �m�[�_���[�W�{�[�i�X�̏���������
	InitNodamage();

	// BGM�̍Đ�
	bool bBgm = GAME_BGM;
	if (bBgm == true)
	{
		if (g_nGameCnt == 0)
		{
			PlaySound(SOUND_LABEL_BGM001);
		}
		else if (g_nGameCnt == 1)
		{
			PlaySound(SOUND_LABEL_BGM005);
		}
		else if (g_nGameCnt == 2)
		{
			PlaySound(SOUND_LABEL_BGM006);
		}
	}

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

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�̏I������
	UninitEnemy();

	// �u���b�N�̏I������
	UninitBlock();

	// �ړ��_�̏I������
	UninitBlockPoint();

	// �A�C�e���̏I������
	UninitItem();

	// �G�t�F�N�g�̏I������
	UninitParticle();

	// �X�R�A�̏I������
	UninitScore();

	// �^�C�}�[�̏I������
	UninitTimer();

	// �|�[�Y���j���[�̏I������
	UninitPause();

	// �̗͕\���̏I������
	UninitLife();

	// UI�̏I������
	UninitUI();

	// �m�[�_���[�W�{�[�i�X�̏I������
	UninitNodamage();
}

//==============================================================================
// �Q�[����ʂ̍X�V����
//==============================================================================
void UpdateGame(void)
{
	// ���[�J���ϐ��錾
	PLAYER *Player;
	PAUSE pause;
	FADE fade;
	int nTime = GetTimer();
	int nGameCnt = g_nGameCnt;

	// �v���C���[�̎擾
	Player = GetPlayer();

	// �|�[�Y�̎擾
	pause = GetPause();

	// �t�F�[�h�̎擾
	fade = GetFade();

	// �|�[�Y���j���[�ֈڍs
	if (Player->state != PLAYERSTATE_CLEAR && Player->state != PLAYERSTATE_GAMEOVER && pause.colOption.a <= 0.0f && fade == FADE_NONE)
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
		{ // �I�����̉�ʑJ�ڏ���
			if (fade != FADE_OUT && pause.colOption.a <= 0.0f)
			{ // �I�������̏d���h�~
				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_START000);
			}
			
			if (pause.PauseMenu == PAUSE_MENU_CONTINUE)
			{ // CONTINUE�̎�
				g_bPause = false;				// �|�[�Y�̉���(�Q�[���̑��s)
			}
			else if(pause.PauseMenu == PAUSE_MENU_RETRY)
			{ // RETRY�̎�
				SetFade(FADE_OUT, MODE_GAME);	// �Q�[���̂�蒼��
				g_nGameCnt = START_NUM;
			}
			else if (pause.PauseMenu == PAUSE_MENU_QUIT)
			{ // QUIT�̎�
				SetFade(FADE_OUT, MODE_TITLE);	// �^�C�g���ֈڍs
			}
		}
	}
	else
	{ // ��|�[�Y���̏���
		if (Player->state != PLAYERSTATE_CLEAR && Player->state != PLAYERSTATE_GAMEOVER)
		{ // �Q�[�����̏���
		  // �w�i�|���S���̍X�V����
			UpdateBg();

			// �G�̍X�V����
			UpdateEnemy();

			// �A�C�e���̍X�V����
			UpdateItem();

			// �v���C���[�̍X�V����
			UpdatePlayer();

			// �ړ��_�̍X�V����
			UpdateBlockPoint();

			// �u���b�N�̍X�V����
			UpdateBlock();

			// �̗͕\���̍X�V����
			UpdateLife();

			// UI�̍X�V����
			UpdateUI();
		}

		// �G�t�F�N�g�̍X�V����
		UpdateParticle();

		if (Player->state != PLAYERSTATE_GAMEOVER)
		{
			// �X�R�A�̍X�V����
			UpdateScore();

			// �^�C�}�[�̍X�V����
			UpdateTimer();

			// �m�[�_���[�W�{�[�i�X�̍X�V����
			UpdateNodamage();
		}
	
		// ��ʂ̑J�ځi�Q�[���N���A���j
		if (nTime <= 0 && Player->state == PLAYERSTATE_CLEAR && g_nGameCnt < MAX_STAGE && g_nStartCnt == g_nGameCnt)
		{
			g_nGameCnt++;					// �X�e�[�W�J�E���g���Z
			SetFade(FADE_OUT, MODE_GAME);	// �Q�[�����[�h���s
		}
		else if(nTime <= 0 && g_nGameCnt == MAX_STAGE)
		{
			SetFade(FADE_OUT, MODE_RESULT);	// ���U���g��
		}
		
		// ��ʂ̑J�ځi�Q�[���I�[�o�[���j
		if (Player->state == PLAYERSTATE_GAMEOVER)
		{
			SetFade(FADE_OUT, MODE_RESULT);	// ���U���g��
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

	// �ړ��_�̕`�揈��
	DrawBlockPoint();

	// �G�̕`�揈��
	DrawEnemy();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �|���S���̕`�揈��
	DrawPlayer();

	// �G�t�F�N�g�̕`�揈��
	DrawParticle();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// UI�̕`�揈��
	DrawUI();

	// �X�R�A�̕`�揈��
	DrawScore();

	// �^�C�}�[�̕`�揈��
	DrawTimer();

	// �J�E���^�[�̕`�揈��
	//DrawCounter();

	// �̗͕\���̕`�揈��
	DrawLife();

	// �m�[�_���[�W�{�[�i�X�̕`�揈��
	DrawNodamage();

	// �|�[�Y�̕`�揈��
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//==============================================================================
// �Q�[���X�e�[�W�J�E���g�̏�����
//==============================================================================
HRESULT InitGameCnt(void)
{
	// �ϐ��̏�����
	g_nGameCnt = 0;

	return S_OK;
}

//==============================================================================
// �G�̔z�u
//==============================================================================
void PutEnemy(int nStage)
{
	// �X�e�[�W�ɂ��z�u�ύX
	if (nStage == 0)
	{
		// �G�̔z�u
		SetEnemy(D3DXVECTOR3(40.0f, 690.0f, 0.0f), D3DXVECTOR3(-2.5f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 0, 0);
		SetEnemy(D3DXVECTOR3(1240.0f, 690.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 0, 0);
		SetEnemy(D3DXVECTOR3(1240.0f, 500.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 1, 0);
		SetEnemy(D3DXVECTOR3(640.0f, 150.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
	}
	else if (nStage == 1)
	{
		// �G�̔z�u
		SetEnemy(D3DXVECTOR3(320.0f, 650.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(640.0f, 510.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(320.0f, 370.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(640.0f, 230.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(320.0f, 110.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 1, 0);
	}
	else if (nStage == 2)
	{
		// �G�̔z�u
		SetEnemy(D3DXVECTOR3(1240.0f, 150.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
	}
}

//==============================================================================
// �A�C�e���̔z�u
//==============================================================================
void PutItem(int nStage)
{
	// �X�e�[�W�ɂ��z�u�ύX
	if (nStage == 0)	// ���v48��
	{
		// �A�C�e���̔z�u�ݒ�
		for (int nCnt = 0; nCnt < 6; nCnt++)	// 12��
		{
			// �������ʒu
			SetItem(D3DXVECTOR3(390.0f + (nCnt * 100), 120.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(420.0f + (nCnt * 100), 230.0f, 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 8; nCnt++)	// 16��
		{
			// �n�ʈʒu
			SetItem(D3DXVECTOR3(30.0f + (nCnt * 70), 650.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1250.0f - (nCnt * 70), 650.0f, 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 6; nCnt++)	// 12��
		{
			// ���i�ʒu
			SetItem(D3DXVECTOR3(40.0f + (nCnt * 70), 460.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1240.0f - (nCnt * 70), 460.0f, 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)	// 8��
		{
			// ��i�ʒu
			SetItem(D3DXVECTOR3(40.0f + (nCnt * 70), 120.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1240.0f - (nCnt * 70), 120.0f, 0.0f), 0);
		}
	}
	else if (nStage == 1)	// ���v70��
	{
		// �A�C�e���̔z�u�ݒ�
		for (int nCnt = 0; nCnt < 14; nCnt++)	// 14��
		{
			SetItem(D3DXVECTOR3(230.0f + (nCnt * 70), 650.0f, 0.0f), 0);	// �n�ʈʒu
			
			SetItem(D3DXVECTOR3(350.0f + (nCnt * 70), 510.0f, 0.0f), 0);	// 1�i��
			
			SetItem(D3DXVECTOR3(100.0f + (nCnt * 70), 370.0f, 0.0f), 0);	// 2�i��
			
			SetItem(D3DXVECTOR3(350.0f + (nCnt * 70), 230.0f, 0.0f), 0);	// 3�i��
			
			SetItem(D3DXVECTOR3(100.0f + (nCnt * 70), 90.0f, 0.0f), 0);		// 4�i��
		}
	}
	else if (nStage == 2)
	{
		for (int nCnt = 0; nCnt < 6; nCnt++)	// 30��
		{
			SetItem(D3DXVECTOR3(70.0f, 170.0f + (nCnt * 50), 0.0f), 0);
			SetItem(D3DXVECTOR3(100.0f, 170.0f + (nCnt * 50), 0.0f), 0);
			SetItem(D3DXVECTOR3(315.0f, 120.0f + (nCnt * 90), 0.0f), 0);
			SetItem(D3DXVECTOR3(520.0f + (nCnt * 80), 390.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1115.0f, 120.0f + (nCnt * 90), 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 5; nCnt++)	// 10��
		{
			SetItem(D3DXVECTOR3(550.0f + (nCnt * 80), 520.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1250.0f, 220.0f + (nCnt * 80), 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 7; nCnt++)	// 14��
		{
			SetItem(D3DXVECTOR3(480.0f + (nCnt * 70), 120.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(500.0f + (nCnt * 70), 230.0f, 0.0f), 0);
		}
	}
}

//==============================================================================
// �u���b�N�̔z�u
//==============================================================================
void PutBlock(int nStage)
{
	// csv�t�@�C���̓ǂݍ���
	LoadBlock(nStage);
	int aType[PATTERN_MAX] = {};			// �^�C�v���Ƃ̔z�u��
	D3DXVECTOR3 posA[PATTERN_MAX] = {};		// �ŏ��ɐݒu�����ʒu
	D3DXVECTOR3 moveA[PATTERN_MAX] = {};	// �u���b�N�̈ړ���
	int aCounter[PATTERN_MAX] = {};			// �J�E���^�[�ő�l
	bool bGet[PATTERN_MAX] = { false };		// �ŏ��ɐݒu����ʒu���擾�ς݂�

	// �u���b�N�̔z�u
	for (int nCntY = 0; nCntY < BLOCKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < BLOCKX; nCntX++)
		{
			// �U�蕪�����������Ƃ̔z�u�E�ړ��p�^�[��
			if (g_aMapData[nCntY][nCntX] == 1)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-50.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int nCounter = 0;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 2)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-50.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				int nCounter = 90;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 3)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int nCounter = 0;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 4)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
				int nCounter = 210;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 5)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
				int nCounter = 210;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 6)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(2.0f, 0.0f, 0.0f);
				int nCounter = 150;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, 150);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 7)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(-2.0f, 0.0f, 0.0f);
				int nCounter = 120;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// �ϐ��̑��
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < PATTERN_MAX; nCnt++)
	{
		if (moveA[nCnt] != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			if (moveA[nCnt].y != 0.0f)
			{// �c�ړ��u���b�N
				posA[nCnt].x = posA[nCnt].x + ((float)POINT_SIZEX * (float)aType[nCnt] / 2.0f) - (float)POINT_SIZEX / 2.0f;

				// �ړ��_�̔z�u
				for (int nSet = 0; nSet < aCounter[nCnt]; nSet++)
				{
					if (nSet % POINT_SET == 0)
					{
						SetPoint(D3DXVECTOR3(posA[nCnt].x, posA[nCnt].y + moveA[nCnt].y * (float)nSet, 0.0f), POINT_SIZEX, POINT_SIZEY);
					}
				}
			}
			else if (moveA[nCnt].x != 0.0f)
			{// ���ړ��u���b�N
				if (moveA[nCnt].x < 0.0f)
				{// ���ړ��X�^�[�g
					posA[nCnt].x = posA[nCnt].x + moveA[nCnt].x * (float)aCounter[nCnt];	// �����ʒu�̏C��
					moveA[nCnt] *= -1;

					// �ړ��_�̔z�u
					for (int nSet = 0; nSet < (aCounter[nCnt] + (aType[nCnt] * POINT_SET)); nSet++)
					{
						if (nSet % POINT_SET == 0)
						{
							SetPoint(D3DXVECTOR3(posA[nCnt].x + moveA[nCnt].x * (float)nSet, posA[nCnt].y, posA[nCnt].z), POINT_SIZEX, POINT_SIZEY);
						}
					}
				}
				else
				{// �E�ړ��X�^�[�g
					for (int nSet = 0; nSet < (aCounter[nCnt] + (aType[nCnt] * POINT_SET)); nSet++)
					{
						// �ړ��_�̔z�u
						if (nSet % POINT_SET == 0)
						{
							SetPoint(D3DXVECTOR3(posA[nCnt].x + moveA[nCnt].x * (float)nSet, posA[nCnt].y, posA[nCnt].z), POINT_SIZEX, POINT_SIZEY);
						}
					}
				}
			}
		}
	}
}

//==============================================================================
// �X�e�[�W�J�E���g�̎擾
//==============================================================================
int GetStage(void)
{
	return g_nGameCnt;
}

//==============================================================================
// �u���b�N�f�[�^�̓ǂݍ���
//==============================================================================
void LoadBlock(int nStage)
{
	// ���[�J���ϐ��錾
	FILE *pFile = NULL;
	char aLine[256];
	int nCntX = 0;
	int nCntY = 0;

	// �t�@�C�����J��
	if (nStage == 0)
	{
		pFile = fopen("data/STAGE/stage00.csv", "r");
	}
	else if (nStage == 1)
	{
		pFile = fopen("data/STAGE/stage01.csv", "r");
	}
	else if (nStage == 2)
	{
		pFile = fopen("data/STAGE/stage02.csv", "r");
	}

	while (fgets(aLine, 256, pFile) != NULL)
	{
		nCntX = 0;
		char *pToken = strtok(aLine, ",");

		while (pToken != NULL)
		{
			int nNum = atoi(pToken);
			g_aMapData[nCntY][nCntX] = nNum;
			pToken = strtok(NULL, ",");
			nCntX++;
		}
		nCntY++;
	}
	fclose(pFile);
}