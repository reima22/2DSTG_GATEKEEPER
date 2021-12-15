//=============================================================================
//
// 実行処理 [exe.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _EXE_H_
#define _EXE_H_

#include "main.h"
#include "scene.h"

// 前方宣言
class CMeshfield;	// メッシュフィールドクラス
class CPlayer;
class CSphere;

//==============================================================================
// 実行クラス
//==============================================================================
class CExe : public CScene
{
public:
	typedef enum
	{
		EDITMODE_EDIT = 0,	// 編集
		EDITMODE_PREVIEW,	// プレビュー
		EDITMODE_MAX
	}EDITMODE;

	CExe();
	~CExe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CExe *Create(void);

	static CMeshfield *GetMeshField(void) { return m_pMeshField; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }

	static EDITMODE GetEditMode(void) { return m_editMode; }	// モードの取得
	static void ChangeEditMode(void);					// 編集モードの切り替え

	char *GetModeChar(void) { return m_apMode[m_editMode]; }

private:
	int m_nNumObject;						// オブジェクト数
	static CMeshfield *m_pMeshField;		// オリジナルメッシュ
	static CPlayer *m_pPlayer;				// プレイヤーのポインタ
	static CSphere *m_pSphere;				// 球体ポインタ
	static EDITMODE m_editMode;				// 編集モード

	char *m_apMode[EDITMODE_MAX] =
	{
		{"編集モード"},
		{"プレビュー"}
	};
};

#endif