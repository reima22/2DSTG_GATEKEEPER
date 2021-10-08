//=============================================================================
//
// ���b�Z�[�W�`�揈�� [message.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MASSAGE_H_
#define _MASSAGE_H_

#include "main.h"
#include "scene.h"

// �}�N����`
#define MESSAGE_SIZEX	(400.0f)	// ��
#define MESSAGE_SIZEY	(120.0f)	// ����

// �O���錾
class CPolygon;

//==============================================================================
// ���b�Z�[�W�N���X
//==============================================================================
class CMessage
{
public:
	// �J�n�����b�Z�[�W�̒i�K
	typedef enum
	{
		MESSAGE_START_START = 0,	// �J�n
		MESSAGE_START_IN,			// �t�F�[�h�C���ŕ\��
		MESSAGE_START_ON,			// �\�����Ă���
		MESSAGE_START_OUT,			// �t�F�[�h�A�E�g�ŏ���
		MESSAGE_START_END,			// �I��
		MESSAGE_START_MAX
	}MESSAGE_START;

	CMessage();
	~CMessage();
	HRESULT Init(D3DXVECTOR3 pos);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	static CMessage *Create(D3DXVECTOR3 pos);

	static void ChangeOnEnd(void) { m_bStartMessageEnd = true; }
	static bool GetEnd(void) { return m_bStartMessageEnd; }

protected:
	static int m_nStartCnt;			// �Q�[���J�n�����o�J�E���g
	static bool m_bStartMessageEnd;	// ���b�Z�[�W�̏I��
};
#endif
