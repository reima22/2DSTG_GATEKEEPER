//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	CSound();
	~CSound();
	//サウンドファイル
	typedef enum
	{
		SOUND_LABEL_BGM000,				// BGM0
		SOUND_LABEL_BGM001,				// BGM1
		SOUND_LABEL_BGM002,				// BGM2
		SOUND_LABEL_BGM003,				// BGM3
		SOUND_LABEL_BGM004,				// BGM4
		SOUND_LABEL_BGM005,				// BGM5
		SOUND_LABEL_BGM006,				// BGM6
		SOUND_LABEL_SE_SHOT000,			// 弾発射音(敵機)
		SOUND_LABEL_SE_SHOT001,			// 弾発射音(弾)
		SOUND_LABEL_SE_SHOT002,			// 弾発射音(ウェーブ)
		SOUND_LABEL_SE_SHOT003,			// 弾発射音(ミサイル)
		SOUND_LABEL_SE_HIT000,			// ヒット音
		SOUND_LABEL_SE_HIT001,			// ヒット音
		SOUND_LABEL_SE_EXPLOSION000,	// 爆発音0
		SOUND_LABEL_SE_EXPLOSION001,	// 爆発音1
		SOUND_LABEL_SE_EXPLOSION002,	// 爆発音2
		SOUND_LABEL_SE_EXPLOSION003,	// 爆発音3
		SOUND_LABEL_SE_START000,		// 決定
		SOUND_LABEL_SE_ITEM000,			// アイテム取得
		SOUND_LABEL_SE_ITEM001,			// 武装切り替え
		SOUND_LABEL_SE_WARNING,			// 警告
		SOUND_LABEL_SE_LINEOVER,		// ライン越え
		SOUND_LABEL_SE_LINE_CAUTION,	// ライン警告
		SOUND_LABEL_SE_MOTHER_CREATE,	// 母艦敵生成音
		SOUND_LABEL_SE_COUNT,			// カウント音
		SOUND_LABEL_MAX
	}SOUND_LABEL;

	// BGMの種類
	typedef enum
	{
		BGMTYPE_TITLE = 0,	// タイトル
		BGMTYPE_TUTORIAL,	// チュートリアル
		BGMTYPE_GAME,		// ゲーム
		BGMTYPE_BOSS,		// ボス戦
		BGMTYPE_RESULT,		// リザルト
		BGMTYPE_RANKING,	// ランキング
		BGMTYPE_MAX
	}BGMTYPE;

	static HRESULT Init(HWND hWnd);
	static void Uninit(void);
	static HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	static void Stop(void);

private:
	// パラメータ構造体
	typedef struct
	{
		char *pFilename;
		int nCntLoop;
	}PARAM;

	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2 *m_pXAudio2;

	static PARAM m_aParam[SOUND_LABEL_MAX];

	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ
};
#endif
