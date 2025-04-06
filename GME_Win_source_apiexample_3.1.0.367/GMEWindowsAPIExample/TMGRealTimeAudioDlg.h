#pragma once
#include "GMEWindowsAPIExample.h"
#include <string>
#include <vector>


class CGMEWindowsAPIExampleDlg;

class CTMGRealTimeAudioDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTMGRealTimeAudioDlg)

public:
	CTMGRealTimeAudioDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTMGRealTimeAudioDlg();

	void SetHostDialog(CGMEWindowsAPIExampleDlg *pHostDlg);
	void HandleEnterRoomCallback(const char* data);
	void HandleExitRoomCallback(const char* data);
	void HandleUserInfoUpdateCallback(const char* data);
	void HandleDisconnectRoomCallback(const char* data);
	void HandleRoomTypeChangeCallback(const char* data);
	void AppendCallbackInfo2Monitor(const std::string& callbackINfo);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RTAUDIO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


private:
	CEdit m_RoomId;
	CEdit m_EditMonitor;
	CComboBox m_ComBoxRoomType;
	CButton m_EnableMicCheckBox;
	CButton m_EnableSpeakerCheckBox;
	CButton m_EnableLoopbackCheckBox;
	CButton m_EnterRoomBtn;
	CGMEWindowsAPIExampleDlg *m_pHostDlg;

	CString mMonitorLog;
public:
	afx_msg void OnBnClickedButtonEnterroom();
	afx_msg void OnBnClickedButtonExitroom();
	afx_msg void OnBnClickedCheckEnablemic();
	afx_msg void OnBnClickedCheckEnablespeaker();
	afx_msg void OnCbnSelchangeComboRoomtype();
	afx_msg void OnBnClickedCheckEnableloopback();
};
