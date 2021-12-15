//==============================================================================
//
// ���͏����kinput.h�l
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

// �}�N����`
#define REPEAT_CNT	(30)	// ���s�[�g�J�E���g�̏��
#define MOUSE_BOTTON_NUM	(4)	// �}�E�X�{�^����


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
		KEYINFO_OK = 0,			// ����
		KEYINFO_UP,				// ��
		KEYINFO_DOWN,			// ��
		KEYINFO_LEFT,			// ��
		KEYINFO_RIGHT,			// �E
		KEYINFO_AREA_PLUS,		// �ҏW�G���A�̊g��
		KEYINFO_AREA_MINUS,		// �ҏW�G���A�̏k��
		KEYINFO_MESH_UP,		// ���b�V���㏸
		KEYINFO_MESH_DOWN,		// ���b�V�����~
		KEYINFO_MESH_RESET,		// ���_�̍����̃��Z�b�g
		KEYINFO_CAMERA_FRONT,	// �J�����ړ��O�i
		KEYINFO_CAMERA_BACK,	// �J�����ړ����
		KEYINFO_CAMERA_LEFT,	// �J�����ړ���
		KEYINFO_CAMERA_RIGHT,	// �J�����ړ��E
		KEYINFO_JUMP,			// �W�����v
		KEYINFO_TURN_LEFT,		// �J��������
		KEYINFO_TURN_RIGHT,		// �J��������E
		KEYINFO_CAMERA_RESET,	// �J�����̃��Z�b�g
		KEYINFO_CAMERA_UP,		// �J�����㏸
		KEYINFO_CAMERA_DOWN,	// �J�������~
		KEYINFO_ZOOMIN,			// �J�����̃Y�[���C��
		KEYINFO_ZOOMOUT,		// �J�����̃Y�[���A�E�g
		KEYINFO_TEXIDX_PLUS,	// �e�N�X�`���̃C���f�b�N�X�O�i
		KEYINFO_TEXIDX_MINUS,	// �e�N�X�`���̃C���f�b�N�X���
		KEYINFO_TEXCUT,			// �e�N�X�`���̕���
		KEYINFO_WIRE,			// ���C���[�t���[���̔���
		KEYINFO_SAVE_MESHINFO,	// ���b�V�����ۑ�
		KEYINFO_LOAD_MESHINFO,	// ���b�V���ǂݍ���
		KEYINFO_MODECHANGE,		// �ҏW���[�h�̐؂�ւ�
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

// �L�[�{�[�h�N���X
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

	int GetCntRepeat(int nKey) { return m_nCountRepeat[nKey]; }

private:
	BYTE m_aKeyState[KEY_STATE];			// �L�[�v���X���
	BYTE m_aKeyStateTrigger[KEY_STATE];		// �L�[�g���K�[���
	BYTE m_aKeyStateRelease[KEY_STATE];		// �L�[�����[�X���
	int m_nCountRepeat[KEY_STATE];			// ���s�[�g�J�E���g
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
