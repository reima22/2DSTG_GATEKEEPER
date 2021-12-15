//=============================================================================
//
// �J�����̐ݒ� [camera.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define CAMERA_TURN	(D3DX_PI / 100.0f)	// �J�������񑬓x
#define CAMERA_SPDOWN	(0.1f)			// �J�����̌����W��
#define CAMERA_MOVE		(5.0f)

//==============================================================================
// �J�����N���X
//==============================================================================
class CCamera
{
public:
	//typedef enum
	//{
	//	CAMERATYPE_EDIT = 0,	// �ҏW���[�h
	//	CAMERATYPE_PREVIEW,		// �v���r���[
	//	//CAMERATYPE_FPS,		// �L�����N�^�[��1�l�̎��_
	//	CAMERATYPE_MAX
	//}CAMERATYPE;

	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);

	static CCamera *Create(void);

	void SetCamera(void);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// �����̎擾

	D3DXVECTOR3 GetPositionR(void) { return m_posR; }	// �����_
	D3DXVECTOR3 GetPositionV(void) { return m_posV; }	// ���_

	float GetLength(void) { return m_fLength; }
	float GetGroundLength(void) { return m_fGroundLength; }

	void SetInit(void);

	void ControlCamera(void);

private:
	D3DXMATRIX m_mtxView;		// �r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X

	D3DXVECTOR3 m_posV;			// ���_�ʒu
	D3DXVECTOR3 m_posR;			// �����_�ʒu
	D3DXVECTOR3 m_vecU;			// ������x�N�g��
	
	D3DXVECTOR3 m_rot;			// ����

	float m_fLength;			// ���_�Ԃ̋���
	float m_fGroundLength;		// ���_(����0)�ƒ����_�Ԃ̋���

	float m_fFront;				// �L�����̑O������

	float m_fHeightR;			// �����_����
	float m_fHeightV;			// ���_����

	D3DXVECTOR3 m_posVDest;		// �ړI�̎��_
	D3DXVECTOR3 m_posRDest;		// �ړI�̒����_

	D3DXVECTOR3 m_vecInterval;	// ���_�A�����_�̃x�N�g��
};

#endif