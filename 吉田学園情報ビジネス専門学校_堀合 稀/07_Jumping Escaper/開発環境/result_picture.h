//=============================================================================
//
// リザルトに描画する図の処理 [result_picture.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RESULT_PICTURE_H_
#define _RESULT_PICTURE_H_

#include "main.h"
#include "scene.h"
#include "result.h"

// マクロ定義
#define R_PICT_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f))	// ゲーム結果表示の位置
#define R_PICT_SIZE				(D3DXVECTOR2(480.0f, 80.0f))					// ゲーム結果表示のサイズ
		  
#define R_PICT_SCORE_POS		(D3DXVECTOR3(250.0f, 260.0f, 0.0f))				// スコアロゴの位置
#define R_PICT_SCORE_SIZE		(D3DXVECTOR2(160.0f, 50.0f))					// スコアロゴのサイズ

#define R_PICT_TIMER_POS		(D3DXVECTOR3(100.0f, 400.0f, 0.0f))				// タイマーロゴの位置
#define R_PICT_TIMER_SIZE		(D3DXVECTOR2(100.0f, 50.0f))						// タイマーロゴのサイズ
#define R_PICT_TIMER_MARK_POS_0	(D3DXVECTOR3(440.0f, 400.0f, 0.0f))				// タイマー算用記号０の位置
#define R_PICT_TIMER_MARK_POS_1	(D3DXVECTOR3(550.0f, 400.0f, 0.0f))				// タイマー算用記号１の位置
#define R_PICT_TIMER_MARK_POS_2	(D3DXVECTOR3(840.0f, 400.0f, 0.0f))				// タイマー算用記号２の位置

#define R_PICT_COUNT_POS		(D3DXVECTOR3(100.0f, 525.0f, 0.0f))				// カウントロゴの位置
#define R_PICT_COUNT_SIZE		(D3DXVECTOR2(100.0f, 50.0f))						// カウントロゴのサイズ
#define R_PICT_COUNT_MARK_POS_0	(D3DXVECTOR3(440.0f, 525.0f, 0.0f))				// カウント算用記号０の位置
#define R_PICT_COUNT_MARK_POS_1	(D3DXVECTOR3(550.0f, 525.0f, 0.0f))				// カウント算用記号１の位置
#define R_PICT_COUNT_MARK_POS_2	(D3DXVECTOR3(840.0f, 525.0f, 0.0f))				// カウント算用記号２の位置

#define R_PICT_BONUS_POS		(D3DXVECTOR3(100.0f, 650.0f, 0.0f))				// ボーナスロゴの位置
#define R_PICT_BONUS_SIZE		(D3DXVECTOR2(100.0f, 50.0f))						// ボーナスロゴのサイズ
#define R_PICT_BONUS_MARK_POS_0	(D3DXVECTOR3(440.0f, 650.0f, 0.0f))				// ボーナス算用記号０の位置
#define R_PICT_BONUS_MARK_POS_1	(D3DXVECTOR3(550.0f, 650.0f, 0.0f))				// ボーナス算用記号１の位置
#define R_PICT_BONUS_MARK_POS_2	(D3DXVECTOR3(840.0f, 650.0f, 0.0f))				// ボーナス算用記号２の位置

#define R_PICT_MARK_SIZE		(D3DXVECTOR2(40.0f, 40.0f))						// 算用記号のサイズ

// 前方宣言
class CScene2D;	// 2Dポリゴン描画クラス

//==============================================================================
// リザルト画面に描画する図のクラス
//==============================================================================
class CResultPicture : public CResult
{
public:
	CResultPicture(int nPriority = PRIORITY_UI);
	~CResultPicture();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, char *pTypeCheck);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CResultPicture *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, char *pTypeCheck);

private:
	CScene2D *m_pScene2D;	// 2Dポリゴンクラス
};
#endif