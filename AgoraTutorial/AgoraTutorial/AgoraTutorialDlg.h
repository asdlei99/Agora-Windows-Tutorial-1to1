
// AgoraTutorialDlg.h : 头文件
//

#pragma once
#include "afxmenubutton.h"
#include "AgoraObject.h"
#include "afxwin.h"


// CAgoraTutorialDlg 对话框
class CAgoraTutorialDlg : public CDialog
{
// 构造
public:
	CAgoraTutorialDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AGORATUTORIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedBtnjoin();
	afx_msg void OnBnClickedBtnleave();
	afx_msg void OnBnClickedBtndisvid();
	afx_msg void OnBnClickedBtnmutelocaud();

	// ENGINE EVENT MESSAGE
	afx_msg LRESULT OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CString			m_strChannelName;

	CAgoraObject	*m_lpAgoraObject;

	CStatic		m_wndRemote;
	CStatic		m_wndLocal;
};
