//==============================================================================
//
// プレイヤーモーション管理〔motion_player.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "model.h"
#include "motion.h"
#include "motion_player.h"
#include "textdata_player.h"

//==============================================================================
// コンストラクタ
//==============================================================================
CMotionPlayer::CMotionPlayer()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CMotionPlayer::~CMotionPlayer()
{

}

//==============================================================================
// プレイヤーモーションの初期化
//==============================================================================
HRESULT CMotionPlayer::Init(void)
{
	// テキストデータの取得
	CTextDataPlayer *pDataPlayer = CTextDataPlayer::GetDataPlayer();

	// パーツ数
	m_nNumParts = pDataPlayer->GetPartsNum();

	// モデルの動的確保
	m_apModel = new CModel*[m_nNumParts];
	m_aIndexParent = new int[m_nNumParts];
	m_aOffset = new float*[m_nNumParts];
	m_posMemo = new D3DXVECTOR3[m_nNumParts];
	m_rotMemo = new D3DXVECTOR3[m_nNumParts];

	// モーション情報の動的確保
	m_aKey = new float***[MOTIONTYPE_MAX];	// キー情報
	m_nNumKey = new int[MOTIONTYPE_MAX];	// キーの数
	m_pFrame = new int*[MOTIONTYPE_MAX];	// フレーム数

	// キーカウント数
	m_nKeyCnt = 0;

	// パーツごとの設定
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		// 親のインデックス情報の取得
		m_aIndexParent[nCnt] = pDataPlayer->GetParent(nCnt);

		// 各パラメータ分確保
		m_aOffset[nCnt] = new float[CTextDataPlayer::INFOMAX];

		for (int nCntOffset = 0; nCntOffset < CTextDataPlayer::INFOMAX; nCntOffset++)
		{
			// オフセット情報の取得
			m_aOffset[nCnt][nCntOffset] = pDataPlayer->GetOffset(nCnt, nCntOffset);
		}

		// Xファイル名取得
		char *pFileName = pDataPlayer->GetFileName(nCnt);

		// パーツごとのオフセット位置
		D3DXVECTOR3 pos = D3DXVECTOR3(
			m_aOffset[nCnt][CTextDataPlayer::INFO_POS_X],
			m_aOffset[nCnt][CTextDataPlayer::INFO_POS_Y],
			m_aOffset[nCnt][CTextDataPlayer::INFO_POS_Z]);

		// パーツごとのオフセット角度
		D3DXVECTOR3 rot = D3DXVECTOR3(
			m_aOffset[nCnt][CTextDataPlayer::INFO_ROT_X],
			m_aOffset[nCnt][CTextDataPlayer::INFO_ROT_Y],
			m_aOffset[nCnt][CTextDataPlayer::INFO_ROT_Z]);

		// モデルの生成
		m_apModel[nCnt] = CModel::Create(pFileName, pos, rot);

		if (nCnt != 0)
		{
			m_apModel[nCnt]->SetParent(m_apModel[m_aIndexParent[nCnt]]);
		}
	}

	// キー数ごと
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		m_nNumKey[nCntMotion] = pDataPlayer->GetNumKey((MOTIONTYPE)nCntMotion);

		// フレーム数のモーション毎の動的確保
		m_pFrame[nCntMotion] = new int[m_nNumKey[nCntMotion]];

		// キーフレーム情報のモーションごとの確保
		m_aKey[nCntMotion] = new float**[m_nNumKey[nCntMotion]];

		for (int nCntKey = 0; nCntKey < m_nNumKey[nCntMotion]; nCntKey++)
		{
			// フレーム数の取得
			m_pFrame[nCntMotion][nCntKey] = pDataPlayer->GetFrame((MOTIONTYPE)nCntMotion, nCntKey);

			// キーフレーム情報のキー数ごとの確保
			m_aKey[nCntMotion][nCntKey] = new float*[m_nNumParts];

			// パーツ数ごと
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
			{
				// キーフレーム情報のパーツごとの確保
				m_aKey[nCntMotion][nCntKey][nCntParts] = new float[KEY_MAX];

				// キー情報
				m_aKey[nCntMotion][nCntKey][nCntParts][KEY_POS_X] = pDataPlayer->GetKeyInfo((MOTIONTYPE)nCntMotion, nCntKey, nCntParts, KEY_POS_X);
				m_aKey[nCntMotion][nCntKey][nCntParts][KEY_POS_Y] = pDataPlayer->GetKeyInfo((MOTIONTYPE)nCntMotion, nCntKey, nCntParts, KEY_POS_Y);
				m_aKey[nCntMotion][nCntKey][nCntParts][KEY_POS_Z] = pDataPlayer->GetKeyInfo((MOTIONTYPE)nCntMotion, nCntKey, nCntParts, KEY_POS_Z);
				m_aKey[nCntMotion][nCntKey][nCntParts][KEY_ROT_X] = pDataPlayer->GetKeyInfo((MOTIONTYPE)nCntMotion, nCntKey, nCntParts, KEY_ROT_X);
				m_aKey[nCntMotion][nCntKey][nCntParts][KEY_ROT_Y] = pDataPlayer->GetKeyInfo((MOTIONTYPE)nCntMotion, nCntKey, nCntParts, KEY_ROT_Y);
				m_aKey[nCntMotion][nCntKey][nCntParts][KEY_ROT_Z] = pDataPlayer->GetKeyInfo((MOTIONTYPE)nCntMotion, nCntKey, nCntParts, KEY_ROT_Z);
			}
		}
	}

	return S_OK;
}

