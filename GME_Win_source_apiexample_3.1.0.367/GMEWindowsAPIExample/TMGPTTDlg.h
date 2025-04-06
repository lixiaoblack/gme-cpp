#pragma once

#include "GMEWindowsAPIExampleDlg.h"
#include <string>

// CTMGPTTDlg dialog
class CGMEWindowsAPIExampleDlg;
class CTMGPTTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTMGPTTDlg)

public:
	CTMGPTTDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTMGPTTDlg();

	void SetHostDialog(CGMEWindowsAPIExampleDlg *pHostDlg);
	void HandlePTTRecordComplete(const char* data);
	void HandlePTTUploadComplete(const char* data);
	void HandlePTTDownloadComplete(const char* data);
	void HandlePTTPlayComplete(const char* data);
	void HandleSPEECH2TEXTComplete(const char* data);
	void HandleSTREAM2TEXTComplete(const char* data, bool isComplete);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PTT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	void AppendCallbackInfo2Monitor(const std::string& callbackINfo);
private:
	CEdit m_EditLocalFile;
	CEdit m_EditRemoteFile;
	CEdit m_EditDownload;
	CEdit m_EditConvertContent;
	CEdit m_EditMonitor;
	CButton m_CheckBoxRealTime;
	CButton m_ButtonStart, m_ButtonUpload, m_ButtonDownload, m_ButtonConvert;


	CGMEWindowsAPIExampleDlg *m_pHostDlg;
	CString mMonitorLog;
	bool m_bPlaying;
public:
	afx_msg void OnBnClickedButtonRecStart();
	afx_msg void OnBnClickedButtonRecCancel();
	afx_msg void OnBnClickedButtonRecStop();
	afx_msg void OnBnClickedButtonRecUpload();
	afx_msg void OnBnClickedButtonRecDownload();
	afx_msg void OnBnClickedButtonRecPlay();
	afx_msg void OnBnClickedButtonRecConvert();
	afx_msg void OnBnClickedButtonRecClear();
	afx_msg void OnBnClickedCheckPptRealtime();
	afx_msg void OnBnClickedButtonRecPlayLocal();
};
