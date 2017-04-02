
// AgoraTutorialDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AgoraTutorial.h"
#include "AgoraTutorialDlg.h"
#include "afxdialogex.h"

#include "AGEventDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAgoraTutorialDlg 对话框



CAgoraTutorialDlg::CAgoraTutorialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgoraTutorialDlg::IDD, pParent)
	, m_strChannelName(_T("AgoraTest"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAgoraTutorialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDCHANNEL, m_strChannelName);
	DDX_Control(pDX, IDC_STATIC_REMOTE, m_wndRemote);
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_wndLocal);
}

BEGIN_MESSAGE_MAP(CAgoraTutorialDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNJOIN, &CAgoraTutorialDlg::OnBnClickedBtnjoin)
	ON_BN_CLICKED(IDC_BTNLEAVE, &CAgoraTutorialDlg::OnBnClickedBtnleave)
	ON_BN_CLICKED(IDC_BTNDISVID, &CAgoraTutorialDlg::OnBnClickedBtndisvid)
	ON_BN_CLICKED(IDC_BTNMUTELOCAUD, &CAgoraTutorialDlg::OnBnClickedBtnmutelocaud)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CAgoraTutorialDlg::OnFirstRemoteVideoDecoded)
END_MESSAGE_MAP()


// CAgoraTutorialDlg 消息处理程序

BOOL CAgoraTutorialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	if (_tcslen(APP_ID) == 0) {
		MessageBox(_T("请在源码中加入正确的APP_ID"), _T("错误"), MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	m_lpAgoraObject = CAgoraObject::GetAgoraObject(APP_ID);
	m_lpAgoraObject->SetMsgHandlerWnd(GetSafeHwnd());

	m_lpAgoraObject->GetEngine()->enableVideo();

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_wndLocal.GetSafeHwnd();
	vc.renderMode = RENDER_MODE_FIT;

	m_lpAgoraObject->GetEngine()->setupLocalVideo(vc);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAgoraTutorialDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAgoraTutorialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAgoraTutorialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAgoraTutorialDlg::OnBnClickedBtnjoin()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_strChannelName.IsEmpty())
		MessageBox(_T("频道号不能为空"), _T("提示"), MB_OK | MB_ICONERROR);
	else
		m_lpAgoraObject->JoinChannel(m_strChannelName);
}


void CAgoraTutorialDlg::OnBnClickedBtnleave()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lpAgoraObject->LeaveCahnnel();
}

void CAgoraTutorialDlg::OnBnClickedBtnmutelocaud()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL bStatValue = !m_lpAgoraObject->IsLocalAudioMuted();
	m_lpAgoraObject->MuteLocalAudio(bStatValue);
}

void CAgoraTutorialDlg::OnBnClickedBtndisvid()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL bStatValue = !m_lpAgoraObject->IsVideoEnabled();
	m_lpAgoraObject->MuteLocalAudio(bStatValue);
}

LRESULT CAgoraTutorialDlg::OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;

	VideoCanvas vc;

	vc.renderMode = RENDER_MODE_FIT;
	vc.uid = lpData->uid;
	vc.view = m_wndRemote.GetSafeHwnd();

	m_lpAgoraObject->GetEngine()->setupRemoteVideo(vc);

	delete lpData;

	return 0;
}

