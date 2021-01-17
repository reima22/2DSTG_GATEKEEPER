//==============================================================================
//
// �u���b�N�ړ��_�̕`��kblockpoint.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BLOCKPOINT_H_
#define _BLOCKPOINT_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define POINT_MAX	(64)	// �ړ��_�̍ő吔
#define POINT_SIZEX	(30)	// �ړ��_�̕�
#define POINT_SIZEY	(30)	// �ړ��_�̍���

//==============================================================================
// �u���b�N�ړ��_�̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	float fWidth;		// ��
	float fHeight;		// ����
	bool bUse;			// �g�p���Ă��邩�ǂ���
} BLOCKPOINT;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitBlockPoint(void);	// �u���b�N�ړ��_�̏���������
void UninitBlockPoint(void);	// �u���b�N�ړ��_�̏I������
void UpdateBlockPoint(void);	// �u���b�N�ړ��_�̍X�V����
void DrawBlockPoint(void);		// �u���b�N�ړ��_�̕`�揈��
BLOCKPOINT *GetPoint(void);		// �ړ��_�̎擾
void SetPoint(					// �ړ��_�̐ݒ�
	D3DXVECTOR3 pos,			// �ʒu
	float fWidth,				// ��
	float fHeight);				// ����
void SetVertexPoint(int nIdx);	// �ړ��_�̒��_���W�擾

#endif