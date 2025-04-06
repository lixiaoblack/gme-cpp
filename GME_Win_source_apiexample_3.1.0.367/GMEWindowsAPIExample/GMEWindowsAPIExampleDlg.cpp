
// GMEWindowsAPIExampleDlg.cpp : implementation file
//

#include "stdafx.h"

#include <sstream>

#include "EnginePollHelper.h"
#include "GMEWindowsAPIExample.h"
#include "GMEWindowsAPIExampleDlg.h"
#include "Utils.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TMGSDK_APPID AppID To Replace Here https://cloud.tencent.com/document/product/607/10782  // This TMGSDK_APPID corresponds to your game.
#define ACCOUNT_KEY \
  AppKey To Replace Here https://cloud.tencent.com/document/product/607/10782  // The password of the account. It is for testing at
                                                                      // present,

#define TIMER_POLL 0x100

CGMEWindowsAPIExampleDlg::CGMEWindowsAPIExampleDlg(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_GME_API_EXAMPLE_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_pCTMGPTTDlg = nullptr;
  m_pRealTimeAudioDlg = nullptr;
}
CGMEWindowsAPIExampleDlg::~CGMEWindowsAPIExampleDlg() {
  ITMGContextGetInstance()->SetTMGDelegate(nullptr);
}

void CGMEWindowsAPIExampleDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_APPID, m_EditAppid);
  DDX_Control(pDX, IDC_EDIT_KEY, m_EditKey);
  DDX_Control(pDX, IDC_EDIT_OPENID, m_EditUserID);
  DDX_Control(pDX, IDC_STATIC_CONTAINER_PTT, m_PTTDlgContainer);
  DDX_Control(pDX, IDC_STATIC_CONTAINER_RTAUDIO, m_RealTimeAudioDlgContainer);
}

BEGIN_MESSAGE_MAP(CGMEWindowsAPIExampleDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_TIMER()
ON_WM_ACTIVATEAPP()
ON_BN_CLICKED(IDC_BUTTON_INIT, &CGMEWindowsAPIExampleDlg::OnBnClickedButtonInit)
ON_BN_CLICKED(IDC_BUTTON_UNINIT, &CGMEWindowsAPIExampleDlg::OnBnClickedButtonUninit)
ON_BN_CLICKED(IDC_BUTTON_ONLINE_DOCUMENT, &CGMEWindowsAPIExampleDlg::OnBnClickedButtonOnlineDocument)
END_MESSAGE_MAP()

BOOL CGMEWindowsAPIExampleDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  SetIcon(m_hIcon, TRUE);   // Set big icon
  SetIcon(m_hIcon, FALSE);  // Set small icon

  // init subViews
  InitViews();

  CString strSDKVersion;
  ITMGContext* pTmgContext = ITMGContextGetInstance();
  strSDKVersion = pTmgContext->GetSDKVersion();
  SetWindowText(L"GME Windows Native Demo " + strSDKVersion);
  pTmgContext->SetAppVersion("Native Demo");  // Just for Test

  return TRUE;
}

