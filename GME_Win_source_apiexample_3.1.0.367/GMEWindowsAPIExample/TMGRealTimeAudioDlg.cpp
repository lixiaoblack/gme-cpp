// TMGRealTimeAudioDlg.cpp : implementation file
//

#include "stdafx.h"

#include <algorithm>
#include <sstream>

#include "GMEWindowsAPIExampleDlg.h"
#include "TMGRealTimeAudioDlg.h"
#include "Utils.h"
#include "afxdialogex.h"
#include "av_error.h"
#include "jsoncpp\json.h"
#include "tmg_sdk.h"

#define TMG_INIT_ROOMID "20210801"  // The audio room's id, in the same room id, OpenIDs can talk with each other.

// CTMGRealTimeAudioDlg dialog

IMPLEMENT_DYNAMIC(CTMGRealTimeAudioDlg, CDialogEx)

CTMGRealTimeAudioDlg::CTMGRealTimeAudioDlg(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_DIALOG_RTAUDIO, pParent) {}

CTMGRealTimeAudioDlg::~CTMGRealTimeAudioDlg() {}

void CTMGRealTimeAudioDlg::AppendCallbackInfo2Monitor(const std::string& callbackINfo) {
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

void CTMGRealTimeAudioDlg::SetHostDialog(CGMEWindowsAPIExampleDlg* pHostDlg) {
  m_pHostDlg = pHostDlg;
}

void CTMGRealTimeAudioDlg::HandleEnterRoomCallback(const char* data) {
  std::string strText = u8"EnterRoom complete: ret=";
  strText.append(data);

  if (strText.find("\"result\":0") == strText.npos) {
    m_EnterRoomBtn.EnableWindow(true);
  }

  AppendCallbackInfo2Monitor(strText);
}

void CTMGRealTimeAudioDlg::HandleUserInfoUpdateCallback(const char* data) {
  std::string strText = u8"UserUpdate complete: ret=";
  strText.append(data);
  AppendCallbackInfo2Monitor(strText);
}
void CTMGRealTimeAudioDlg::HandleDisconnectRoomCallback(const char* data) {
  std::string strText = u8"UserUpdate complete: ret=";
  strText.append(data);
  AppendCallbackInfo2Monitor(strText);
  m_EnableMicCheckBox.SetCheck(false);
  m_EnableSpeakerCheckBox.SetCheck(false);
  m_EnableLoopbackCheckBox.SetCheck(false);
}
void CTMGRealTimeAudioDlg::HandleExitRoomCallback(const char* data) {
  std::string strText = u8"ExitRoom complete: ret=";
  strText.append(data);
  AppendCallbackInfo2Monitor(strText);
  m_EnableMicCheckBox.SetCheck(false);
  m_EnableSpeakerCheckBox.SetCheck(false);
  m_EnableLoopbackCheckBox.SetCheck(false);
  m_EnterRoomBtn.EnableWindow(true);
}

void CTMGRealTimeAudioDlg::HandleRoomTypeChangeCallback(const char* data) {
  std::string strText = u8"Room type changed: ret=";
  strText.append(data);
  AppendCallbackInfo2Monitor(strText);

  std::string strData = data;
  if (strData.find("\"result\":0") != std::string::npos) {
    if (strData.find("\"sub_event_type\":1") != std::string::npos || strData.find("\"sub_event_type\":3") != std::string::npos) {
      if (strData.find("\"new_room_type\":1") != std::string::npos) {
        m_ComBoxRoomType.SetCurSel(0);
      } else if (strData.find("\"new_room_type\":2") != std::string::npos) {
        m_ComBoxRoomType.SetCurSel(1);
      } else if (strData.find("\"new_room_type\":3") != std::string::npos) {
        m_ComBoxRoomType.SetCurSel(2);
      }
    }
  }
}

void CTMGRealTimeAudioDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_ROOMID, m_RoomId);
  DDX_Control(pDX, IDC_COMBO_ROOMTYPE, m_ComBoxRoomType);
  DDX_Control(pDX, IDC_EDIT_AUDIO_MONITOR, m_EditMonitor);
  DDX_Control(pDX, IDC_CHECK_ENABLEMIC, m_EnableMicCheckBox);
  DDX_Control(pDX, IDC_CHECK_ENABLESPEAKER, m_EnableSpeakerCheckBox);
  DDX_Control(pDX, IDC_CHECK_ENABLELOOPBACK, m_EnableLoopbackCheckBox);
  DDX_Control(pDX, IDC_BUTTON_ENTERROOM, m_EnterRoomBtn);
}

BOOL CTMGRealTimeAudioDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  // init subViews
  m_RoomId.SetWindowText(_T(TMG_INIT_ROOMID));
  m_ComBoxRoomType.AddString(L"1_ROOM_TYPE_FLUENCY");
  m_ComBoxRoomType.AddString(L"2_ROOM_TYPE_STANDARD");
  m_ComBoxRoomType.AddString(L"3_ROOM_TYPE_HIGHQUALITY");
  m_ComBoxRoomType.SetCurSel(0);
  return TRUE;
}
BEGIN_MESSAGE_MAP(CTMGRealTimeAudioDlg, CDialogEx)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BUTTON_ENTERROOM, &CTMGRealTimeAudioDlg::OnBnClickedButtonEnterroom)
ON_BN_CLICKED(IDC_BUTTON_EXITROOM, &CTMGRealTimeAudioDlg::OnBnClickedButtonExitroom)
ON_BN_CLICKED(IDC_CHECK_ENABLEMIC, &CTMGRealTimeAudioDlg::OnBnClickedCheckEnablemic)
ON_BN_CLICKED(IDC_CHECK_ENABLESPEAKER, &CTMGRealTimeAudioDlg::OnBnClickedCheckEnablespeaker)
ON_CBN_SELCHANGE(IDC_COMBO_ROOMTYPE, &CTMGRealTimeAudioDlg::OnCbnSelchangeComboRoomtype)
ON_BN_CLICKED(IDC_CHECK_ENABLELOOPBACK, &CTMGRealTimeAudioDlg::OnBnClickedCheckEnableloopback)
END_MESSAGE_MAP()

// Handler EnterRoomClick
void CTMGRealTimeAudioDlg::OnBnClickedButtonEnterroom() {
  CString cstrRoomId;
  m_RoomId.GetWindowTextW(cstrRoomId);
  std::string utf8RoomId = CUtils::WCharToUTF8(cstrRoomId).GetString();

  unsigned int authBufferLen = 4096;
  char* pAuthBuffer = new char[authBufferLen];
  memset(pAuthBuffer, 0, authBufferLen);
  m_pHostDlg->CreateAuthBuffer(utf8RoomId.c_str(), pAuthBuffer, authBufferLen);

  // Step 6/12 enter room, then you need handle ITMG_MAIN_EVENT_TYPE_ENTER_ROOM,ITMG_MAIN_EVENT_TYPE_USER_UPDATE
  //  and ITMG_MAIN_EVENT_TYPE_ROOM_DISCONNECT event in OnEvent function in CTMGSDK_For_Audio_ApiExampleDlg class
  // https://cloud.tencent.com/document/product/607/15232#.E5.8A.A0.E5.85.A5.E6.88.BF.E9.97.B4
  ITMGContext* pTmgContext = ITMGContextGetInstance();

  ITMG_ROOM_TYPE type = (ITMG_ROOM_TYPE)(m_ComBoxRoomType.GetCurSel() + 1);
  int ret = pTmgContext->EnterRoom(utf8RoomId.c_str(), type, (const char*)pAuthBuffer, authBufferLen);
  char buffer[64] = {0};
  snprintf(buffer, 63, "EnterRoom %s with type %d ret %d", utf8RoomId.c_str(), type, ret);
  AppendCallbackInfo2Monitor(buffer);

  delete[] pAuthBuffer;
  m_EnterRoomBtn.EnableWindow(false);
}

void CTMGRealTimeAudioDlg::OnBnClickedCheckEnablemic() {
  // Step 9/12 enable to capture audio and send to server
  // https://cloud.tencent.com/document/product/607/15232#.E5.BC.80.E5.90.AF.E6.88.96.E5.85.B3.E9.97.AD.E9.BA.A6.E5.85.8B.E9.A3.8E
  ITMGContextGetInstance()->GetAudioCtrl()->EnableMic(m_EnableMicCheckBox.GetCheck() == TRUE);
}

void CTMGRealTimeAudioDlg::OnBnClickedCheckEnablespeaker() {
  // Step 10/12 enable to capture audio and send to server
  // https://cloud.tencent.com/document/product/607/15232#.E5.BC.80.E5.90.AF.E6.88.96.E5.85.B3.E9.97.AD.E6.89.AC.E5.A3.B0.E5.99.A8
  ITMGContextGetInstance()->GetAudioCtrl()->EnableSpeaker(m_EnableSpeakerCheckBox.GetCheck() == TRUE);
}

// Hanlde ExitRoomClick
void CTMGRealTimeAudioDlg::OnBnClickedButtonExitroom() {
  // Step 11/12 exit room
  // https://cloud.tencent.com/document/product/607/15232#.E9.80.80.E5.87.BA.E6.88.BF.E9.97.B4
  ITMGContextGetInstance()->ExitRoom();
  m_EnterRoomBtn.EnableWindow(true);
}

void CTMGRealTimeAudioDlg::OnCbnSelchangeComboRoomtype() {
  // TODO: Add your control notification handler code here
  ITMG_ROOM_TYPE type = (ITMG_ROOM_TYPE)(m_ComBoxRoomType.GetCurSel() + 1);
  int ret = ITMGContextGetInstance()->GetRoom()->ChangeRoomType(type);

  char buffer[64] = {0};
  snprintf(buffer, 63, "ChangeRoomType to %d ret %d", type, ret);
  AppendCallbackInfo2Monitor(buffer);
}

void CTMGRealTimeAudioDlg::OnBnClickedCheckEnableloopback() {
  // TODO: Add your control notification handler code here
  ITMGContextGetInstance()->GetAudioCtrl()->EnableLoopBack(m_EnableLoopbackCheckBox.GetCheck());
}
