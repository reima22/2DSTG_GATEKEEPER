//=============================================================================
//
// ���s���� [exe.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _EXE_H_
#define _EXE_H_

#include "main.h"
#include "scene.h"

// �O���錾
class CMeshfield;	// ���b�V���t�B�[���h�N���X

//==============================================================================
// ���s�N���X
//==============================================================================
class CExe : public CScene
{
public:
	CExe();
	~CExe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ��������
	static CExe *Create(void);

	static CMeshfield *GetMeshField(void) { return m_pMeshField; }

private:
	int m_nNumObject;						// �I�u�W�F�N�g��
	static CMeshfield *m_pMeshField;		// �I���W�i�����b�V��
};

#endif