void CGMEWindowsAPIExampleDlg::OnCancel() {
  EnginePollHelper::DestroyEnginePollHelper();
  ITMGContextGetInstance()->Uninit();
  PostQuitMessage(0);
}
void CGMEWindowsAPIExampleDlg::OnOK() {}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGMEWindowsAPIExampleDlg::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this);  // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialogEx::OnPaint();
  }
}
void CGMEWindowsAPIExampleDlg::OnActivateApp(BOOL bActive, DWORD dwThreadId) {
  CDialogEx::OnActivateApp(bActive, dwThreadId);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGMEWindowsAPIExampleDlg::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

void CGMEWindowsAPIExampleDlg::OnTimer(UINT_PTR eventId) {
  switch (eventId) {
    case TIMER_POLL:
      ITMGContextGetInstance()->Poll();
      break;
    default:
      break;
  }
}

void CGMEWindowsAPIExampleDlg::CreateAndChangePositionSubDialog(CDialogEx* pSubDialog, UINT nIDTemplate, CWnd* pParentWnd) {
  RECT rcArea;
  pParentWnd->GetWindowRect(&rcArea);
  RECT rcWnd = {0, 0, rcArea.right - rcArea.left, rcArea.bottom - rcArea.top};
  pSubDialog->Create(nIDTemplate, pParentWnd);
  pSubDialog->MoveWindow(&rcWnd);
  pSubDialog->ShowWindow(SW_SHOW);
}

void CGMEWindowsAPIExampleDlg::InitViews() {
  m_EditAppid.SetWindowText(_T(TMGSDK_APPID));
  m_EditKey.SetWindowText(_T(ACCOUNT_KEY));
  m_EditUserID.SetWindowText(CreateOpenId());

  // create real time voice subDialog
  m_pRealTimeAudioDlg = new CTMGRealTimeAudioDlg();
  m_pRealTimeAudioDlg->SetHostDialog(this);
  CreateAndChangePositionSubDialog(m_pRealTimeAudioDlg, IDD_DIALOG_RTAUDIO, &m_RealTimeAudioDlgContainer);

  // create Voice Message & Recognition subDialog
  m_pCTMGPTTDlg = new CTMGPTTDlg();
  m_pCTMGPTTDlg->SetHostDialog(this);
  CreateAndChangePositionSubDialog(m_pCTMGPTTDlg, IDD_DIALOG_PTT, &m_PTTDlgContainer);
}

CString CGMEWindowsAPIExampleDlg::CreateOpenId() {
  srand((unsigned)time(NULL));
  int nNumber = rand() % 10000 + 1;
  CString strOpenID;  // the OpenID must be unique, so random a new one at beginning.
                      // In your game, it should be casted from the exist user's id.
  strOpenID.Format(L"%d", nNumber);
  return strOpenID;
}

void CGMEWindowsAPIExampleDlg::CreateAuthBuffer(const char* strRoomID, char* retAuthBuff, unsigned int& bufferLength) {
  CString strAppID;
  m_EditAppid.GetWindowText(strAppID);
  int nAppid = atoi(CUtils::WCharToUTF8(strAppID));

  CString strOpenID;
  m_EditUserID.GetWindowText(strOpenID);

  CString strKey;
  m_EditKey.GetWindowText(strKey);

  // Generate AuthBuffer for encryption and authentication of relevant features.
  // For release in the production environment, please use the backend deployment key as detailed in
  // https://intl.cloud.tencent.com/document/product/607/12218
  bufferLength = QAVSDK_AuthBuffer_GenAuthBuffer(nAppid, strRoomID, CUtils::WCharToUTF8(strOpenID), CUtils::WCharToUTF8(strKey),
                                                 (unsigned char*)retAuthBuff, bufferLength);
}

// onInitClick
void CGMEWindowsAPIExampleDlg::OnBnClickedButtonInit() {
  CString strAppID;
  m_EditAppid.GetWindowText(strAppID);
  strAppID = strAppID.Trim();
  if (strAppID.IsEmpty()) {
    MessageBox(L"Please input AppId", L"Alert");
    return;
  }

  CString strKey;
  m_EditKey.GetWindowText(strKey);
  strKey = strKey.Trim();
  if (strKey.IsEmpty()) {
    MessageBox(L"Please input Key", L"Alert");
    return;
  }

  CString strOpenID;
  m_EditUserID.GetWindowText(strOpenID);
  strOpenID = strOpenID.Trim();
  if (strOpenID.IsEmpty()) {
    MessageBox(L"Please input OpenId", L"Alert");
    return;
  }

  // step 1/12: get ITMGContext
  // https://cloud.tencent.com/document/product/607/15232#.E8.8E.B7.E5.8F.96.E5.8D.95.E4.BE.8B
  ITMGContext* pTmgContext = ITMGContextGetInstance();

  // step 2/12: Set GME CallBack
  // https://cloud.tencent.com/document/product/607/15232#.E5.9B.9E.E8.B0.83
  pTmgContext->SetTMGDelegate(this);

  // Step 3/12 Init GME
  // https://cloud.tencent.com/document/product/607/15232#.E5.88.9D.E5.A7.8B.E5.8C.96-sdk
  pTmgContext->Init(CUtils::WCharToUTF8(strAppID), CUtils::WCharToUTF8(strOpenID));
  pTmgContext->SetLogLevel(ITMG_LOG_LEVEL_INFO, ITMG_LOG_LEVEL_INFO);

  // Step 4: Poll to trigger callback
  // https://cloud.tencent.com/document/product/607/15232#.E8.A7.A6.E5.8F.91.E4.BA.8B.E4.BB.B6.E5.9B.9E.E8.B0.83
  EnginePollHelper::CreateEnginePollHelper();

  // Step 5/12 if use PTT, apply ptt authbuffer
  // https://cloud.tencent.com/document/product/607/15232#.E5.88.9D.E5.A7.8B.E5.8C.96.E7.9B.B8.E5.85.B3.E6.8E.A5.E5.8F.A3
  unsigned int authBufferLen = 4096;
  char* pAuthBuffer = new char[authBufferLen];
  memset(pAuthBuffer, 0, authBufferLen);
  CreateAuthBuffer(nullptr, pAuthBuffer, authBufferLen);
  pTmgContext->GetPTT()->ApplyPTTAuthbuffer(pAuthBuffer, authBufferLen);
  delete[] pAuthBuffer;
}

void CGMEWindowsAPIExampleDlg::OnEvent(ITMG_MAIN_EVENT_TYPE eventType, const char* data) {
  switch (eventType) {
    case ITMG_MAIN_EVENT_TYPE_ENTER_ROOM:
      // Step 7/12 : Perform the enter room event
      m_pRealTimeAudioDlg->HandleEnterRoomCallback(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_USER_UPDATE:
      // Step 8/12 : User in the room has new state, such as turn on/off microphone
      //			or new one enter/quit the room.
      //			more info please read the document.
      m_pRealTimeAudioDlg->HandleUserInfoUpdateCallback(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_ROOM_DISCONNECT:
      m_pRealTimeAudioDlg->HandleDisconnectRoomCallback(data);
    case ITMG_MAIN_EVENT_TYPE_EXIT_ROOM:
      // Step 12/12 : Perform the exit room event
      m_pRealTimeAudioDlg->HandleExitRoomCallback(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_CHANGE_ROOM_TYPE:
      m_pRealTimeAudioDlg->HandleRoomTypeChangeCallback(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_RECORD_COMPLETE:
      m_pCTMGPTTDlg->HandlePTTRecordComplete(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_UPLOAD_COMPLETE:
      m_pCTMGPTTDlg->HandlePTTUploadComplete(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_DOWNLOAD_COMPLETE:
      m_pCTMGPTTDlg->HandlePTTDownloadComplete(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_PLAY_COMPLETE:
      m_pCTMGPTTDlg->HandlePTTPlayComplete(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_SPEECH2TEXT_COMPLETE:
      m_pCTMGPTTDlg->HandleSPEECH2TEXTComplete(data);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_COMPLETE:
      m_pCTMGPTTDlg->HandleSTREAM2TEXTComplete(data, true);
      break;
    case ITMG_MAIN_EVENT_TYPE_PTT_STREAMINGRECOGNITION_IS_RUNNING:
      m_pCTMGPTTDlg->HandleSTREAM2TEXTComplete(data, false);
      break;
    // case ITMG_MAIN_EVENT_TYPE_VOICE_CHANGER_FETCH_COMPLETE:
    //   m_pRealTimeAudioDlg->HandleFetchVoiceListCallback(data);
    //   break;
    default:
      break;
  }
}

void CGMEWindowsAPIExampleDlg::OnBnClickedButtonUninit() {
  ITMGContextGetInstance()->Uninit();
}

void CGMEWindowsAPIExampleDlg::OnBnClickedButtonOnlineDocument() {
  ShellExecute(NULL, L"open", L"https://cloud.tencent.com/document/product/607/15232", NULL, NULL, SW_SHOWNORMAL);
}
