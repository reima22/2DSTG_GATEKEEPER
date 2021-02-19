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

// ���f���̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;	// ���f���̊e���W�̍ő�l�E�ŏ��l
	int nIdx;
	D3DXVECTOR3 posPoint[FOUR_POINT];	// 4���_�̈ʒu
	D3DXVECTOR3 vecPoint[FOUR_POINT];	// �l�ӂ̃x�N�g��
	float fVec[4];						// �v�Z�p�x�N�g��
	int nMovePattern;					// �ړ��p�^�[���Ǘ�
	bool bUse;
} Enemy;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitEnemy(void);	// �G�̏���������
void UninitEnemy(void);		// �G�̏I������
void UpdateEnemy(void);		// �G�̍X�V����
void DrawEnemy(void);		// �G�̕`�揈��
Enemy *GetEnemy(void);		// �G�̎擾
void SetEnemy(D3DXVECTOR3 pos,D3DXVECTOR3 move,int nMove);
bool TouchEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

//bool CollisionObject(		// �����蔻��
//	D3DXVECTOR3 *pPos,
//	D3DXVECTOR3 *pPosOld,
//	D3DXVECTOR3 *pMove,
//	float fWidthMax,
//	float fWidthMin,
//	float fDepthMax,
//	float fDepthMin);
//
//bool CollisionVec(
//	D3DXVECTOR3 *pPos,
//	D3DXVECTOR3 *pPosOld,
//	D3DXVECTOR3 *pMove,
//	float fWidthMax,
//	float fWidthMin,
//	float fDepthMax,
//	float fDepthMin,
//	float fHeightMax,
//	float fheightMin);

#endif
