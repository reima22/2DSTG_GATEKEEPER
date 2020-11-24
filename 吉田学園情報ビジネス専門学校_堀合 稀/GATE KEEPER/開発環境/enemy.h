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
#define MAX_TYPE_ENEMY		(6)			// �G�̎��
#define OPEN_CNT			(160)		// �I�[�v�j���O�̃J�E���g���
#define ENEMY_SIZEX			(20)		// �G�̕�
#define ENEMY_SIZEY			(20)		// �G�̍���
#define BIGENEMY_SIZEX		(100)		// ��^�̓G�̕�
#define BIGENEMY_SIZEY		(50)		// ��^�̓G�̍���
#define BIGENEMY_APPEAR		(150)		// �{�X�o���̌��j��

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
// �{�X�̏��
//==============================================================================
typedef enum
{
	BIGENEMY_NORMAL = 0,	// �ʏ�
	BIGENEMY_DEATH,			// ���S
	BIGENEMY_MAX
} BIGENEMY;

//==============================================================================
// �G�̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nType;			// ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
	ENEMYSTATE state;	// ���
	int nCounterState;	// ��ԃJ�E���^�[
	int nLife;			// �̗�
	D3DXVECTOR3 move;	// �ړ���
	int nRandAngle;		// �g�`�ړ��p�x�̃����_���ݒ�
	float fAngle;		// �g�`�ړ�
	int nShotCnt;		// �G�̒e���˃J�E���g
} ENEMY;

//==============================================================================
// �G�S�̂̏ڍ׏��
//==============================================================================
typedef struct
{
	int nDestroy;		// �G�̌��j��
	int nVanish;		// �G�̏��Ő�
	int nShotCnt;		// �G�̒e���˃J�E���g
	int nStartCnt;		// �I�[�v�j���O�J�E���g
	int nSetCounter;	// �G�̐ݒ�J�E���g
	int nUfoCnt;		// UFO�̐ݒ�J�E���g
	int nUfoSet;		// UFO�̐ݒ�^�C�~���O
	int nBigEnemyCnt;	// �{�X�̐ݒ�J�E���g
	BIGENEMY boss;		// �{�X�̏��
} ENEMYINFO;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitEnemy(void);								// �G�̏���������
void UninitEnemy(void);									// �G�̏I������
void UpdateEnemy(void);									// �G�̍X�V����
void DrawEnemy(void);									// �G�̕`�揈��
void SetEnemy(D3DXVECTOR3 pos, int nType,int nLife);	// �G�̐ݒ�
void SetEnemyDes(void);									// ���j�����Ƃ̓G�̔z�u
void SetVertexEnemy(int nIdx);							// �G�̒��_���W�擾
void SetEnemyMove(int nIdx,int nCnt);					// �G�̃^�C�v���Ƃ̈ړ��ʕω�
void SetEnemyMoveDes(int nIdx);							// ���j�����Ƃ̓G�̈ړ��ʕω�
void SetBigEnemy(void);									// �{�X�G�̔z�u
ENEMY *GetEnemy(void);									// �G�̏��
bool HitEnemy(int nIdx, int nDagame);					// �G�̖����ƃ_���[�W����
ENEMYINFO GetInfo(void);								// �G�S�̂̏��̎擾

#endif
