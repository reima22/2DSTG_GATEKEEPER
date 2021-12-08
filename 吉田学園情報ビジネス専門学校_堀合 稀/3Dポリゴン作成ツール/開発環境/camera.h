//=============================================================================
//
// カメラの設定 [camera.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define CAMERA_TURN			(D3DX_PI / 100.0f)	// カメラ旋回速度
#define CAMERA_SPDOWN		(0.1f)				// カメラの減衰係数
#define CAMERA_LENGTH		(200.0f)			// 視点・注視点間の距離
#define CAMERA_FORNT		(15.0f)				// キャラ正面に注視点を置くキャラとの距離
#define CAMERA_HEIGHT_R		(0.0f)				// 注視点の高さ
#define CAMERA_HEIGHT_V		(-100.0f)			// 視点の高さ
#define CAMERA_LIMIT_MIN	(10.0f)				// カメラの描画する手前限界
#define CAMERA_LIMIT_MAX	(3000.0f)			// カメラの描画する奥限界
#define CAMERA_MOVE			(1.5f)

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

	static CCamera *Create(void);

	void SetCamera(void);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// 向きの取得

	void SetInit(void);

private:
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
	float m_fLength;			// 距離
	float m_fFront;				// 前方距離
	float m_fHeightR;			// 注視点位置
	float m_fHeightV;			// 視点位置

	D3DXVECTOR3 m_move;			// 移動力

	D3DXVECTOR3 m_posVDest;		// 目的の視点
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
};
#endif