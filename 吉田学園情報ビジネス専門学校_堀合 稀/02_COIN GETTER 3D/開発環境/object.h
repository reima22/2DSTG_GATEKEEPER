//==============================================================================
//
// 3D���f���̕`��kobject.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_OBJECT		(32)	// �I�u�W�F�N�g�̍ő吔
#define MAX_TEX_OBJECT	(4)		// �e�N�X�`�����̍ő�
#define COLLISION_PARTS	(4)		// �����蔻��̖ʂ̐�
#define MAX_OBJECT_TYPE	(4)		// �I�u�W�F�N�g�̎�ލő吔

//==============================================================================
// �I�u�W�F�N�g�̎�ނ��Ƃ̍\����
//==============================================================================
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX_OBJECT] = {};	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;							// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;						// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMat = NULL;								// �}�e���A���̐�
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;				// ���f���̊e���W�̍ő�l�E�ŏ��l
	char aFileName[TEXT_LENGTH];
}ObjectType;

//==============================================================================
// �I�u�W�F�N�g���Ƃ̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;							// �ʒu
	D3DXVECTOR3 rot;							// �p�x
	D3DXMATRIX mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 move;							// �ړ���
	D3DXVECTOR3 rotDest;						// �ړI�̊p�x
	int nIdx;									// �ۉe�ݒ�ԍ�
	D3DXVECTOR3 aPos[COLLISION_PARTS];			// �I�u�W�F�N�g4���_
	D3DXVECTOR3 aVec[COLLISION_PARTS];			// �I�u�W�F�N�g4�ӂ̃x�N�g��
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;		// ���f���̊e���W�̍ő�l�E�ŏ��l
	float fPlayerVec[COLLISION_PARTS];			// �����蔻��p�ϐ�
	int nType;									// �I�u�W�F�N�g�̎��
	bool bUse;									// �g�p���
}ObjectInfo;

//==============================================================================
// �I�u�W�F�N�g�̍\����
//==============================================================================
typedef struct
{
	ObjectInfo objectInfo[MAX_OBJECT];
	ObjectType objectType[MAX_OBJECT_TYPE];
	int nNumObject;
	int nSetObject;
}Object;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitObject(void);	// 3D�I�u�W�F�N�g�̏���������
void UninitObject(void);	// 3D�I�u�W�F�N�g�̏I������
void UpdateObject(void);	// 3D�I�u�W�F�N�g�̍X�V����
void DrawObject(void);		// 3D�I�u�W�F�N�g�̕`�揈��
Object *GetObject(void);	// 3D�I�u�W�F�N�g�̎擾
void SetObject(				// 3D�I�u�W�F�N�g�̐ݒ�
	D3DXVECTOR3 pos,
	int nType,
	D3DXVECTOR3 rot);
bool CollisionVec(			// 3D�I�u�W�F�N�g�̓����蔻��
	D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,
	float fInRadius,
	float fHeight);
void LoadObject(void);		// �I�u�W�F�N�g�̓ǂݍ���

#endif