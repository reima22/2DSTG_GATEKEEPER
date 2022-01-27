//=============================================================================
//
// ボーナスカウント描画処理 [bonus_count.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _BONUS_COUNT_H_
#define _BONUS_COUNT_H_

#include "main.h"
#include "bonus.h"
#include "scene2D.h"

// マクロ定義
#define BONUS_DIGIT_NUM	(3)	// 最大桁数

// 前方宣言
class CNumber;

//==============================================================================
// 落下数カウントクラス
//==============================================================================
class CBonusCount : public CBonus
{
public:
	CBonusCount();
	~CBonusCount();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBonusCount *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

	// 数値の設定
	int GetNumber(void) { return m_nBonus; }
	void SetNumber(int nBonus);

	// 数の加算
	void AddBonus(int nAdd);

	void SizeReturn(void);

	// リザルトへの継承
	static int GetBonusNum(void) { return m_nBonusResult; }

	// 中間地点で保存
	static void SaveBonus(int nSave) { m_nSaveBonus = nSave; }

private:
	CNumber *m_apNumber[BONUS_DIGIT_NUM];	// ナンバークラスポインタ
	int m_nBonus;							// 落下数値
	static int m_nSaveBonus;				// 中間地点で保存する値
	static int m_nBonusResult;				// リザルトへ引き継ぐ値
};
#endif