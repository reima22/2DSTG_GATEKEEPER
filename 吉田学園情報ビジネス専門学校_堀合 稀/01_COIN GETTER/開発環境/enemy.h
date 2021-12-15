//==============================================================================
//
// �G�̕`��kenemy.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_ENEMY			(256)		// �G�̍ő吔
#define MAX_TYPE_ENEMY		(5)			// �G�̎��

//==============================================================================
// �G�̏��
//==============================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// �ʏ���
	ENEMYSTATE_DAMAGE,		// �_���[�W���
	ENEMYSTATE_MAX
} ENEMYSTATE;

//==============================================================================
// �G�̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 posOld;	// �O��̈ʒu
	float fWidth;		// ��
	float fHeight;		// ����
	int nType;			// ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
	ENEMYSTATE state;	// ���
	int nCounterState;	// ��ԃJ�E���^�[
	int nLife;			// �̗�
	D3DXVECTOR3 move;	// �ړ���
	int nAnimCnt;
	float fTex;
} ENEMY;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitEnemy(void);		// �G�̏���������
void UninitEnemy(void);			// �G�̏I������
void UpdateEnemy(void);			// �G�̍X�V����
void DrawEnemy(void);			// �G�̕`�揈��
void SetEnemy(					// �G�̐ݒ�
	D3DXVECTOR3 pos,			// �ʒu
	D3DXVECTOR3 move,			// �ړ���
	float fWidth,				// ��
	float fHeight,				// ����
	int nType,int nLife);		// �^�C�v�A�̗�
void SetVertexEnemy(int nIdx);	// �G�̒��_���W�擾
void MoveEnemy(void);			// �G�̈ړ�
ENEMY *GetEnemy(void);			// �G�̏��
void AnimEnemy(void);			// �G�̃A�j���[�V����

#endif