//==============================================================================
// プレイヤーモーションの終了
//==============================================================================
void CMotionPlayer::Uninit(void)
{
	// モデル情報の破棄
	if (m_apModel != NULL)
	{
		delete m_apModel;
		m_apModel = NULL;
	}

	// 親情報の破棄
	if (m_aIndexParent != NULL)
	{
		delete m_aIndexParent;
		m_aIndexParent = NULL;
	}

	// 過去の位置情報の破棄
	if (m_posMemo != NULL)
	{
		delete m_posMemo;
		m_posMemo = NULL;
	}

	// 過去の向き情報情報の破棄
	if (m_rotMemo != NULL)
	{
		delete m_rotMemo;
		m_rotMemo = NULL;
	}

	// フレームの破棄
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		if (m_pFrame[nCntMotion] != NULL)
		{
			delete m_pFrame[nCntMotion];
			m_pFrame[nCntMotion] = NULL;
		}
	}

	// オフセット情報の破棄
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_aOffset[nCnt] != NULL)
		{
			delete[] m_aOffset[nCnt];
		}
	}
	if (m_aOffset != NULL)
	{
		delete m_aOffset;
		m_aOffset = NULL;
	}

	// キー情報の破棄
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		if (m_aKey[nCntMotion] != NULL)
		{
			for (int nCntKey = 0; nCntKey < m_nNumKey[nCntMotion]; nCntKey++)
			{
				for (int nCntParts = 0; nCntParts < KEY_MAX; nCntParts++)
				{
					delete[] m_aKey[nCntMotion][nCntKey][nCntParts];
				}

				delete[] m_aKey[nCntMotion][nCntKey];
			}

			delete[] m_aKey[nCntMotion];
			m_aKey[nCntMotion] = NULL;
		}
	}
}

