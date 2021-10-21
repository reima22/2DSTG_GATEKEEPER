//=============================================================================
//
// カメラの設定 [camera.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//==============================================================================
// カメラクラス
//==============================================================================
class CCamera
{
public:
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);

	void SetCamera(void);

private:
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
};
#endif