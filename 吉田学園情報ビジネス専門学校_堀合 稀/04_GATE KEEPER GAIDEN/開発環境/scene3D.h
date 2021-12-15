//=============================================================================
//
// 3D���f���̐ݒ� [scene3D.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"


//==============================================================================
// Scene3D�N���X
//==============================================================================
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority = PRIORITY_DEFAULT);
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3D *Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

private:
	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXVECTOR3 m_rot;								// ����
	D3DXVECTOR3 m_size;								// �傫��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// �o�b�t�@�ւ̃|�C���^

	LPD3DXMESH m_pMesh;				// ���b�V���|�C���^
	LPD3DXBUFFER m_pBuffMat;		// �}�e���A���o�b�t�@
	DWORD m_nNumMat;				// �}�e���A����
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X

	LPDIRECT3DTEXTURE9 *m_pTexture;	// �e�N�X�`���_�u���|�C���^
};
#endif