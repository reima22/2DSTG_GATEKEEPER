//==============================================================================
//
// �����_�����O�����krenderer.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "game.h"
#include "debugproc.h"
#include "polygon.h"
#include "camera.h"
#include "input.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_pFont = NULL;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CRenderer::~CRenderer()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// ���[�J���ϐ��錾
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// �f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{ // �`�揈��:�n�[�h�E�F�A�A���_����:�n�[�h�E�F�A�ł̏������s
		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{ // �`�揈��:�\�t�g�E�F�A�A���_����:�n�[�h�E�F�A�ł̏������s
			if (FAILED(m_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{ // �`�揈��:�\�t�g�E�F�A�A���_����:�\�t�g�E�F�A�ł̏������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�(�e�N�X�`���w�i(�|���S���{�̂̐F)�̓��߂Ȃ�)
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �k�����@���
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �g�厞�@���
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// U�l�@�J��Ԃ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// V�l�@�J��Ԃ�

																				// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// �A���t�@�l�̍���	
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �t�H���g�I�u�W�F�N�g�̐���
	D3DXCreateFont(
		m_pD3DDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pFont);

	// �}���`�^�[�Q�b�g�����_�����O
	// ���[�J���ϐ��錾
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// �ۑ��p�ϐ�

	for (int nCnt = 0; nCnt < FEEDBACK_NUM; nCnt++)
	{
		// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̐���
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X�̐���
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_apBuffMT[nCnt],
			NULL);

		// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// ���݂�Z�o�b�t�@���擾(�ۑ�)
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// �����_�����O�^�[�Q�b�g��ݒ肵���������e�N�X�`���ɐݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Z�o�b�t�@��ݒ�
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̃N���A
		m_pD3DDevice->Clear(
			0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// �����_�����O�^�[�Q�b�g�����ɖ߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Z�o�b�t�@�����ɖ߂�
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	// �����_�����O�p�r���[�|�[�g�̐ݒ�
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// �t�B�[�h�o�b�N�p�|���S���̐���
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	// ���_�o�b�t�@�̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	D3DXVECTOR3 pos = SCREEN_CENTER;
	m_fLength = sqrtf(SCREEN_WIDTH / 2.0f * SCREEN_WIDTH / 2.0f + SCREEN_HEIGHT / 2.0f * SCREEN_HEIGHT / 2.0f);
	m_fAngle = atan2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	m_fAlpha = DEFAULT_ALPHA;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	// ���l�̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle) * m_fLength - 0.5f,
		pos.y + cosf(-m_fAngle) * m_fLength - 0.5f,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle - D3DX_PI) * m_fLength - 0.5f,
		pos.y + cosf(m_fAngle - D3DX_PI) * m_fLength - 0.5f,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle) * m_fLength - 0.5f,
		pos.y + cosf(m_fAngle) * m_fLength - 0.5f,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle + D3DX_PI) * m_fLength - 0.5f,
		pos.y + cosf(-m_fAngle + D3DX_PI) * m_fLength - 0.5f,
		0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

	// �e�N�X�`�����_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CRenderer::Uninit(void)
{
	for (int nCnt = 0; nCnt < FEEDBACK_NUM; nCnt++)
	{
		// �e�N�X�`���̊J��
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}

		// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X�̊J��
		if (m_apRenderMT[nCnt] != NULL)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = NULL;
		}

		// �e�N�X�`�������_�����O�pZ�o�b�t�@�̊J��
		if (m_apBuffMT[nCnt] != NULL)
		{
			m_apBuffMT[nCnt]->Release();
			m_apBuffMT[nCnt] = NULL;
		}
	}

	// �t�H���g�̊J��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̊J��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void CRenderer::Update(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	D3DXVECTOR3 pos = SCREEN_CENTER;

	// �t�F�[�h���̎擾 
	CFade::FADE fade = CFade::GetFade();

	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME && fade == CFade::FADE_OUT)
	{
		return;
	}
	else
	{
		// �|���S���̑S�X�V
		CScene::UpdateAll();
	}

	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �f�o�b�O���[�h�̎�
#ifdef _DEBUG

	if (keyboard->GetPress(CInput::KEYINFO_ALPHA_PLUS) == true)
	{
		m_fAlpha += 0.01f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_ALPHA_MINUS) == true)
	{
		m_fAlpha -= 0.01f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_FE_BIG) == true)
	{
		m_fLength += 1.0f;
	}

	if (keyboard->GetPress(CInput::KEYINFO_FE_SMALL) == true)
	{
		m_fLength -= 1.0f;
	}

