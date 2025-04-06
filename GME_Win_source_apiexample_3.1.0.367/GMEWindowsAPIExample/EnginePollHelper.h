#pragma once
#include <string>



class EnginePollHelper
{
public:
	static void CreateEnginePollHelper();
	static void DestroyEnginePollHelper();
	static void PauseEnginePollHelper();		// Just for Test, ignore this function 
	static void ResumeEnginePollHelper();		// Just for Test, ignore this function 
private:
	static EnginePollHelper* s_enginePollHelper;


public:
	LRESULT OnTimer(WPARAM wParam);
private:
	EnginePollHelper();
	~EnginePollHelper();
	void SetupWindow();
	void StartTimer();

	HWND m_hWnd;
};
