//=============================================================================
//
// ���b�Z�[�W�e�L�X�g�`�揈�� [message_text.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MASSAGE_TEXT_H_
#define _MASSAGE_TEXT_H_

#include "main.h"
#include "scene.h"
#include "message.h"

// �O���錾
class CPolygon;

//==============================================================================
// ���b�Z�[�W�e�L�X�g�N���X
//==============================================================================
class CMessageText : public CScene2D
{
public:
	CMessageText(int nPriority = CScene::PRIORITY_MESSAGE);
	~CMessageText();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMessageText *Create(D3DXVECTOR3 pos);
	void SetState(void);
	static CMessage::MESSAGE_START GetState(void) { return m_StartState; }

private:
	int m_nStartCnt;
	static CMessage::MESSAGE_START m_StartState;	// �J�n�����b�Z�[�W�̏��
	D3DXCOLOR m_col;
};
#endif