#endif

	// �G�t�F�N�g�������̏���
	if (m_bAppeal == true)
	{
		m_nAppealCnt++;

		if (m_nAppealCnt >= FEEDBACK_FRAMECNT)
		{
			m_fLength -= FEEDBACK_LENGTH;
			m_nAppealCnt = 0;
			m_fAlpha = DEFAULT_ALPHA;
			m_bAppeal = false;
		}
	}

	// ���l�̏���E����
	if (m_fAlpha >= 1.0f)
	{
		m_fAlpha = 0.99f;
	}

	if (m_fAlpha < 0.0f)
	{
		m_fAlpha = 0.0f;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	// ���l�̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle) * m_fLength,
		pos.y + cosf(-m_fAngle) * m_fLength,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle - D3DX_PI) * m_fLength,
		pos.y + cosf(m_fAngle - D3DX_PI) * m_fLength,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(
		pos.x + sinf(m_fAngle) * m_fLength,
		pos.y + cosf(m_fAngle) * m_fLength - 0.5f,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(
		pos.x + sinf(-m_fAngle + D3DX_PI) * m_fLength,
		pos.y + cosf(-m_fAngle + D3DX_PI) * m_fLength,
		0.0f);

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMT->Unlock();
}

//==============================================================================
// �`�揈��
//==============================================================================
void CRenderer::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// �ۑ��p�ϐ�

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// ���݂�Z�o�b�t�@���擾(�ۑ�)
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// �����_�����O�^�[�Q�b�g���e�N�X�`��[0]�ݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

		// Z�o�b�t�@�̐ݒ�
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);

		// �e�N�X�`��[0]�̃N���A
		m_pD3DDevice->Clear(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// �|���S���̑S�`�揈��
		CScene::DrawAll();

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		m_pD3DDevice->SetStreamSource(
			0,
			m_pVtxBuffMT,
			0,
			sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// �t�B�[�h�o�b�N�p�|���S���փe�N�X�`��[1]��\��t���`��
		m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);

		// �|���S���̕`��
		m_pD3DDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`����J�n���钸�_�C���f�b�N�X
			2);							// �`�悷��v���~�e�B�u��

		// �����_�����O�^�[�Q�b�g�����ɖ߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Z�o�b�t�@�����ɖ߂�
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);

		// �s������L��
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		m_pD3DDevice->SetStreamSource(
			0,
			m_pVtxBuffMT,
			0,
			sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// �t�B�[�h�o�b�N�p�|���S���Ƀe�N�X�`��[0]��\��t���`��
		m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);

		// �|���S���̕`��
		m_pD3DDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`����J�n���钸�_�C���f�b�N�X
			2);							// �`�悷��v���~�e�B�u��

		// ���ɖ߂�
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		// �e�N�X�`��[0]�ƃe�N�X�`��[1]�����ւ���
		pRenderWk = m_apRenderMT[0];
		m_apRenderMT[0] = m_apRenderMT[1];
		m_apRenderMT[1] = pRenderWk;

		pTextureWk = m_apTextureMT[0];
		m_apTextureMT[0] = m_apTextureMT[1];
		m_apTextureMT[1] = pTextureWk;

		// �t�F�[�h�̕`��
		CFade::Draw();

		// �f�o�b�O���[�h�̎�
#ifdef _DEBUG

		// FPS�̕`�揈��
		//DrawFPS();

		CDebugProc::Draw();

#endif

		// �`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================================
// FPS�`��
//==============================================================================
void CRenderer::DrawFPS(void)
{
	// ���[�J���ϐ��錾
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	int nCountFPS = GetFPSCnt();	// FPS�J�E���g�̎擾

	wsprintf(&aStr[0], "FPS:%d\n", nCountFPS);

	// �e�L�X�g�̕`��
	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================================================
// �t�B�[�h�o�b�N�G�t�F�N�g�̔���
//==============================================================================
void CRenderer::SetFeedbackEffect(void)
{
	if (m_bAppeal == false)
	{
		m_fLength += FEEDBACK_LENGTH;
		m_fAlpha += FEEDBACK_ALPHA;
		m_bAppeal = true;
	}
}