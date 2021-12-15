//=============================================================================
//
// チュートリアルモード処理 [tutorial.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "scene.h"

//==============================================================================
// チュートリアルモードクラス
//==============================================================================
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CTutorial *Create(void);

private:
	
	static bool m_bStart;		// ゲームの開始判定
	static bool m_bAutoFade;	// 自動フェード判定
	int m_nAutoFadeCnt;			// 自動遷移までのカウント
};

#endif
