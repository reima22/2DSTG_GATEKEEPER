//=============================================================================
//
// 3D�L�����̐ݒ� [chara3D.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CHARA3D_H_
#define _CHARA3D_H_

#include "main.h"
#include "scene3D.h"

//==============================================================================
// �L�����N�^�[3D�N���X
//==============================================================================
class CChara3D : public CScene3D
{
public:
	typedef enum
	{
		CHARA3DTYPE_MAIN = 0,	// ���C��
		CHARA3DTYPE_SUB,		// �T�u�p�[�c
		CHARA3DTYPE_MAX
	}CHARA3DTYPE;

	CChara3D(int nPriority = PRIORITY_CHARA);
	~CChara3D();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CChara3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);

private:
	static char *m_pFileName[CHARA3DTYPE_MAX];

	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_rot;				// �p�x

	D3DXVECTOR3 m_move;				// �ړ���
	D3DXVECTOR3 m_rotMove;			// ��]��
	LPD3DXMESH m_pMesh;				// ���b�V���|�C���^
	LPD3DXBUFFER m_pBuffMat;		// �}�e���A���o�b�t�@
	DWORD m_nNumMat;				// �}�e���A����
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 *m_pTexture;	// �e�N�X�`���_�u���|�C���^

	CHARA3DTYPE m_type;				// ���
};
#endif
