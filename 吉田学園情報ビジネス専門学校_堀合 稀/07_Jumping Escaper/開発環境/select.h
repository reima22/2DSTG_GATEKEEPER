//=============================================================================
//
// ステージセレクト [select.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

//*****************************************************************************
// ヘッダのインクルード
//*****************************************************************************
#include "main.h"
#include "Scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SELECT_BG_POSITION			(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))	// 背景位置
#define SELECT_BG_SIZE				(D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f))		// 背景サイズ

#define SELECT_TEXT_SIZE			(D3DXVECTOR2(400.0f, 60.0f))					// テキスト板ポリゴンのサイズ
#define SELECT_TEXT_POSITION		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 90.0f, 0.0f))	// 位置

#define SELECT_MAP_SIZE				(D3DXVECTOR2(130.0f, 130.0f))	// ステージのサイズ
#define SELECT_MAP_SIZE_SELECTED	(D3DXVECTOR2(180.0f, 180.0f))	// 選択時のサイズ
#define SELECT_MAP_SIZE_AMPLITUDE	(10.0f)							// 選択時のサイズの振れ幅
#define SELECT_MAP_SIZE_FREQUENCY	(0.01f)							// サイズの変更する速度
#define SELECT_MAP_BLINK_FREQUENCY	(1.0f)							// ポリゴン点滅の速度

#define SELECT_MAP_ARROW_SIZE		(D3DXVECTOR2(40.0f, 60.0f))			// 矢印のサイズ
#define SELECT_MAP_ARROW_OFFSET		(D3DXVECTOR3(0.0f, -250.0f, 0.0f))	// 矢印の位置オフセット
#define SELECT_MAP_ARROW_AMPLITUDE	(10.0f)								// 矢印の揺れの振れ幅
#define SELECT_MAP_ARROW_FREQUENCY	(0.01f)								// 矢印の揺れの頻度

#define SELECT_MAP_OFFSET			(D3DXVECTOR3(380.0f, 150.0f, 0.0f))																			// マップのポリゴン位置オフセット
#define SELECT_MAP1_POSITION		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - SELECT_MAP_OFFSET.x, SCREEN_HEIGHT / 2.0f + SELECT_MAP_OFFSET.y, 0.0f))	// ステージ１UI位置
#define SELECT_MAP2_POSITION		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + SELECT_MAP_OFFSET.y, 0.0f))						// ステージ２UI位置
#define SELECT_MAP3_POSITION		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SELECT_MAP_OFFSET.x, SCREEN_HEIGHT / 2.0f + SELECT_MAP_OFFSET.y, 0.0f))	// ステージ３UI位置

// 前方宣言
class CBg;

//*****************************************************************************
// ステージセレクトクラス
//*****************************************************************************
class CStageSelect : public CScene
{
public:
	// ステージの種類
	typedef enum {
		STAGE_SELECT_1 = 0,
		STAGE_SELECT_2,
		STAGE_SELECT_3,
		
		STAGE_SELECT_MAX,
	} STAGE_SELECT;

	CStageSelect();
	~CStageSelect();

	static STAGE_SELECT GetSelectingStage(void) { return m_currentStage; }	// 選択しているステージを取得

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStageSelect *Create(void);

private:
	typedef enum {	// 配列の添え字用インデックス,
		SELECT_IDX_TEXT = 0,
		SELECT_IDX_MAP1,
		SELECT_IDX_MAP2,
		SELECT_IDX_MAP3,
		SELECT_IDX_ARROW,

		SELECT_IDX_MAX,
	} SELECT_IDX;

	void SelectStage(STAGE_SELECT stage);	// ステージ選択
	void StageScale(STAGE_SELECT stage);	// ステージの拡縮
	void StageBlink(STAGE_SELECT stage);	// ステージの点滅
	void WaveArrow(void);					// 矢印を揺れさせる

	SELECT_IDX ToIdx(STAGE_SELECT stage);	// 添え字への補正

	CBg *m_pBg;								// 背景クラス

	CScene2D *m_apScene[SELECT_IDX_MAX];	// 2Dオブジェクト

	static STAGE_SELECT m_currentStage;		// 現在選択しているステージ

	bool m_bSelected;						// 選択完了
};

#endif


