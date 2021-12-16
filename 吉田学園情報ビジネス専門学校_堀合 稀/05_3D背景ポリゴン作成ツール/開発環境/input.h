//==============================================================================
//
// ���͏����kinput.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define KEY_STATE	(256)	// ���͏��

// ���̓N���X
class CInput
{
public:
	typedef enum 
	{
		KEYINFO_SAVE = 0,				// �f�[�^�̏����o��
		KEYINFO_UP,						// ��
		KEYINFO_DOWN,					// ��
		KEYINFO_LEFT,					// ��
		KEYINFO_RIGHT,					// �E
		KEYINFO_WAVE_FLUG,				// �g�`�t���O
		KEYINFO_WAVE_SYNTHETIC,			// �����؂�ւ�
		KEYINFO_WAVE_CUTTEX,			// �e�N�X�`�������̐؂�ւ�
		KEYINFO_WAVE_TEX_CHANGE,		// ���삷��e�N�X�`���̐؂�ւ�
		KEYINFO_WIREFRAME,				// ���C���[�t���[���Ƃ̐؂�ւ�
		KEYINFO_TURN_LEFT,				// �J��������
		KEYINFO_TURN_RIGHT,				// �J��������E
		KEYINFO_CAMERA_UP,				// �J�����㏸
		KEYINFO_CAMERA_DOWN,			// �J�������~
		KEYINFO_CAMERA_ZOOMIN,			// �Y�[���C��
		KEYINFO_CAMERA_ZOOMOUT,			// �Y�[���A�E�g
		KEYINFO_WAVE_HEIGHTUP,			// �g�`�̍����㏸
		KEYINFO_WAVE_HEIGHTDOWN,		// �g�`�̍������~
		KEYINFO_WAVE_DISTANCEUP,		// �g�`�̊Ԋu����
		KEYINFO_WAVE_DISTANCEDOWN,		// �g�`�̊Ԋu����
		KEYINFO_WAVE_SPEEDUP,			// �T�C���g�̑傫���̑���
		KEYINFO_WAVE_SPEEDDOWN,			// �T�C���g�̑傫���̌���
		KEYINFO_WAVE_TEXMOVEROT_PLUS,	// �e�N�X�`���̗���������]�p+
		KEYINFO_WAVE_TEXMOVEROT_MINUS,	// �e�N�X�`���̗���������]�p-
		KEYINFO_WAVE_TEXMOVE_PLUS,		// �e�N�X�`���̗���鑬��+
		KEYINFO_WAVE_TEXMOVE_MINUS,		// �e�N�X�`���̗���鑬��-
		KEYINFO_WAVE_TEXTURE_IDX_PLUS,	// �e�N�X�`���C���f�b�N�X�̉��Z
		KEYINFO_WAVE_TEXTURE_IDX_MINUS,	// �e�N�X�`���C���f�b�N�X�̌��Z
		KEYINFO_MAX
	}KEYINFO;

	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;	// �������z�֐���

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;		// �f�o�C�X�|�C���^
	static LPDIRECTINPUT8 m_pInput;		// �S�̂ň�ŗǂ�(static:�ÓI�����o�ϐ�)	
	static int m_aKeyInfo[KEYINFO_MAX];	// ���̓L�[���
};

class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

private:
	BYTE m_aKeyState[KEY_STATE];			// �L�[�v���X���
	BYTE m_aKeyStateTrigger[KEY_STATE];		// �L�[�g���K�[���
	BYTE m_aKeyStateRelease[KEY_STATE];		// �L�[�����[�X���
	BYTE m_aKeyStateRepeat[KEY_STATE];		// �L�[���s�[�g���

	int m_nCountRepeat;						// ���s�[�g�J�E���g
};

// �}�E�X�N���X
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		MOUSEINFO_LEFT = 0,		// ���N���b�N
		MOUSEINFO_RIGHT,		// �E�N���b�N
		MOUSEINFO_WHEEL,		// �}�E�X�z�C�[����������
		MOUSEINFO_BBACK,		// �u���E�U�o�b�N�{�^��
		MOUSEINFO_MOVEON,		// �u�i�ށv�{�^��
		MOUSEINFO_MAX
	}MOUSEINFO;

	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);

	bool GetPress(int nKey);

	//bool GetTrigger(int nKey);
	//bool GetRelease(int nKey);

	//bool GetRepeat(int nKey);

	//int GetCntRepeat(int nKey) { return m_nCountRepeat[nKey]; }

	DIMOUSESTATE2 GetMouseState(void) { return m_mouseState; }

	//DIMOUSESTATE2 GetMouseTrigger(void) { return m_mouseStateTrigger; }
	//DIMOUSESTATE2 GetMouseRelease(void) { return m_mouseStateRelease; }

	BYTE GetButton(int nKey) { return m_aButton[nKey]; }

	// �J�[�\���̈ړ���
	int GetMouselX(void) { return m_mouseState.lX; }
	int GetMouselY(void) { return m_mouseState.lY; }
	int GetMouselZ(void) { return m_mouseState.lZ; }	// �z�C�[���̉�]��

private:
	DIMOUSESTATE2 m_mouseState;

	// �{�^���̎��
	BYTE m_aButton[MOUSEINFO_MAX] = {};

	//DIMOUSESTATE2 m_mouseStateTrigger;
	//DIMOUSESTATE2 m_mouseStateRelease;
	//BYTE m_aMouseState[MOUSE_BOTTON_NUM];			// �}�E�X�v���X���
	//BYTE m_aMouseStateTrigger[MOUSE_BOTTON_NUM];	// �}�E�X�g���K�[���
	//BYTE m_aMouseStateRelease[MOUSE_BOTTON_NUM];	// �}�E�X�����[�X���
};

#endif
