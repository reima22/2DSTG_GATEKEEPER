//==============================================================================
//
// �`���[�g���A����ʂ̕`��ktutorial_main.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _TUTORIAL_MAIN_H_
#define _TUTORIAL_MAIN_H_
#include "main.h"

// �}�N����`
#define TUTOMAIN_NUM	(2)	// �`���[�g���A���\���̎��

typedef struct
{
	LPDIRECT3DTEXTURE9 pTextureTutoMain = NULL;			// �`���[�g���A����ʃe�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	bool bUse;
}TutoMainInfo;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTutoMain = NULL;	// �o�b�t�@�ւ̃|�C���^
	TutoMainInfo tutoMainInfo[TUTOMAIN_NUM];
}TutoMain;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitTutoMain(void);	// �`���[�g���A����ʂ̏���������
void UninitTutoMain(void);	// �`���[�g���A����ʂ̏I������
void UpdateTutoMain(void);	// �`���[�g���A����ʂ̍X�V����
void DrawTutoMain(void);	// �`���[�g���A����ʂ̕`�揈��

#endif