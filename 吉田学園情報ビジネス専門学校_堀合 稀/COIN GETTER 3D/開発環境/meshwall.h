//==============================================================================
//
// ���b�V���ǂ̕`��kmeshwall.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _MESHWALL_H_
#define _MESHWALL_H_
#include "main.h"

// �}�N����`
#define MAX_MWALL		(4)
#define MAX_WALL_TYPE	(4)

// �ǂ̎�ނ��Ƃ̏��
typedef struct
{
	LPDIRECT3DTEXTURE9 pTextureMeshwall = {};	// �e�N�X�`���ւ̃|�C���^
	char aFileName[TEXT_LENGTH];				// �e�L�X�g�ǂݎ��p�ϐ�
}WallType;

// �ǂ̏ڍ׏��
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �����ʒu
	D3DXVECTOR3	posMove;	// �����ʒu����̈ړ������ʒu
	D3DXVECTOR3 rot;		// ����
	float fWidth;			// ��
	float fHeight;			// ����
	float fWidthMax;
	float fHeightMax;
	int nWidth;				// ��
	int nHeight;			// �s��
	int nWidthPoint;		// ���̒��_��
	int nHeightPoint;		// �����̒��_��
	int nAllPoint;
	int nPolygon;
	int nIdxPoint;
	bool bUse;				// �g�p���
	D3DXVECTOR3 posPoint[FOUR_POINT];	// 4���_�̈ʒu
	D3DXVECTOR3 vecPoint[FOUR_POINT];	// �l�ӂ̃x�N�g��
	int nType;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuffMeshwall = NULL;	// �o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuffMeshwall = NULL;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
}WallInfo;

// �ǂ̍\����
typedef struct
{
	WallInfo wallInfo[MAX_MWALL];		// �ǂ̏ڍ׏��
	WallType wallType[MAX_WALL_TYPE];	// �ǂ̎�ނ��Ƃ̏��
	int nNumTex;						// �e�N�X�`���̐�
	int nNumWall;						// �z�u����ǂ̐�

} Meshwall;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitMeshwall(void);		// ���b�V���ǂ̏���������
void UninitMeshwall(void);		// ���b�V���ǂ̏I������
void UpdateMeshwall(void);		// ���b�V���ǂ̍X�V����
void DrawMeshwall(void);		// ���b�V���ǂ̕`�揈��
void SetMeshwall(				// ���b�V���ǂ̐ݒ菈��
	D3DXVECTOR3 pos, 
	D3DXVECTOR3 rot, 
	float fWidth, 
	float fHeight,
	int nWidth,
	int nHeight);	

void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 move, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

void LoadWall(void);		// �ǂ̃e�L�X�g�ǂݍ���

#endif
