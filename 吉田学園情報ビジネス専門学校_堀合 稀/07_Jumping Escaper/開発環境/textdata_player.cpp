//==============================================================================
//
// テキストデータ（プレイヤー）管理〔textdata_player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "textdata.h"
#include "textdata_player.h"
#include "stdio.h"

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

	// 数値カウント
	m_nModelCnt = 0;
	m_nPartsCnt = 0;

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_PLAYER], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		// スクリプト読み込み終わりまで
		while (strcmp("END_SCRIPT", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// モデル数
			if (strcmp("NUM_MODEL", pLoadText) == 0)
			{
				fscanf(pFile, "%s %d" , &aLoadText[0],&m_nNumModel);

				// モデル数分、動的確保
				m_pFileModelName = new char*[m_nNumModel];

				m_pFileModelNameLoad = new char*[m_nNumModel];
				
				// 2次元配列の確保
				for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
				{
					m_pFileModelNameLoad[nCnt] = new char[TEXT_NUM];

					// 先頭アドレスの設定
					m_pFileModelName[nCnt] = &m_pFileModelNameLoad[nCnt][0];
				}
			}

			// モデルファイル名
			if (strcmp("MODEL_FILENAME", pLoadText) == 0)
			{			
				// モデル名
				fscanf(pFile, "%s %s", &aLoadText[0], &m_pFileModelNameLoad[m_nModelCnt][0]);

				// カウント増加
				m_nModelCnt++;
			}

			// プレイヤー情報
			if (strcmp("PLAYERSET", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aPlayerText[TEXT_NUM] = {};
				char *pPlayerText = &aPlayerText[0];

				while (strcmp("END_PLAYERSET", pPlayerText) != 0)
				{
					fscanf(pFile, "%s", &aPlayerText[0]);

					// 項目に対応した変数への格納
					if (strcmp("POS", pPlayerText) == 0)
					{// 位置
						fscanf(pFile, "%s %f %f %f", 
							&aPlayerText[0],
							&m_posPlayer.x,
							&m_posPlayer.y,
							&m_posPlayer.z);
					}

					if (strcmp("ROT", pPlayerText) == 0)
					{// 角度
						fscanf(pFile, "%s %f %f %f",
							&aPlayerText[0],
							&m_rotPlayer.x,
							&m_rotPlayer.y,
							&m_rotPlayer.z);
					}

					if (strcmp("SHADOW", pPlayerText) == 0)
					{// 影の大きさ
						fscanf(pFile, "%s %f %f",
							&aPlayerText[0],
							&m_shadowSize.x,
							&m_shadowSize.y);
					}

					if (strcmp("RADIUS", pPlayerText) == 0)
					{// 当たり判定半径
						fscanf(pFile, "%s %f", &aPlayerText[0], &m_fRadius);
					}

					if (strcmp("HEIGHT", pPlayerText) == 0)
					{// 当たり判定高さ
						fscanf(pFile, "%s %f", &aPlayerText[0], &m_fHeight);
					}
				}
			}

			// キャラクター情報読み込み
			if (strcmp("CHARACTERSET", pLoadText) == 0)
			{
				// ローカル変数宣言
				char aCharaText[TEXT_NUM] = {};
				char *pCharaText = &aCharaText[0];

				while (strcmp("END_CHARACTERSET", pCharaText) != 0)
				{
					// 読み込み
					fscanf(pFile, "%s", &aCharaText[0]);

					// 項目に対応した変数への格納
					if (strcmp("NUM_PARTS", pCharaText) == 0)
					{// モデルの数
						fscanf(pFile, "%s %d", &aCharaText[0], &m_nNumParts);

						// モデルパーツ情報ポインタの動的確保
						m_pIdx = new int[m_nNumParts];			// インデックス情報
						m_pParentIdx = new int[m_nNumParts];	// 親情報
						m_pOffset = new float*[m_nNumParts];	// オフセット情報
					}



					// パーツ情報読み込み
					if (strcmp("PARTSSET", pCharaText) == 0)
					{
						// ローカル変数宣言
						char aPartsText[TEXT_NUM] = {};
						char *pPartsText = &aPartsText[0];

						// 2次元の確保
						m_pOffset[m_nPartsCnt] = new float[INFOMAX];

						while (strcmp("END_PARTSSET", pPartsText) != 0)
						{
							// 項目読み込み
							fscanf(pFile, "%s", pPartsText);

							// インデックス情報
							if (strcmp("INDEX", pPartsText) == 0)
							{
								fscanf(pFile, "%s %d", &aPartsText[0], &m_pIdx[m_nPartsCnt]);
							}

							// 親のインデックス
							if (strcmp("PARENT", pPartsText) == 0)
							{
								fscanf(pFile, "%s %d", &aPartsText[0], &m_pParentIdx[m_nPartsCnt]);
							}

							// オフセット位置
							if (strcmp("POS", pPartsText) == 0)
							{
								fscanf(pFile, "%s %f %f %f",
									&aPartsText[0],
									&m_pOffset[m_nPartsCnt][INFO_POS_X],
									&m_pOffset[m_nPartsCnt][INFO_POS_Y],
									&m_pOffset[m_nPartsCnt][INFO_POS_Z]);
							}

							// オフセット角度
							if (strcmp("ROT", pPartsText) == 0)
							{
								fscanf(pFile, "%s %f %f %f",
									&aPartsText[0],
									&m_pOffset[m_nPartsCnt][INFO_ROT_X],
									&m_pOffset[m_nPartsCnt][INFO_ROT_Y],
									&m_pOffset[m_nPartsCnt][INFO_ROT_Z]);
							}
						}

						m_nPartsCnt++;
					}
				}
			}

			// モーション情報の読み込み
			if (strcmp("MOTIONSET", pLoadText) == 0)
			{
				// キーのカウント初期化
				m_nKeyCnt = 0;

				// ローカル変数宣言
				char aMotionText[TEXT_NUM] = {};
				char *pMotionText = &aMotionText[0];

				while (strcmp("END_MOTIONSET", pMotionText) != 0)
				{// 終了までループ
					fscanf(pFile, "%s", &aMotionText[0]);

					// ループの有無
					if (strcmp("LOOP", pMotionText) == 0)
					{
						int nLoop = 0;
						fscanf(pFile, "%s %d", &aMotionText[0], &nLoop);
						
						if (nLoop != 0)
						{
							m_bLoop[m_nMotionCnt] = true;
						}
					}

					// キー数
					if (strcmp("NUM_KEY", pMotionText) == 0)
					{
						// キー数の読み込み
						fscanf(pFile, "%s %d", &aMotionText[0], &m_nNumKey[m_nMotionCnt]);

						// フレーム数の動的確保
						m_pFrame[m_nMotionCnt] = new int[m_nNumKey[m_nMotionCnt]];

						// キーの数だけの動的確保
						m_pKey[m_nMotionCnt] = new float**[m_nNumKey[m_nMotionCnt]];

						for (int nCntModel = 0; nCntModel < m_nNumKey[m_nMotionCnt]; nCntModel++)
						{
							// パーツ数だけ確保
							m_pKey[m_nMotionCnt][nCntModel] = new float*[m_nNumParts];

							for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
							{
								// キー要素数だけ確保
								m_pKey[m_nMotionCnt][nCntModel][nCnt] = new float[CMotionPlayer::KEY_MAX];
							}
						}				
					}

					// キー情報
					if (strcmp("KEYSET", pMotionText) == 0)
					{
						// モデルカウント初期化
						m_nPartsCnt = 0;

						// ローカル変数宣言
						char aKeyText[TEXT_NUM] = {};
						char *pKeyText = &aKeyText[0];

						while (strcmp("END_KEYSET", pKeyText) != 0)
						{
							// キー数の読み込み
							fscanf(pFile, "%s", &aKeyText[0]);

							// フレーム数の読み込み
							if (strcmp("FRAME", pKeyText) == 0)
							{
								fscanf(pFile, "%s %d", &aKeyText[0], &m_pFrame[m_nMotionCnt][m_nKeyCnt]);
							}

							// キー要素の読み込み
							if (strcmp("KEY", pKeyText) == 0)
							{
								// ローカル変数宣言
								char aKeyInfoText[TEXT_NUM] = {};
								char *pKeyInfoText = &aKeyInfoText[0];

								while (strcmp("END_KEY", pKeyInfoText) != 0)
								{
									fscanf(pFile, "%s", &aKeyInfoText[0]);

									// コメント内の例外
									if (strcmp(":",pKeyInfoText) == 0)
									{
										m_nPartsCnt--;
										break;
									}

									// 位置
									if (strcmp("POS", pKeyInfoText) == 0)
									{
										fscanf(pFile, "%s %f %f %f",
											&aKeyInfoText[0],
											&m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_POS_X],
											&m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_POS_Y],
											&m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_POS_Z]);

										m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_POS_X] += m_pOffset[m_nPartsCnt][CMotionPlayer::KEY_POS_X];
										m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_POS_Y] += m_pOffset[m_nPartsCnt][CMotionPlayer::KEY_POS_Y];
										m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_POS_Z] += m_pOffset[m_nPartsCnt][CMotionPlayer::KEY_POS_Z];
									}												 

									// 角度
									if (strcmp("ROT", pKeyInfoText) == 0)
									{
										fscanf(pFile, "%s %f %f %f",
											&aKeyInfoText[0],
											&m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_ROT_X],
											&m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_ROT_Y],
											&m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_ROT_Z]);

										m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_ROT_X] += m_pOffset[m_nPartsCnt][CMotionPlayer::KEY_ROT_X];
										m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_ROT_Y] += m_pOffset[m_nPartsCnt][CMotionPlayer::KEY_ROT_Y];
										m_pKey[m_nMotionCnt][m_nKeyCnt][m_nPartsCnt][CMotionPlayer::KEY_ROT_Z] += m_pOffset[m_nPartsCnt][CMotionPlayer::KEY_ROT_Z];
									}
								}

								// モデルのカウント
								m_nPartsCnt++;
							}
						}

						// キー数のカウント
						m_nKeyCnt++;
					}
				}

				// モーション数のカウント
				m_nMotionCnt++;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
		return;
	}
}

