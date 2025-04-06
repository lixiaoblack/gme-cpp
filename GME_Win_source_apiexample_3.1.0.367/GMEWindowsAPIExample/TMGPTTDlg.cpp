// TMGPTTDlg.cpp : implementation file
//

#include "stdafx.h"

#include <sstream>

#include "GMEWindowsAPIExample.h"
#include "TMGPTTDlg.h"
#include "Utils.h"
#include "afxdialogex.h"
#include "av_error.h"
#include "gci-json.h"
#include "jsoncpp\json.h"

// CTMGPTTDlg dialog

IMPLEMENT_DYNAMIC(CTMGPTTDlg, CDialogEx)

CTMGPTTDlg::CTMGPTTDlg(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_DIALOG_PTT, pParent) {
  m_bPlaying = false;
}

CTMGPTTDlg::~CTMGPTTDlg() {}

void CTMGPTTDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_PTT_MONITOR, m_EditMonitor);
  DDX_Control(pDX, IDC_EDIT_PTT_LOCALFILE, m_EditLocalFile);
  DDX_Control(pDX, IDC_EDIT_PTT_REMOTEFILE, m_EditRemoteFile);
  DDX_Control(pDX, IDC_EDIT_PTT_CONVERT_CONTENT, m_EditConvertContent);
  DDX_Control(pDX, IDC_EDIT_PTT_DOWNLOAD, m_EditDownload);
  DDX_Control(pDX, IDC_CHECK_PPT_REALTIME, m_CheckBoxRealTime);
  DDX_Control(pDX, IDC_BUTTON_REC_START, m_ButtonStart);
  DDX_Control(pDX, IDC_BUTTON_REC_UPLOAD, m_ButtonUpload);
  DDX_Control(pDX, IDC_BUTTON_REC_DOWNLOAD, m_ButtonDownload);
  DDX_Control(pDX, IDC_BUTTON_REC_CONVERT, m_ButtonConvert);
}

BEGIN_MESSAGE_MAP(CTMGPTTDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON_REC_START, &CTMGPTTDlg::OnBnClickedButtonRecStart)
ON_BN_CLICKED(IDC_BUTTON_REC_CANCEL, &CTMGPTTDlg::OnBnClickedButtonRecCancel)
ON_BN_CLICKED(IDC_BUTTON_REC_STOP, &CTMGPTTDlg::OnBnClickedButtonRecStop)
ON_BN_CLICKED(IDC_BUTTON_REC_UPLOAD, &CTMGPTTDlg::OnBnClickedButtonRecUpload)
ON_BN_CLICKED(IDC_BUTTON_REC_DOWNLOAD, &CTMGPTTDlg::OnBnClickedButtonRecDownload)
ON_BN_CLICKED(IDC_BUTTON_REC_PLAY, &CTMGPTTDlg::OnBnClickedButtonRecPlay)
ON_BN_CLICKED(IDC_BUTTON_REC_CONVERT, &CTMGPTTDlg::OnBnClickedButtonRecConvert)
ON_BN_CLICKED(IDC_BUTTON_REC_CLEAR, &CTMGPTTDlg::OnBnClickedButtonRecClear)
ON_BN_CLICKED(IDC_CHECK_PPT_REALTIME, &CTMGPTTDlg::OnBnClickedCheckPptRealtime)
ON_BN_CLICKED(IDC_BUTTON_REC_PLAY_LOCAL, &CTMGPTTDlg::OnBnClickedButtonRecPlayLocal)
END_MESSAGE_MAP()

void CTMGPTTDlg::AppendCallbackInfo2Monitor(const std::string& callbackINfo) {
  if (mMonitorLog.GetLength() > 40960) {
    mMonitorLog = "";
  }
  if (!mMonitorLog.IsEmpty()) {
    mMonitorLog.Append(L"\r\n");
  }
  mMonitorLog.Append(CUtils::UTF8ToWChar(callbackINfo).c_str());
  m_EditMonitor.SetWindowText(mMonitorLog);
  m_EditMonitor.LineScroll(100000);
}

void CTMGPTTDlg::SetHostDialog(CGMEWindowsAPIExampleDlg* pHostDlg) {
  m_pHostDlg = pHostDlg;
}

