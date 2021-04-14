//==============================================================================
//
// �|�[�Y���S�̕`��kpause_logo.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _PAUSE_LOGO_H_
#define _PAUSE_LOGO_H_
#include "main.h"

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;			// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	// �o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;							// �ʒu
	D3DXCOLOR col;								// �F
	bool bPlus;									// �����x�����Z����
	int nCntAnim;								// �A�j���[�V�����J�E���g
}PauseLogo;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitPauseLogo(void);	// �`���[�g���A���̏���������
void UninitPauseLogo(void);	// �`���[�g���A���̏I������
void UpdatePauseLogo(void);	// �`���[�g���A���̍X�V����
void DrawPauseLogo(void);	// �`���[�g���A���̕`�揈��

#endif