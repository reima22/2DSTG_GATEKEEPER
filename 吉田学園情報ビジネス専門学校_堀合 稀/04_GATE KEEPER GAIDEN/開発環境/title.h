
//=============================================================================
//
// タイトルモード処理 [title.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "scene.h"

// マクロ定義
#define AUTO_FADE_CNT_R	(300)	// ランキングへの遷移カウント数

//==============================================================================
// タイトルモードクラス
//==============================================================================
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CTitle *Create(void);

	static bool GetDifficultStart(void) { return m_bDifficult; }
	static void SetDifficultStart(bool bSet) { m_bDifficult = bSet; }

	static bool GetStart(void) { return m_bStart; }
	static void SetStart(bool bSet) { m_bStart = bSet; }

	static bool GetAutoFade(void) { return m_bAutoFade; }

private:
	static bool m_bDifficult;	// 難易度選択開始判定
	static bool m_bStart;		// ゲームの開始判定
	static bool m_bAutoFade;	// 自動フェード判定
	int m_nAutoFadeCnt;			// 自動遷移までのカウント
};

#endif
