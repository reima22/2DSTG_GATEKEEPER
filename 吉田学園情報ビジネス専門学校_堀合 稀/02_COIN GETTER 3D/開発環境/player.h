//==============================================================================
//
// 3D���f���̕`��kplayer.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MODEL_PARTS		(14)	// ���f���p�[�c�̍ő吔
#define MAX_KEY			(8)		// �L�[���̍ő吔
#define FRAME_NUM		(50)	// �t���[���̍ő吔
#define MBF_NEU_MOVE	(3)		// ���[�V�����u�����h�t���[���F�j���[�g��������ړ�
#define MBF_MOVE_NEU	(15)	// ���[�V�����u�����h�t���[���F�ړ�����j���[�g����
#define MBF_NEU_ACT		(15)	// ���[�V�����u�����h�t���[���F�j���[�g��������A�N�V����
#define MBF_ACT_NEU		(40)	// ���[�V�����u�����h�t���[���F�A�N�V��������j���[�g����
#define MBF_LND_NEU		(10)	// ���[�V�����u�����h�t���[���F���n����j���[�g����

//==============================================================================
// �v���C���[�̏��
//==============================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	// �o��
	PLAYERSTATE_NORMAL,		// �ʏ�
	PLAYERSTATE_DAMAGE,		// �_���[�W
	PLAYERSTATE_WAIT,		// �o���҂�
	PLAYERSTATE_DEATH,		// ���S
	PLAYERSTATE_CLEAR,		// �Q�[���N���A
	PLAYERSTATE_GAMEOVER,	// �Q�[���I�[�o�[
	PLAYERSTATE_MAX
} PLAYERSTATE;

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// �j���[�g�������[�V����
	MOTIONTYPE_MOVE,		// �ړ����[�V����
	MOTIONTYPE_ACTION,		// �A�N�V�������[�V����
	MOTIONTYPE_JUMP,		// �W�����v���[�V����
	MOTIONTYPE_LANDING,		// ���n���[�V����
	MOTIONTYPE_DAMAGE,		// ���ꃂ�[�V����
	MOTIONTYPE_MAX			// ���[�V�����̍ő吔
} MOTIONTYPE;

//==============================================================================
// �L�[�̍\����
//==============================================================================
typedef struct
{
	float fPosX;	// �ʒuX
	float fPosY;	// �ʒuY
	float fPosZ;	// �ʒuZ
	float fRotX;	// ����X
	float fRotY;	// ����Y
	float fRotZ;	// ����Z
} KEY;

//==============================================================================
// �L�[���̍\����
//==============================================================================
typedef struct
{
	int nFrame;				// �Đ��t���[��
	int nBrendFrame;		// ���[�V�����u�����h�p�Đ��t���[��
	KEY aKey[MODEL_PARTS];	// �e���f���̃L�[�v�f 
} KEY_SET;

//==============================================================================
// ���[�W�������̍\����
//==============================================================================
typedef struct
{
	bool bLoop;				// ���[�v���邩
	int nNumKey;			// �L�[�̑���
	KEY_SET aKeySet[10];	// �L�[���
} MOTION_INFO;

//==============================================================================
// ���f���̍\����
//==============================================================================
typedef struct
{
	LPD3DXMESH pMeshModel;		// ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMatModel;	// �}�e���A���ւ̃|�C���^
	DWORD nNumMatModel;			// �}�e���A���̐�
	D3DXVECTOR3 pos;			// �ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 posReset;		// ���f���̏����ʒu
	D3DXVECTOR3 rot;			// ����
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	int nIdxModelParent;		// �e���f���̃C���f�b�N�X
	char aFileName[128];		// �ǂݍ���X�f�[�^�̃p�X��
	D3DXVECTOR3 posMemo;		// �ۑ��p�ʒu���
	D3DXVECTOR3 rotMemo;		// �ۑ��p�p�x���
}Model;

//==============================================================================
// �����蔻��̍\����
//==============================================================================
typedef struct
{
	float fFront;		// ����
	float fBack;		// ���
	float fLeft;		// ��
	float fRight;		// �E
	float fFrontLeft;	// ���O
	float fFrontRight;	// �E�O
	float fBackLeft;	// �����
	float fBackRight;	// �E���
}Collision;

//==============================================================================
// �v���C���[�̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;							// �ʒu
	D3DXVECTOR3 posOld;							// �ߋ��̈ʒu
	D3DXVECTOR3 rot;							// �p�x
	D3DXMATRIX mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 move;							// �ړ���
	D3DXVECTOR3 rotDest;						// �ߋ��̊p�x
	PLAYERSTATE state;							// �v���C���[�̏��
	int nLife;									// �̗�
	int nIdx;									// �e�̊��蓖�Ĕԍ�
	D3DXVECTOR3 vtxMinPlayer, vtxMaxPlayer;		// �����蔻��̍ő�ŏ��l

	Model aModel[MODEL_PARTS];					// ���f��(�p�[�c)
	int nNumModel;								// ���f��(�p�[�c)��

	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	// ���[�V�������
	int nNumMotion = MOTIONTYPE_MAX;			// ���[�V������
	MOTIONTYPE motionType;						// ���[�V�����^�C�v
	MOTIONTYPE motionTypeOld;					// ���O�̃��[�V�����^�C�v
	bool bLoopMotion;							// ���[�v���邩�ǂ���
	int nNumKey;								// �L�[��
	int nKey;									// �L�[��
	int nCounterMotion;							// ���[�V�����̃J�E���^�[

	bool bOnBlock;								// �����ɏ���Ă��邩

	int nCntState;								// ��ԕω��̃J�E���^�[
	float fInRadius;							// ���f���Q�Ƃ̓����a
	float fOutRadius;							// �v���C���[�̊O���a
	float fHeight;								// �v���C���[�̍���

	//Collision collision;						// �v���C���[�̓����蔻��

	int nJumpCnt;								// �W�����v�͂̃J�E���g
	int nMoveCnt;								// �ړ����[�V�����J�E���g
	bool bMotionChange;							// ���[�V�����ω����̔���
	int nBrendFrame;							// ���[�V�����u�����h���̃t���[����
} Player;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitPlayer(void);					// �v���C���[�̏���������
void UninitPlayer(void);					// �v���C���[�̏I������
void UpdatePlayer(void);					// �v���C���[�̍X�V����
void DrawPlayer(void);						// �v���C���[�̕`�揈��
Player *GetPlayer(void);					// �v���C���[�̎擾
void UpdateMotion(MOTIONTYPE motionType);	// ���[�V�����̍X�V
void LoadMotion(void);						// .txt�t�H���_�̓ǂݍ���
void StateChange(void);						// ��ԕω�
void MovePlayer(void);						// �v���C���[�̑���

#endif