//=============================================================================
//
// テクスチャ管理 [texture.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

class CTextData;

//==============================================================================
// テクスチャ管理クラス
//==============================================================================
class CTexture
{
public:
	// テクスチャの種類
	//typedef enum
	//{
	//	TEXTYPE_NONE = 0,			// テクスチャ無し
	//	TEXTYPE_PLAYER_0,			// プレイヤー服0
	//	TEXTYPE_PLAYER_1,			// プレイヤー服1
	//	TEXTURE_OBJECT,				// オブジェクト
	//	TEXTYPE_TITLE_BG,			// タイトル背景
	//	TEXTYPE_TITLE_LOGO,			// タイトルロゴ
	//	TEXTYPE_TITLE_ENTER,		// タイトルエンターサイン
	//	TEXTYPE_RESULT_BG,			// リザルト背景
	//	TEXTYPE_RESULT_LOGO,		// リザルトロゴ
	//	TEXTYPE_RANKING_BG,			// ランキング背景
	//	TEXTYPE_RANKING_LOGO,		// ランキングロゴ
	//	TEXTYPE_RANKING_RANK,		// ランキング順位
	//	TEXTYPE_RANKING_SCORELOGO,	// ランキング画面のリザルトスコアロゴ
	//	TEXTYPE_NUMBER,				// ナンバー
	//	TEXTYPE_EXPLOSION,			// 爆発
	//	TEXTYPE_EFFECT,				// エフェクト
	//	TEXTYPE_PAUSE_BASE,			// ポーズ画面のベース
	//	TEXTYPE_PAUSE_CONTINUE,		// ポーズコンテニュー
	//	TEXTYPE_PAUSE_RETRY,		// ポーズリトライ
	//	TEXTYPE_PAUSE_QUIT,			// ポーズ終了
	//	TEXTYPE_TUTORIAL_0,			// チュートリアル0
	//	TEXTYPE_TUTORIAL_1,			// チュートリアル1
	//	TEXTYPE_TUTORIAL_2,			// チュートリアル2
	//	TEXTYPE_WINDOWEFFECT,		// 画面エフェクト
	//	TEXTYPE_EASY,				// easy
	//	TEXTYPE_NORMAL,				// normal
	//	TEXTYPE_HARD,				// hard
	//	TEXTYPE_METEOR,				// 隕石
	//	TEXTYPE_PANEL,				// ソーラーパネル
	//	TEXTYPE_MAX					// 最大数
	//}TEXTYPE;

	CTexture();
	~CTexture();

	HRESULT LoadAll(void);			// 全読み込み
	void UnloadAll(void);			// 全破棄
	static CTexture *Create(void);	// 生成処理

	// テクスチャの割り当て
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

	// 3Dモデルへのテクスチャ割り当て
	LPDIRECT3DTEXTURE9 GetAddress3D(char *pTexFilename);

	// テクスチャの総数
	int GetTexMax(void) { return m_nTexMax; }

	// テクスチャテキストのデータ取得
	CTextData *GetData(void) { return m_pData; }

private:
	CTextData *m_pData;					// データポインタ
	char **m_pFileName;					// 保管用ポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;		// ポインタ配列格納アドレス
	int m_nTexMax;						// テクスチャの最大数
};
#endif
