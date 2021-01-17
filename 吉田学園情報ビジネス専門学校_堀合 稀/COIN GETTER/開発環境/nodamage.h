//==============================================================================
//
// �m�[�_���[�W�{�[�i�X�̕`��knodamage.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _NODAMAGE_H_
#define _NODAMAGE_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define TEX_BONUS	(2)			// �e�N�X�`���̐�
#define MAX_BONUS	(6)			// �{�[�i�X�̌���
#define MAX_POLY	(7)			// �|���S���̑���
#define LOGO_POSX	(150.0f)	// �\����X���W
#define LOGO_POSY	(100.0f)	// �\����Y���W
#define LOGO_SIZEX	(150.0f)	// ���S�T�C�Y�̕�
#define LOGO_SIZEY	(20.0f)		// ���S�T�C�Y�̍���
#define BONUS_POSX	(120.0f)	// �{�[�i�X�X�R�A��1���ڂ�X���W
#define BONUS_POSY	(150.0f)	// �{�[�i�X�X�R�A��1���ڂ�Y���W
#define BONUS_SIZEX	(10.0f)		// �{�[�i�X�X�R�A1���̕�
#define BONUS_SIZEY	(20.0f)		// �{�[�i�X�X�R�A1���̍���

//==============================================================================
// �{�[�i�X�̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 posNodamage;			// �{�[�i�X�\���̈ʒu
	D3DXVECTOR3 posBonus[MAX_BONUS];	// �{�[�i�X�X�R�A�̈ʒu
	D3DXCOLOR colNodamage;				// �F�E�����x
	int nBonus;							// �{�[�i�X�X�R�A
	int nBonusCnt;						// �{�[�i�X�����̃J�E���g
} BONUS;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitNodamage(void);	// �m�[�_���[�W�{�[�i�X�̏���������
void UninitNodamage(void);	// �m�[�_���[�W�{�[�i�X�̏I������
void UpdateNodamage(void);	// �m�[�_���[�W�{�[�i�X�̍X�V����
void DrawNodamage(void);	// �m�[�_���[�W�{�[�i�X�̕`�揈��
void AddScoreBonus(void);
BONUS GetBonus(void);

#endif
