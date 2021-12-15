//==============================================================================
//
// ���̕`��ksphere.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "main.h"
#include "player.h"

// ���b�V���t�B�[���h�N���X
class CSphere : public CPlayer
{
public:
	CSphere();
	~CSphere();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSphere *Create(void);

	// �e�p�����[�^�̐ݒ�
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetSlice(int nSlice) { m_nSlice = nSlice; }
	void SetStack(int nStack) { m_nStack = nStack; }

private:
	LPD3DXMESH m_pMeshSphere = NULL;			// ���̗p���b�V��
	LPD3DXBUFFER m_pBuffMatSphere = NULL;		// ���̃}�e���A���o�b�t�@
	D3DXMATRIX m_sphereMtxWorld;						// ���[���h�}�g���b�N�X

	float m_fRadius;							// ���̔��a
	int m_nSlice;								// ���̂̃X���C�X
	int m_nStack;								// ���̂̃X�^�b�N
	
	//D3DXVECTOR3 m_posSphire;					// ���̈ʒu
};

#endif