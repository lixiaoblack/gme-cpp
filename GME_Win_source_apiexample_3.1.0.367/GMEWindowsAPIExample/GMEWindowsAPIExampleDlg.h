
// GMEWindowsAPIExampleDlg.h : header file
//

#pragma once
#include "TMGRealTimeAudioDlg.h"
#include "TMGPTTDlg.h"
#include "tmg_sdk.h"

class CTMGRealTimeAudioDlg;
class CTMGPTTDlg;

// CGMEWindowsAPIExampleDlg dialog
class CGMEWindowsAPIExampleDlg : public CDialogEx, public ITMGDelegate
{
// Construction
public:
	CGMEWindowsAPIExampleDlg(CWnd* pParent = NULL);	// standard constructor
	~CGMEWindowsAPIExampleDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TMGSDK_FOR_AUDIO_APIEXAMPLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog() override;
	virtual void OnCancel() override;
	virtual void OnOK() override;

	afx_msg void OnPaint();
	afx_msg void OnActivateApp(BOOL bActive, DWORD /*dwThreadID*/);
	afx_msg void OnTimer(UINT_PTR eventId);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CString CreateOpenId();
	void InitViews();
	void CreateAndChangePositionSubDialog(CDialogEx *pSubDialog, UINT nIDTemplate, CWnd* pParentWnd);

private:
	CEdit m_EditAppid;
	CEdit m_EditKey;
	CEdit m_EditUserID;
	CStatic m_PTTDlgContainer;
	CStatic m_RealTimeAudioDlgContainer;

	CTMGRealTimeAudioDlg *m_pRealTimeAudioDlg;
	CTMGPTTDlg *m_pCTMGPTTDlg;
public:
	void OnEvent(ITMG_MAIN_EVENT_TYPE eventType, const char* data) override;
	void CreateAuthBuffer(const char* strRoomID, char* retAuthBuff, unsigned int& bufferLength);

	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonUninit();
	afx_msg void OnBnClickedButtonOnlineDocument();
};
