//==============================================================================
//
// テキストデータ（プレイヤー）管理〔textdata_player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_player.h"
#include "stdio.h"

// 静的メンバ変数宣言
int CTextDataPlayer::m_aPlayerData[PLAYERDATA_MAX];
float CTextDataPlayer::m_fSpeedData;

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataPlayer::CTextDataPlayer()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataPlayer::~CTextDataPlayer()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataPlayer::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];

	// 変数の初期化
	for (int nCnt = 0; nCnt < PLAYERDATA_MAX; nCnt++)
	{
		m_aPlayerData[nCnt] = 0;
	}

	// 速度
	m_fSpeedData = 0.0f;

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_PLAYER], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		while (strcmp("PLAYERDATA_END", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 項目に対応した変数への格納
			if (strcmp("STOCK", pLoadText) == 0)
			{// 残機数
				fscanf(pFile, "%s %d", &aLoadText[0],&m_aPlayerData[PLAYERDATA_STOCK]);
			}
			else if (strcmp("LIFE_START", pLoadText) == 0)
			{// 開始時のライフ
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aPlayerData[PLAYERDATA_LIFE_START]);
			}
			else if (strcmp("LIFE_MAX", pLoadText) == 0)
			{// ライフの最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aPlayerData[PLAYERDATA_LIFE_MAX]);
			}
			else if (strcmp("SPEED", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData);
			}
			else if (strcmp("BULLET_WAVE_UNLOCK", pLoadText) == 0)
			{// 弾(波形)のアンロック
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aPlayerData[PLAYERDATA_UNLOCK_BULLET_WAVE]);
			}
			else if (strcmp("BULLET_MISSILE_UNLOCK", pLoadText) == 0)
			{// 弾(ミサイル)のアンロック
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aPlayerData[PLAYERDATA_UNLOCK_BULLET_MISSILE]);
			}
			else if (strcmp("BULLET_REVERSE_UNLOCK", pLoadText) == 0)
			{// 弾(逆波形)のアンロック
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aPlayerData[PLAYERDATA_UNLOCK_BULLET_REVERSE]);
			}
		}

		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}