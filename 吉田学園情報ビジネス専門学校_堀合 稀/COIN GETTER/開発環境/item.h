//==============================================================================
//
// �A�C�e���̕`��kitem.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_ITEM		(128)		// �A�C�e���̍ő吔
#define MAX_TYPE_ITEM	(3)			// �A�C�e���̎��
#define ITEM_SIZEX		(15)		// �A�C�e���̕�
#define ITEM_SIZEY		(30)		// �A�C�e���̍���

//==============================================================================
// �A�C�e���̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	int nType;			// ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
	float fAnim;		// �e�N�X�`���A�j���[�V�����J�b�g��
	bool bHiScore;		// ���X�R�A��Ԃ̃R�C��
} ITEM;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitItem(void);						// �A�C�e���̏���������
void UninitItem(void);						// �A�C�e���̏I������
void UpdateItem(void);						// �A�C�e���̍X�V����
void DrawItem(void);						// �A�C�e���̕`�揈��
void SetItem(D3DXVECTOR3 pos, int nType);	// �A�C�e���̔z�u
void SetVertexItem(int nIdx);				// �A�C�e���̒��_���W�擾
ITEM *GetItem(void);						// �A�C�e���̏��
int RandItem(void);						// �����_���Ńn�C�X�R�A�A�C�e���̑I��

#endif
