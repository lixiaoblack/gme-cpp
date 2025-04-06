
#include "StdAfx.h"

#include "EnginePollHelper.h"
#include "tmg_sdk.h"

class CSLock : public CRITICAL_SECTION
{
public:
	CSLock()
	{
		InitializeCriticalSection(this);
	}
	~CSLock()
	{
		DeleteCriticalSection(this);
	}
	void Lock()
	{
		EnterCriticalSection(this);
	}
	void Unlock()
	{
		LeaveCriticalSection(this);
	}
};

class CSAutoLock
{
public:
	CSAutoLock(CSLock& lock) : lock_(lock)
	{
		lock_.Lock();
	}
	~CSAutoLock()
	{
		lock_.Unlock();
	}
private:
	CSLock& lock_;
	CSAutoLock(const CSAutoLock&);
	CSAutoLock& operator=(const CSAutoLock&);
};


EnginePollHelper* EnginePollHelper::s_enginePollHelper = NULL;
static CSLock s_lock;
static bool s_pollEnabled = true;

void EnginePollHelper::CreateEnginePollHelper()
{
	if (s_enginePollHelper == NULL)
	{
		CSAutoLock autoLock(s_lock);
		if (s_enginePollHelper == NULL)
		{
			s_enginePollHelper = new EnginePollHelper();	
		}
	}
}

void EnginePollHelper::DestroyEnginePollHelper()
{
	if (s_enginePollHelper != NULL)
	{
		CSAutoLock autoLock(s_lock);
		if (s_enginePollHelper != NULL)
		{
			delete s_enginePollHelper;
			s_enginePollHelper = NULL;
		}
	}
}

void EnginePollHelper::PauseEnginePollHelper()
{
	s_pollEnabled = false;
}

void EnginePollHelper::ResumeEnginePollHelper()
{
	s_pollEnabled = true;
}


#define ENGINEPOLL_WNDCLASSNAME _T("GMEEnginePoll_Class")
#define ENGINEPOLL_TIMERID 1024
#define ENGINEPOLL_TIMERELAPSE 33



static LRESULT CALLBACK TimerWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT  * cs = (CREATESTRUCT *)lParam;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);
	}
	else if (uMsg == WM_DESTROY)
	{
		SetWindowLongPtr(hWnd, 0, 0);
	}

	EnginePollHelper* p = NULL;
	if (uMsg == WM_TIMER)
	{
		LONG_PTR px = GetWindowLongPtr(hWnd, 0);
		if (px)
		{
			p = (EnginePollHelper*) px;
		}
	}

	if (p)
	{
		if (uMsg == WM_TIMER)
		{
			return p->OnTimer(wParam);
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


EnginePollHelper::EnginePollHelper()
{
	m_hWnd = NULL;
	SetupWindow();
	StartTimer();
}

void EnginePollHelper::SetupWindow()
{
	if (m_hWnd != 0) return;
	WNDCLASSEX wc    = {sizeof(wc)};
	wc.hInstance     = GetModuleHandle(0);
	wc.lpszClassName = ENGINEPOLL_WNDCLASSNAME;
	wc.lpfnWndProc   = &TimerWndProc;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.cbWndExtra    = sizeof(void*);
	RegisterClassEx(&wc);
	m_hWnd = CreateWindowEx(0, wc.lpszClassName, _T("EnginePollHelper Window"), WS_OVERLAPPED, 0, 0, 100, 100, 
		HWND_MESSAGE, NULL, wc.hInstance, this);
	::ShowWindow(m_hWnd, SW_HIDE);
}

void EnginePollHelper::StartTimer()
{
	::SetTimer(m_hWnd, ENGINEPOLL_TIMERID, ENGINEPOLL_TIMERELAPSE, NULL);
}

EnginePollHelper::~EnginePollHelper()
{
	SetWindowLongPtr(m_hWnd, 0, 0);
	if (::IsWindow(m_hWnd))
	{
		::KillTimer(m_hWnd, ENGINEPOLL_TIMERID);
		DWORD_PTR dwRet;
		::SendMessageTimeout(m_hWnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG, 500, &dwRet);
		m_hWnd = NULL;
	}
}

LRESULT EnginePollHelper::OnTimer(WPARAM wParam)
{
	if (s_pollEnabled) {
		ITMGContextGetInstance()->Poll();
	}
	return true;
}