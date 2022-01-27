//--------------------------------------------------------------------------------
//
// ゲーム制作[title.h]
// Author:Tatsuki Matsuda
//
//--------------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "bg.h"

//--------------------------------------------------------------------------------
//前方宣言
//--------------------------------------------------------------------------------
class CBg;
class CTitleLogo;
class CTitleEnter;
class CTitleRunningMan;

//================================================================================
//オブジェクトクラス
//================================================================================
class CTitle : CScene
{
public:
	CTitle();						//コンストラクタ
	~CTitle();						//デストラクタ

	HRESULT Init(void);				//初期化処理
	void Uninit(void);				//終了処理
	void Update(void);				//更新処理
	void Draw(void);				//描画処理

	static CTitle *Create(void);	// 生成処理

	CBg *GetBg(void);				//背景情報の呼び出し

private:
	static CBg *m_pBg;				//背景情報のポインタ

	static CTitleLogo *m_pTitleLogo;				// タイトルロゴポインタ
	static CTitleEnter *m_pTitleEnter;				// エンターサインポインタ
	static CTitleRunningMan *m_pTitleRunningMan;	// ランニングマンタイトル
};

#endif


