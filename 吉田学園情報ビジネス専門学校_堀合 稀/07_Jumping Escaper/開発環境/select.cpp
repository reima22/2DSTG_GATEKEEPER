#define _CRT_SECURE_NO_WARNINGS
//=============================================================================
//
// ステージセレクト [select.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "select.h"
#include "Manager.h"
#include "Input.h"
#include "gamepad.h"
#include "fade.h"
#include "bg.h"
#include "sound.h"

// 静的メンバ変数宣言
CStageSelect::STAGE_SELECT CStageSelect::m_currentStage = STAGE_SELECT_1;

//*****************************************************************************
// ステージセレクトクラス ( 継承元 [CScene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CStageSelect::CStageSelect()
{
	m_currentStage = STAGE_SELECT_1;
	m_bSelected = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CStageSelect::~CStageSelect()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStageSelect::Init(void)
{
	// 背景オブジェクト生成
	m_pBg = CBg::Create();

	//m_apScene[SELECT_IDX_BG] = CScene2D::Create(SELECT_BG_POSITION, VECTOR3_ZERO, SELECT_BG_SIZE);
	//m_apScene[SELECT_IDX_BG]->BindTexture(CManager::GetResourceManager()->m_Texture_Map["TexTest01"]);
	//m_apScene[SELECT_IDX_BG]->SetTextureUV(
	//	D3DXVECTOR2(0.0f, SCREEN_HEIGHT / m_apScene[SELECT_IDX_BG]->GetSize().y),
	//	D3DXVECTOR2(0.0f, 0.0f),
	//	D3DXVECTOR2(SCREEN_WIDTH / m_apScene[SELECT_IDX_BG]->GetSize().x, SCREEN_HEIGHT / m_apScene[SELECT_IDX_BG]->GetSize().y),
	//	D3DXVECTOR2(SCREEN_WIDTH / m_apScene[SELECT_IDX_BG]->GetSize().x, 0.0f));
	//m_apScene[SELECT_IDX_BG]->SetPriority(0);

	// テキスト板
	m_apScene[SELECT_IDX_TEXT] = CScene2D::Create();
	m_apScene[SELECT_IDX_TEXT]->SetPosition(SELECT_TEXT_POSITION);
	m_apScene[SELECT_IDX_TEXT]->SetSize(SELECT_TEXT_SIZE);
	m_apScene[SELECT_IDX_TEXT]->SetTex(1, 1, 0, 0, 0, 0);
	m_apScene[SELECT_IDX_TEXT]->BindTexture("SELECT_LOGO");

	// 矢印
	m_apScene[SELECT_IDX_ARROW] = CScene2D::Create();
	m_apScene[SELECT_IDX_ARROW]->SetPosition(SELECT_MAP1_POSITION + SELECT_MAP_ARROW_OFFSET);
	m_apScene[SELECT_IDX_ARROW]->SetSize(SELECT_MAP_ARROW_SIZE);
	m_apScene[SELECT_IDX_ARROW]->SetTex(1, 1, 0, 0, 0, 0);
	m_apScene[SELECT_IDX_ARROW]->BindTexture("SELECT_ARROW");

	// マップ1 (左)
	m_apScene[SELECT_IDX_MAP1] = CScene2D::Create();
	m_apScene[SELECT_IDX_MAP1]->SetPosition(SELECT_MAP1_POSITION);
	m_apScene[SELECT_IDX_MAP1]->SetSize(SELECT_MAP_SIZE);
	m_apScene[SELECT_IDX_MAP1]->SetTex(1, 1, 0, 0, 0, 0);
	m_apScene[SELECT_IDX_MAP1]->BindTexture("SELECT_EASY");

	// マップ2 (真ん中)
	m_apScene[SELECT_IDX_MAP2] = CScene2D::Create();
	m_apScene[SELECT_IDX_MAP2]->SetPosition(SELECT_MAP2_POSITION);
	m_apScene[SELECT_IDX_MAP2]->SetSize(SELECT_MAP_SIZE);
	m_apScene[SELECT_IDX_MAP2]->SetTex(1, 1, 0, 0, 0, 0);
	m_apScene[SELECT_IDX_MAP2]->BindTexture("SELECT_NORMAL");

	// マップ3 (右)
	m_apScene[SELECT_IDX_MAP3] = CScene2D::Create();
	m_apScene[SELECT_IDX_MAP3]->SetPosition(SELECT_MAP3_POSITION);
	m_apScene[SELECT_IDX_MAP3]->SetSize(SELECT_MAP_SIZE);
	m_apScene[SELECT_IDX_MAP3]->SetTex(1, 1, 0, 0, 0, 0);
	m_apScene[SELECT_IDX_MAP3]->BindTexture("SELECT_HARD");

	// 初期選択
	SelectStage(STAGE_SELECT_1);

	// BGMの再生
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_STAGESELECT) == true)
	{
		m_pSound->Play(CSound::SOUND_LABEL_BGM001);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStageSelect::Uninit(void)
{
	// BGMの停止
	if (m_pSound != NULL && CManager::GetBGMPlay(CSound::BGMTYPE_STAGESELECT) == true)
	{
		m_pSound->Stop();
	}

	// 背景ポインタの開放
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStageSelect::Update(void)
{
	// フェードの取得
	CFade *pFade;
	pFade = CManager::GetFade();
	
	// キー情報の取得
	CInputKeyboard *pKey = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CGamepad *gamepad = CManager::GetInputGamepad();

	if (pFade->GetFade() == CFade::FADE_IN)
	{
		if (pKey->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
		{
			pFade->FadeInCancel();
		}
	}
	else
	{
		// 選択完了
		if (m_bSelected == false) 
		{
			// 決定
			if (pKey->GetTrigger(CInput::KEYINFO_OK) || gamepad->GetTrigger(CInput::KEYINFO_OK)) {
				pFade->SetFade(CFade::FADE_OUT, CMode::MODE_GAME);

				m_pSound->Play(CSound::SOUND_LABEL_SE_SELECTSTAGE);
				m_bSelected = true;
			}
		}
	}

	// 選択ステージを変更
	if (m_bSelected == false)
	{
		if (pKey->GetTrigger(CInput::KEYINFO_RIGHT) || gamepad->GetTrigger(CInput::KEYINFO_RIGHT)) {
			m_currentStage = (STAGE_SELECT)(m_currentStage + 1);
			m_pSound->Play(CSound::SOUND_LABEL_SE_SELECTCURSOR);
		}
		else if (pKey->GetTrigger(CInput::KEYINFO_LEFT) || gamepad->GetTrigger(CInput::KEYINFO_LEFT)) {
			m_currentStage = (STAGE_SELECT)(m_currentStage - 1);
			m_pSound->Play(CSound::SOUND_LABEL_SE_SELECTCURSOR);
		}
	}

	// 余りを計算して繰り返すようにする
	m_currentStage = (STAGE_SELECT)(m_currentStage % 3);
	if (m_currentStage == (STAGE_SELECT)-1) m_currentStage = STAGE_SELECT_3;

	// 矢印の位置を設定
	m_apScene[SELECT_IDX_ARROW]->SetPosition(m_apScene[ToIdx(m_currentStage)]->GetPosition() + SELECT_MAP_ARROW_OFFSET);
	WaveArrow();

	// 選択ステージを拡縮させる
	StageScale(m_currentStage);

	if (m_bSelected == true)
	{
		// 選択後は点滅させる
		StageBlink(m_currentStage);
	}
	
	// 背景をスクロール
	static float fScroll;
	fScroll -= 0.001f;
	m_pBg->GetScene2D()->SetTexUV(
		D3DXVECTOR2(fScroll, SCREEN_HEIGHT / m_pBg->GetScene2D()->GetSize().y + fScroll),
		D3DXVECTOR2(fScroll, fScroll),
		D3DXVECTOR2(SCREEN_WIDTH / m_pBg->GetScene2D()->GetSize().x + fScroll, SCREEN_HEIGHT / m_pBg->GetScene2D()->GetSize().y + fScroll),
		D3DXVECTOR2(SCREEN_WIDTH / m_pBg->GetScene2D()->GetSize().x + fScroll, fScroll));
}

//=============================================================================
// 描画処理
//=============================================================================
void CStageSelect::Draw(void)
{

}

//=============================================================================
// ステージ選択の生成
//=============================================================================
CStageSelect *CStageSelect::Create(void)
{
	// ローカル変数宣言
	CStageSelect *pStageSelect;
	pStageSelect = new CStageSelect;

	if (pStageSelect != NULL)
	{
		pStageSelect->Init();
	}

	return pStageSelect;
}

//=============================================================================
// ステージを選択
//=============================================================================
void CStageSelect::SelectStage(STAGE_SELECT stage)
{
	// 添え字のインデックスに直す
	SELECT_IDX stageIdx = ToIdx(stage);

	// 該当オブジェクトはサイズと色を変える
	for (int nCnt = 0; nCnt < SELECT_IDX_MAX; nCnt++) {
		if (nCnt == stageIdx) {
			m_apScene[nCnt]->SetSize(SELECT_MAP_SIZE_SELECTED);
			m_apScene[nCnt]->SetCol(D3DXCOLOR(1, 1, 1, 1));
		}
		else {
			if (nCnt == SELECT_IDX_MAP1 || nCnt == SELECT_IDX_MAP2 || nCnt == SELECT_IDX_MAP3) {
				m_apScene[nCnt]->SetSize(SELECT_MAP_SIZE);
				m_apScene[nCnt]->SetCol(D3DXCOLOR(1, 1, 1, 0.5f));
			}
		}
			m_apScene[nCnt]->Update();
	}
}

//=============================================================================
// サイズを拡大縮小する
//=============================================================================
void CStageSelect::StageScale(STAGE_SELECT stage)
{
	// 添え字のインデックスに直す
	SELECT_IDX stageIdx = ToIdx(stage);

	// 該当オブジェクトはsinウェーブを使って拡大縮小を繰り返す
	for (int nCnt = 0; nCnt < SELECT_IDX_MAX; nCnt++) {
		if (nCnt == stageIdx) {
			D3DXVECTOR2 size = m_apScene[nCnt]->GetSize();
			float fWave = sinf((float)timeGetTime() * SELECT_MAP_SIZE_FREQUENCY) * SELECT_MAP_SIZE_AMPLITUDE;
			size.x = SELECT_MAP_SIZE_SELECTED.x + fWave;
			size.y = SELECT_MAP_SIZE_SELECTED.y + fWave;
			m_apScene[nCnt]->SetSize(size);
			m_apScene[nCnt]->SetCol(D3DXCOLOR(1, 1, 1, 1));
		}
		else if (nCnt == SELECT_IDX_MAP1 || nCnt == SELECT_IDX_MAP2 || nCnt == SELECT_IDX_MAP3) {
			m_apScene[nCnt]->SetSize(SELECT_MAP_SIZE);
			m_apScene[nCnt]->SetCol(D3DXCOLOR(1, 1, 1, 0.5f));
		}
		m_apScene[nCnt]->Update();
	}

}

//=============================================================================
// ポリゴンを点滅させる
//=============================================================================
void CStageSelect::StageBlink(STAGE_SELECT stage)
{
	// 添え字に直す
	SELECT_IDX stageIdx = ToIdx(stage);

	// sinウェーブにしきい値を設定して点滅させる
	for (int nCnt = 0; nCnt < SELECT_IDX_MAX; nCnt++) {
		if (nCnt == stageIdx) {
			float fWave = sinf((float)timeGetTime() * SELECT_MAP_BLINK_FREQUENCY);
			if (fWave > 0) {
				m_apScene[nCnt]->SetCol(D3DXCOLOR(1, 1, 1, 1));
			}
			else {
				m_apScene[nCnt]->SetCol(D3DXCOLOR(1, 1, 1, 0));
			}
		}
	}
}

//=============================================================================
// 矢印を揺れさせる
//=============================================================================
void CStageSelect::WaveArrow(void)
{
	// sinウェーブ使って揺れさせる
	D3DXVECTOR3 arrowPos = m_apScene[SELECT_IDX_ARROW]->GetPosition();

	float fWave = sinf((float)timeGetTime() * SELECT_MAP_ARROW_FREQUENCY) * SELECT_MAP_ARROW_AMPLITUDE;
	m_apScene[SELECT_IDX_ARROW]->SetPosition(D3DXVECTOR3(arrowPos.x, arrowPos.y + fWave, arrowPos.z));
}

//=============================================================================
// 添え字のインデックスに直す
//=============================================================================
CStageSelect::SELECT_IDX CStageSelect::ToIdx(STAGE_SELECT stage)
{
	SELECT_IDX stageIdx;
	if (stage == STAGE_SELECT_1) {
		stageIdx = SELECT_IDX_MAP1;
	}
	else if (stage == STAGE_SELECT_2) {
		stageIdx = SELECT_IDX_MAP2;
	}
	else if (stage == STAGE_SELECT_3) {
		stageIdx = SELECT_IDX_MAP3;
	}

	return stageIdx;
}
