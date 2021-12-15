//==============================================================================
//
// �u���b�N�̕`��kblock.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define BLOCK_MAX	(256)	// �u���b�N�̍ő��
#define BLOCK_SIZEX	(30)	// �u���b�N�̕�
#define BLOCK_SIZEY	(30)	// �u���b�N�̍���

//==============================================================================
// �u���b�N�ɐG��Ă�����̂̎��
//==============================================================================
typedef enum
{
	ONTYPE_NONE,	// ��������Ă��Ȃ�
	ONTYPE_PLAYER,	// �v���C���[
	ONTYPE_ENEMY,	// �G
	ONTYPE_MAX
}ONTYPE;

//==============================================================================
// �u���b�N�̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	float fWidth;		// ��
	float fHeight;		// ����
	bool bUse;			// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3 move;	// �ړ���
	ONTYPE type;		// �ڐG���Ă�����̂̎��
	int nMoveCnt;		// �u���b�N�̈ړ��J�E���g
	int nMaxCnt;		// �ړ��J�E���g�̏��
} BLOCK;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitBlock(void);		// �u���b�N�̏���������
void UninitBlock(void);			// �u���b�N�̏I������
void UpdateBlock(void);			// �u���b�N�̍X�V����
void DrawBlock(void);			// �u���b�N�̕`�揈��
void SetBlock(					// �u���b�N�̐ݒ�
	D3DXVECTOR3 pos,			// �ʒu
	D3DXVECTOR3 move,			// �ړ���
	float fWidth,				// ��
	float fHeight,				// ����
	int nMaxCnt);				// �J�E���g�̏��
void SetVertexBlock(int nIdx);	// �u���b�N�̒��_���W�擾
BLOCK *GetBlock(void);			// �u���b�N�̏��擾
bool CollisionBlock(			// �u���b�N�̓����蔻��̐ݒ�
	D3DXVECTOR3 *pPos,			// ���݂̈ʒu
	D3DXVECTOR3 *pPosOld,		// �O��̈ʒu
	D3DXVECTOR3 *pMove,			// �ړ���
	float fWidth,				// ��
	float fHeight,				// ����
	ONTYPE type);				// �u���b�N�G��Ă�����̂̎��

#endif