void CTMGPTTDlg::HandlePTTRecordComplete(const char* data) {
  // Step 3/11 handle ITMG_MAIN_EVENT_TYPE_PTT_RECORD_COMPLETE event
  std::string strText = u8"HandlePTTRecordComplete: ret=";
  strText += data;
  AppendCallbackInfo2Monitor(strText);
  m_ButtonStart.EnableWindow(true);

  nsgme::av::Json::Reader reader;
  nsgme::av::Json::Value root;
  bool parseRet = reader.parse(data, root);

  if (!parseRet) {
    AppendCallbackInfo2Monitor(std::string("parse result Json error"));
  } else {
    std::string filepath = root["file_path"].asString();
    m_EditLocalFile.SetWindowText(CUtils::UTF8ToWChar(filepath).c_str());
  }
}

void CTMGPTTDlg::HandlePTTUploadComplete(const char* data) {
  // Step 5/11 handle ITMG_MAIN_EVENT_TYPE_PTT_UPLOAD_COMPLETE event
  std::string strText = u8"HandlePTTUploadComplete: ret=";
  strText += data;
  AppendCallbackInfo2Monitor(strText);

  nsgme::av::Json::Reader reader;
  nsgme::av::Json::Value root;
  bool parseRet = reader.parse(data, root);

  if (!parseRet) {
    AppendCallbackInfo2Monitor(std::string("parse result Json error"));
  } else {
    std::string filepath = root["file_id"].asString();
    m_EditRemoteFile.SetWindowText(CUtils::UTF8ToWChar(filepath).c_str());
  }
}

void CTMGPTTDlg::HandlePTTDownloadComplete(const char* data) {
  // Step 7/11 handle ITMG_MAIN_EVENT_TYPE_PTT_DOWNLOAD_COMPLETE event
  std::string strText = u8"HandlePTTDownloadComplete: ret=";
  strText += data;
  AppendCallbackInfo2Monitor(strText);

  nsgme::av::Json::Reader reader;
  nsgme::av::Json::Value root;
  bool parseRet = reader.parse(data, root);

  if (!parseRet) {
    AppendCallbackInfo2Monitor(std::string("parse result Json error"));
  } else {
    std::string filepath = root["file_path"].asString();
    m_EditDownload.SetWindowText(CUtils::UTF8ToWChar(filepath).c_str());
  }

  m_ButtonDownload.EnableWindow(true);
}

void CTMGPTTDlg::HandlePTTPlayComplete(const char* data) {
  // Step 9/11 handle ITMG_MAIN_EVENT_TYPE_PTT_PLAY_COMPLETE event
  m_bPlaying = false;
  std::string strText = u8"HandlePTTPlayComplete: ret=";
  strText += data;
  AppendCallbackInfo2Monitor(strText);
}

void CTMGPTTDlg::HandleSPEECH2TEXTComplete(const char* data) {
  // Step 11/11 handle ITMG_MAIN_EVENT_TYPE_PTT_SPEECH2TEXT_COMPLETE event
  std::string strText = u8"HandleSPEECH2TEXTComplete: ret=";
  strText += data;
  AppendCallbackInfo2Monitor(strText);

  nsgme::av::Json::Reader reader;
  nsgme::av::Json::Value root;
  bool parseRet = reader.parse(data, root);
  if (!parseRet) {
    AppendCallbackInfo2Monitor(std::string("parse result Json error"));
  } else {
    std::string convertedText = root["text"].asString();
    m_EditConvertContent.SetWindowText(CUtils::UTF8ToWChar(convertedText).c_str());
  }
}

void CTMGPTTDlg::HandleSTREAM2TEXTComplete(const char* data, bool isComplete) {
  std::string strText = u8"HandleSTREAM2TEXTComplete: ret=";
  strText += data;
  AppendCallbackInfo2Monitor(strText);

  nsgme::av::Json::Reader reader;
  nsgme::av::Json::Value root;
  bool parseRet = reader.parse(data, root);
  if (!parseRet) {
    AppendCallbackInfo2Monitor(std::string("parse result Json error"));
  } else {
    if (isComplete) {
      // Step 1.2/3 handle ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_IS_RUNNING event
      m_EditRemoteFile.SetWindowText(CUtils::UTF8ToWChar(root["file_id"].asString()).c_str());
      m_ButtonStart.EnableWindow(true);
    } else {
      // Step 1.3/3 handle ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_COMPLETE event
      std::string isruning = "STREAMINGRECOGNITION_IS_RUNNING";
      m_EditRemoteFile.SetWindowText(CUtils::UTF8ToWChar(isruning).c_str());
    }
    m_EditDownload.SetWindowText(CUtils::UTF8ToWChar(std::string("")).c_str());
    m_EditLocalFile.SetWindowText(CUtils::UTF8ToWChar(root["file_path"].asString()).c_str());
    m_EditConvertContent.SetWindowText(CUtils::UTF8ToWChar(root["text"].asString()).c_str());
  }
}

