//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE,			// �^�C�g��BGM
	SOUND_LABEL_BGM_GAME,			// �Q�[��BGM
	SOUND_LABEL_BGM_RESULT_CLEAR,	// �Q�[���N���A���U���gBGM
	SOUND_LABEL_BGM_RESULT_FAILED,	// �Q�[���I�[�o�[���U���gBGM
	SOUND_LABEL_BGM_RANKING,		// �����L���OBGM
	SOUND_LABEL_SE_DAMAGE,			// ��_���[�W��
	SOUND_LABEL_SE_DECIDE,			// ����
	SOUND_LABEL_SE_COIN,			// �R�C���擾
	SOUND_LABEL_SE_JUMP,			// �W�����v
	SOUND_LABEL_SE_PAUSE_SELECT,	// �|�[�Y�J�[�\���ړ�
	SOUND_LABEL_SE_SCORE_UP,		// �X�R�A�㏸
	SOUND_LABEL_SE_NODAMAGE,		// �m�[�_���[�W�{�[�i�X�l��
	SOUND_LABEL_SE_STOMP,			// �G�𓥂މ�
	SOUND_LABEL_SE_RANKIN,			// �����N�C����
	SOUND_LABEL_SE_COUNTDOWN,		// �X�^�[�g�J�E���g�_�E����
	SOUND_LABEL_SE_GAMESTART,		// �Q�[���J�n��
	SOUND_LABEL_SE_HIGHSCORE,		// �n�C�X�R�A�A�C�e���l����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
