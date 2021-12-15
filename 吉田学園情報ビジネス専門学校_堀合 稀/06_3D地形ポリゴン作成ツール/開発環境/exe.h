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
class CPlayer;
class CSphere;

//==============================================================================
// ���s�N���X
//==============================================================================
class CExe : public CScene
{
public:
	typedef enum
	{
		EDITMODE_EDIT = 0,	// �ҏW
		EDITMODE_PREVIEW,	// �v���r���[
		EDITMODE_MAX
	}EDITMODE;

	CExe();
	~CExe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ��������
	static CExe *Create(void);

	static CMeshfield *GetMeshField(void) { return m_pMeshField; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }

	static EDITMODE GetEditMode(void) { return m_editMode; }	// ���[�h�̎擾
	static void ChangeEditMode(void);					// �ҏW���[�h�̐؂�ւ�

	char *GetModeChar(void) { return m_apMode[m_editMode]; }

private:
	int m_nNumObject;						// �I�u�W�F�N�g��
	static CMeshfield *m_pMeshField;		// �I���W�i�����b�V��
	static CPlayer *m_pPlayer;				// �v���C���[�̃|�C���^
	static CSphere *m_pSphere;				// ���̃|�C���^
	static EDITMODE m_editMode;				// �ҏW���[�h

	char *m_apMode[EDITMODE_MAX] =
	{
		{"�ҏW���[�h"},
		{"�v���r���["}
	};
};

#endif