// handle Start Rec Button click
void CTMGPTTDlg::OnBnClickedButtonRecStart() {
  ITMGContext* pTmgContext = ITMGContextGetInstance();
  if (pTmgContext) {
    static int sCount = 0;
    sCount++;

    int nRet = nsgme::av::AV_OK;
    if (m_CheckBoxRealTime.GetCheck())  // real time voice stream recognition
    {
      std::string rootPath = std::string(pTmgContext->GetLogPath()) + u8"\\stream";
      std::ostringstream stream;
      stream << rootPath << sCount << u8".ogg";
      std::string language = u8"cmn-Hans-CN";
      // step 1.1/3 start recording and real time recognition, need handle
      // ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_IS_RUNNING and HandleSTREAM2TEXTComplete event
      // https://cloud.tencent.com/document/product/607/15232#.E5.90.AF.E5.8A.A8.E6.B5.81.E5.BC.8F.E8.AF.AD.E9.9F.B3.E8.AF.86.E5.88.AB
      pTmgContext->GetPTT()->StartRecordingWithStreamingRecognition(stream.str().c_str(), language.c_str());
    } else {
      std::string rootPath = std::string(pTmgContext->GetLogPath()) + u8"\\record";
      std::ostringstream filePathStream;
      filePathStream << rootPath << sCount << u8".ogg";
      // Step 1/11  start recording
      // https://cloud.tencent.com/document/product/607/15232#.E5.90.AF.E5.8A.A8.E5.BD.95.E9.9F.B3
      nRet = pTmgContext->GetPTT()->StartRecording(filePathStream.str().c_str());
    }

    if (nRet == nsgme::av::AV_ERR_CONTEXT_NOT_START) {
      std::ostringstream ss;
      ss << "Start return Code is:" << nRet << " errorMsg: startrecording failed ptt is Uninitialize";
      AppendCallbackInfo2Monitor(ss.str());
    } else {
      AppendCallbackInfo2Monitor("Start return code is 0");
      m_EditLocalFile.SetWindowText(L"");
      m_EditRemoteFile.SetWindowText(L"");
      m_EditDownload.SetWindowText(L"");
      m_EditConvertContent.SetWindowText(L"");
      m_ButtonStart.EnableWindow(false);
    }
  }
}

// handle Cancel Rec button click
void CTMGPTTDlg::OnBnClickedButtonRecCancel() {
  int retCode = ITMGContextGetInstance()->GetPTT()->CancelRecording();
  std::ostringstream ss;
  ss << "Cancel return Code is:" << retCode;
  AppendCallbackInfo2Monitor(ss.str());
  m_ButtonStart.EnableWindow(true);
}

// handle Stop Rec button click
void CTMGPTTDlg::OnBnClickedButtonRecStop() {
  // Step 2/11 stop recording,  need handle ITMG_MAIN_EVENT_TYPE_PTT_RECORD_COMPLETE event
  // https://cloud.tencent.com/document/product/607/15232#.E5.81.9C.E6.AD.A2.E5.BD.95.E9.9F.B3
  int retCode = ITMGContextGetInstance()->GetPTT()->StopRecording();
  std::ostringstream ss;
  ss << "Stop return Code is:" << retCode;
  AppendCallbackInfo2Monitor(ss.str());
}

// handle Upload button click
void CTMGPTTDlg::OnBnClickedButtonRecUpload() {
  CString strText;
  m_EditLocalFile.GetWindowText(strText);
  strText = strText.Trim();
  if (strText.IsEmpty()) {
    AppendCallbackInfo2Monitor("Record file is not exist, please Record first");
    return;
  }
  // Step 4/11 upload voice file to server, need handle ITMG_MAIN_EVENT_TYPE_PTT_UPLOAD_COMPLETE event
  // https://cloud.tencent.com/document/product/607/15232#.E4.B8.8A.E4.BC.A0.E8.AF.AD.E9.9F.B3.E6.96.87.E4.BB.B6
  ITMGContextGetInstance()->GetPTT()->UploadRecordedFile(CUtils::WCharToUTF8(strText));
}

