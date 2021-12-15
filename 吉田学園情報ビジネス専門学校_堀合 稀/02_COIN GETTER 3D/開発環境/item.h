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
#define MAX_ITEM_TYPE	(8)			// �A�C�e����ނ̍ő吔
#define MAX_ITEM_TEX	(4)			// �A�C�e���̍ő�e�N�X�`����

//==============================================================================
// �A�C�e���̎�ނ��Ƃ̏��
//==============================================================================
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_ITEM_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;						// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;					// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMat = 0;								// �}�e���A���̐�
	char aFileName[TEXT_LENGTH];					// �p�X���ǂݎ��p�ϐ�
	D3DXVECTOR3 vtxMinType, vtxMaxType;				// ���f���̊e���W�̍ő�l�E�ŏ��l
}ItemType;

//==============================================================================
// �A�C�e���̏ڍ׏��
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// �p�x
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	int nType;							// ���
	bool bUse;							// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3 vtxMinItem, vtxMaxItem;	// ���f���̊e���W�̍ő�l�E�ŏ��l
	D3DXVECTOR3 posPoint[FOUR_POINT];	// 4���_�̈ʒu
	D3DXVECTOR3 vecPoint[FOUR_POINT];	// �l�ӂ̃x�N�g��
	int nIdx;							// �e�ԍ�
	bool bHiScore;						// ���X�R�A��Ԃ̃R�C��
}ItemInfo;

//==============================================================================
// �A�C�e���̍\����
//==============================================================================
typedef struct
{
	ItemInfo itemInfo[MAX_ITEM];		// �A�C�e���̏ڍ׏��
	ItemType itemType[MAX_ITEM_TYPE];	// �A�C�e���̎�ނ��Ƃ̏��
	int nNumItem;						// �A�C�e���̎�ސ�
	int nSetItem;						// �e�L�X�g����̔z�u���J�E���g
	int nCntItem;						// �z�u���J�E���g
	int nCntAnim;						// �A�j���[�V�����J�E���g
	D3DMATERIAL9 matHigh;				// �n�C�X�R�A�A�C�e���̃}�e���A��
} Item;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitItem(void);			// �A�C�e���̏���������
void UninitItem(void);			// �A�C�e���̏I������
void UpdateItem(void);			// �A�C�e���̍X�V����
void DrawItem(void);			// �A�C�e���̕`�揈��
void SetItem(					// �A�C�e���̔z�u
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int nType);					
void TouchItem(					// �A�C�e���̎擾����
	D3DXVECTOR3 *pPos,
	float fInRadius,
	float fHeight);				
Item *GetItem(void);			// �A�C�e���̏��
int RandItem(void);				// �����_���Ńn�C�X�R�A�A�C�e���̑I��
void LoadItem(void);			// �A�C�e���̓ǂݍ���

#endif