//==============================================================================
// データの破棄
//==============================================================================
void CTextDataPlayer::Unload(void)
{
	// テキストデータの破棄
	if (m_pFileModelName != NULL)
	{
		delete[] m_pFileModelName;
		m_pFileModelName = NULL;
	}

	if (m_pFileModelNameLoad != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			delete[] m_pFileModelNameLoad[nCnt];
		}

		delete[] m_pFileModelNameLoad;
		m_pFileModelNameLoad = NULL;
	}

	// インデックスの破棄
	if (m_pIdx != NULL)
	{
		delete[] m_pIdx;
		m_pIdx = NULL;
	}

	// 親インデックスの破棄
	if (m_pParentIdx != NULL)
	{
		delete[] m_pParentIdx;
		m_pParentIdx = NULL;
	}

	// オフセットの破棄
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pOffset != NULL)
		{
			delete[] m_pOffset[nCnt];
			m_pOffset = NULL;
		}
	}

	// キー情報の破棄
	for (int nCntMotion = 0; nCntMotion < CMotionPlayer::MOTIONTYPE_MAX; nCntMotion++)
	{
		if (m_pKey[nCntMotion] != NULL)
		{
			for (int nCntKey = 0; nCntKey < m_nNumKey[nCntMotion]; nCntKey++)
			{
				for (int nCntParts = 0; nCntParts < CMotionPlayer::KEY_MAX; nCntParts++)
				{
					delete[] m_pKey[nCntMotion][nCntKey][nCntParts];
				}

				delete[] m_pKey[nCntMotion][nCntKey];
			}

			delete[] m_pKey[nCntMotion];
			m_pKey[nCntMotion] = NULL;
		}
	}
}

//==============================================================================
// プレイヤーデータの生成
//==============================================================================
CTextDataPlayer *CTextDataPlayer::Create(void)
{
	// プレイヤーデータの読み込み
	CTextDataPlayer *pDataPlayer;
	pDataPlayer = new CTextDataPlayer;

	// データのロード
	if (pDataPlayer != NULL)
	{
		pDataPlayer->LoadData();
	}

	return pDataPlayer;
}