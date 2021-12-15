//=============================================================================
//
// �e�L�X�g�f�[�^�i���b�V���t�B�[���h�j�Ǘ� [textdata_meshfield.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_MESHFIELD_H_
#define _TEXTDATA_MESHFIELD_H_

#include "main.h"
#include "textdata.h"
#include "meshfield.h"

//==============================================================================
// �e�L�X�g�Ǘ��i���b�V���t�B�[���h�j�N���X
//==============================================================================
class CTextDataMeshfield : public CTextData
{
public:
	CTextDataMeshfield();
	~CTextDataMeshfield();

	void LoadData(void);
	void Unload(void);

	void SaveData(CMeshfield *pMesh);

	static CTextDataMeshfield *Create(void);	// ����


	// ���擾
	int GetTexType(void) { return m_nType; }								// �e�N�X�`���̎��
	D3DXVECTOR3 GetPosition(void) { return m_pos; }							// �ʒu
	D3DXVECTOR3 GetRotation(void) { return m_rot; }							// �p�x
	D3DXVECTOR2 GetSize(void) { return m_size; }							// �T�C�Y
	int GetWidthPoly(void) { return m_nWidth; }								// �ʂ̐�
	int GetDepthPoly(void) { return m_nDepth; }								// �ʂ̐�
	D3DXVECTOR3 GetPosVtx(int nIdxPoint) { return m_pPosVtx[nIdxPoint]; }	// ���b�V�����_���

private:
	int m_nNumFile;			// �t�@�C�����̑���
	int m_nType;			// ���
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// �p�x
	D3DXVECTOR2 m_size;		// 1�ʓ�����̃T�C�Y
	int m_nWidth;			// �����̃|���S������
	int m_nDepth;			// ���s�̃|���S������
	int m_nIdxPoint;		// �����_��
	D3DXVECTOR3 *m_pPosVtx;	// ���_���W
};
#endif