void CTMGPTTDlg::OnBnClickedButtonRecDownload() {
  CString strText;
  m_EditRemoteFile.GetWindowText(strText);
  strText = strText.Trim();
  if (strText.IsEmpty()) {
    AppendCallbackInfo2Monitor("Download file error, please upload file first");
    return;
  } else {
    AppendCallbackInfo2Monitor("Start download audio file from server");
  }
  ITMGContext* pTmgContext = ITMGContextGetInstance();

  std::string time = CUtils::GetCurrentTimeStr();
  std::string rootPath = std::string(pTmgContext->GetLogPath()) + "\\download";
  std::string filePath = rootPath + time + u8".ogg";

  // Step 6/11 download vocie file from server, need handle ITMG_MAIN_EVENT_TYPE_PTT_DOWNLOAD_COMPLETE event
  // https://cloud.tencent.com/document/product/607/15232#.E4.B8.8B.E8.BD.BD.E8.AF.AD.E9.9F.B3.E6.96.87.E4.BB.B6
  pTmgContext->GetPTT()->DownloadRecordedFile(CUtils::WCharToUTF8(strText), filePath.c_str());
  m_ButtonDownload.EnableWindow(false);
}

void CTMGPTTDlg::OnBnClickedButtonRecPlay() {
  CString strText;
  m_EditDownload.GetWindowText(strText);
  strText = strText.Trim();
  if (strText.IsEmpty()) {
    AppendCallbackInfo2Monitor("Play file error, please download file first");
    return;
  }
  // Step 8/11 play record voice, need handle ITMG_MAIN_EVENT_TYPE_PTT_PLAY_COMPLETE event
  // https://cloud.tencent.com/document/product/607/15232#.E6.92.AD.E6.94.BE.E8.AF.AD.E9.9F.B3
  ITMGContextGetInstance()->GetPTT()->PlayRecordedFile(CUtils::WCharToUTF8(strText));
  m_bPlaying = true;
}

void CTMGPTTDlg::OnBnClickedButtonRecConvert() {
  CString strText;
  m_EditRemoteFile.GetWindowText(strText);
  strText = strText.Trim();
  if (strText.IsEmpty()) {
    AppendCallbackInfo2Monitor("Convert voice to text error, please upload file first");
    return;
  }
  // Step 10/11 recognition voice, need handle ITMG_MAIN_EVENT_TYPE_PTT_SPEECH2TEXT_COMPLETE envent
  // https://cloud.tencent.com/document/product/607/15232#.E5.B0.86.E6.8C.87.E5.AE.9A.E7.9A.84.E8.AF.AD.E9.9F.B3.E6.96.87.E4.BB.B6.E8.AF.86.E5.88.AB.E6.88.90.E6.96.87.E5.AD.97
  ITMGContextGetInstance()->GetPTT()->SpeechToText(CUtils::WCharToUTF8(strText));
}

void CTMGPTTDlg::OnBnClickedButtonRecClear() {
  mMonitorLog = "";
  AppendCallbackInfo2Monitor("");
  m_EditLocalFile.SetWindowText(L"");
  m_EditRemoteFile.SetWindowText(L"");
  m_EditDownload.SetWindowTextW(L"");
  m_EditConvertContent.SetWindowText(L"");
}

void CTMGPTTDlg::OnBnClickedCheckPptRealtime() {
  // TODO: Add your control notification handler code here
  if (m_CheckBoxRealTime.GetCheck()) {
    m_ButtonUpload.EnableWindow(false);
    m_ButtonConvert.EnableWindow(false);
  } else {
    m_ButtonUpload.EnableWindow(true);
    m_ButtonConvert.EnableWindow(true);
  }
}

void CTMGPTTDlg::OnBnClickedButtonRecPlayLocal() {
  CString strText;
  m_EditLocalFile.GetWindowText(strText);
  strText = strText.Trim();
  if (strText.IsEmpty()) {
    AppendCallbackInfo2Monitor("Play file error, please record file first");
    return;
  }

  ITMGContextGetInstance()->GetPTT()->PlayRecordedFile(CUtils::WCharToUTF8(strText));
  m_bPlaying = true;
}