//==============================================================================
// プレイヤーモーションの更新
//==============================================================================
void CMotionPlayer::Update(MOTIONTYPE motionType)
{
	// テキストデータの取得
	CTextDataPlayer *pDataPlayer = CTextDataPlayer::GetDataPlayer();

	// ブレンド用フレーム数
	int nBlendFrame = 10;

	// モーション変化によるカウントの初期化
	if (m_motionTypeOld != motionType)
	{
		m_nKeyCnt = 0;
		m_nFrameCnt = 0;
		m_bBlendMotion = true;
	}

	// ループ情報
	m_bLoop = pDataPlayer->GetLoop(motionType);

	// フレームのカウント
	if (m_nFrameCnt < m_pFrame[motionType][m_nKeyCnt] || motionType != MOTIONTYPE_JUMP)
	{
		m_nFrameCnt++;
	}

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		// 位置・角度の取得
		D3DXVECTOR3 pos = m_apModel[nCnt]->GetPosition();
		D3DXVECTOR3 rot = m_apModel[nCnt]->GetRotation();

		if (m_bLoop == true && m_nKeyCnt == m_nNumKey[motionType] - 1)
		{// ループするときの折り返し
			pos.x = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_X] + (m_aKey[motionType][0][nCnt][KEY_POS_X] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_X]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			pos.y = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Y] + (m_aKey[motionType][0][nCnt][KEY_POS_Y] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Y]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			pos.z = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Z] + (m_aKey[motionType][0][nCnt][KEY_POS_Z] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Z]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			rot.x = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_X] + (m_aKey[motionType][0][nCnt][KEY_ROT_X] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_X]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			rot.y = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Y] + (m_aKey[motionType][0][nCnt][KEY_ROT_Y] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Y]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			rot.z = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Z] + (m_aKey[motionType][0][nCnt][KEY_ROT_Z] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Z]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			m_posMemo[nCnt] = pos;
			m_rotMemo[nCnt] = rot;
		}
		else if (m_bLoop == false && m_nKeyCnt == m_nNumKey[motionType] - 1)
		{// ループせず終了する時
			if (motionType == MOTIONTYPE_LANDING)
			{// ニュートラルへ
				pos.x = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_X] + (m_aKey[MOTIONTYPE_NEUTRAL][0][nCnt][KEY_POS_X] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_X]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
				pos.y = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Y] + (m_aKey[MOTIONTYPE_NEUTRAL][0][nCnt][KEY_POS_Y] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Y]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
				pos.z = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Z] + (m_aKey[MOTIONTYPE_NEUTRAL][0][nCnt][KEY_POS_Z] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Z]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
				rot.x = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_X] + (m_aKey[MOTIONTYPE_NEUTRAL][0][nCnt][KEY_ROT_X] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_X]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
				rot.y = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Y] + (m_aKey[MOTIONTYPE_NEUTRAL][0][nCnt][KEY_ROT_Y] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Y]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
				rot.z = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Z] + (m_aKey[MOTIONTYPE_NEUTRAL][0][nCnt][KEY_ROT_Z] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Z]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
				m_posMemo[nCnt] = pos;
				m_rotMemo[nCnt] = rot;
			}
		}
		else if (m_bBlendMotion == true)
		{// モーションが途中で切り替わる(モーションブレンド)
			pos.x = m_posMemo[nCnt].x + (m_aKey[motionType][0][nCnt][KEY_POS_X] - m_posMemo[nCnt].x) * ((float)m_nFrameCnt / (float)nBlendFrame);
			pos.y = m_posMemo[nCnt].y + (m_aKey[motionType][0][nCnt][KEY_POS_Y] - m_posMemo[nCnt].y) * ((float)m_nFrameCnt / (float)nBlendFrame);
			pos.z = m_posMemo[nCnt].z + (m_aKey[motionType][0][nCnt][KEY_POS_Z] - m_posMemo[nCnt].z) * ((float)m_nFrameCnt / (float)nBlendFrame);
			rot.x = m_rotMemo[nCnt].x + (m_aKey[motionType][0][nCnt][KEY_ROT_X] - m_rotMemo[nCnt].x) * ((float)m_nFrameCnt / (float)nBlendFrame);
			rot.y = m_rotMemo[nCnt].y + (m_aKey[motionType][0][nCnt][KEY_ROT_Y] - m_rotMemo[nCnt].y) * ((float)m_nFrameCnt / (float)nBlendFrame);
			rot.z = m_rotMemo[nCnt].z + (m_aKey[motionType][0][nCnt][KEY_ROT_Z] - m_rotMemo[nCnt].z) * ((float)m_nFrameCnt / (float)nBlendFrame);
		}
		else
		{// 通常時の再生
			pos.x = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_X] + (m_aKey[motionType][m_nKeyCnt + 1][nCnt][KEY_POS_X] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_X]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			pos.y = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Y] + (m_aKey[motionType][m_nKeyCnt + 1][nCnt][KEY_POS_Y] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Y]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			pos.z = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Z] + (m_aKey[motionType][m_nKeyCnt + 1][nCnt][KEY_POS_Z] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_POS_Z]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			rot.x = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_X] + (m_aKey[motionType][m_nKeyCnt + 1][nCnt][KEY_ROT_X] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_X]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			rot.y = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Y] + (m_aKey[motionType][m_nKeyCnt + 1][nCnt][KEY_ROT_Y] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Y]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			rot.z = m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Z] + (m_aKey[motionType][m_nKeyCnt + 1][nCnt][KEY_ROT_Z] - m_aKey[motionType][m_nKeyCnt][nCnt][KEY_ROT_Z]) * ((float)m_nFrameCnt / (float)m_pFrame[motionType][m_nKeyCnt]);
			m_posMemo[nCnt] = pos;
			m_rotMemo[nCnt] = rot;
		}

		// 位置・角度の反映
		m_apModel[nCnt]->SetPosition(pos);
		m_apModel[nCnt]->SetRotation(rot);
	}

	// モーションの変わり目
	if (m_bBlendMotion == false)
	{
		if (m_nFrameCnt >= m_pFrame[motionType][m_nKeyCnt] && motionType != MOTIONTYPE_JUMP)
		{
			m_nFrameCnt = 0;

			if (m_nKeyCnt < m_nNumKey[motionType] - 1)
			{// カウントの加算
				m_nKeyCnt++;
			}
			else
			{// カウントのリセット
				if (m_bLoop == false)
				{
					if (motionType == MOTIONTYPE_LANDING)
					{// アクション、着地の次
						m_motionType = MOTIONTYPE_NEUTRAL;
					}
				}
				else
				{
					m_nKeyCnt = 0;
				}
			}
		}
	}
	else
	{// ブレンド時
		if (m_nFrameCnt >= nBlendFrame)
		{
			m_nFrameCnt = 0;
			m_nKeyCnt = 0;

			// ブレンドの終了
			m_bBlendMotion = false;
		}
	}

	// 旧モーションの保存
	m_motionTypeOld = motionType;
}

//==============================================================================
// 描画
//==============================================================================
void CMotionPlayer::Draw(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			// モデルの描画
			m_apModel[nCntModel]->Draw();
		}
	}
}

//==============================================================================
// プレイヤーモーションの生成
//==============================================================================
CMotionPlayer *CMotionPlayer::Create(void)
{
	// ローカル変数宣言
	CMotionPlayer *pMotionPlayer;
	pMotionPlayer = new CMotionPlayer;

	// 初期化
	if (pMotionPlayer != NULL)
	{
		pMotionPlayer->Init();
	}

	return pMotionPlayer;
}